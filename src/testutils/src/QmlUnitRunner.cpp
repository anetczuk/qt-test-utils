/// MIT License
///
/// Copyright (c) 2019 Arkadiusz Netczuk <dev.arnet@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include "QmlUnitRunner.h"
#include "TestUtils.h"
#include "FunctionParam.h"
#include "Args.h"

#include <QtQuickTest/quicktest.h>
#include <QApplication>


using namespace testutils;


// QtCreator 4.9.0 crashes when tests name is passed by variable. Indirect call prevents the crash.
#define TESTS_MAIN quick_test_main##_with_setup


namespace quicktestutils {

    QStringList get_functions(const CmdParser& args) {
        CmdParser argsList( args );
        argsList.append("-functions");

        const QStringList& params = argsList.list();

        QProcess qmlProcess;
        executeProcess(qmlProcess, params);

        // example of "-functions" output:
        //   qml: RectangleSize::test_height()
        //   qml: RectangleSize::test_width()

        QString output( qmlProcess.readAll() );

        output.replace("\r", "");
        output.replace("qml: ", "");
        output.replace("()", "");
        QStringList foundFunctions = output.split("\n");
        return foundFunctions;
    }

    QStringList get_functions(const QStringList& args) {
        CmdParser params( args );
        return get_functions(params);
    }

    QStringList find_methods(const QStringList& testCaseFunctions, const QString& runFunction) {
        const FunctionParam class_function = FunctionParam::splitForQuickTest( runFunction );
        const QString pattern = class_function.getPattern();
        QRegularExpression caseRE = prepare_regex( pattern );

        QStringList retList;
        const int methodsNum = testCaseFunctions.size();
        for(int i=0; i<methodsNum; ++i) {
            const QString& testCase = testCaseFunctions[i];
            //qDebug() << "executing pattern against value:" << pattern << testCase;
            QRegularExpressionMatch match = caseRE.match( testCase );
            if (match.hasMatch()) {
                //qDebug() << "found matching:" << pattern << testCase;
                retList.append( testCase );
            }
        }

        // remove duplicates
        QSet<QString> stringSet = retList.toSet();
        retList = stringSet.toList();
        retList.sort();

        return retList;
    }

    QStringList find_methods(const QStringList& testCaseFunctions, const QStringList& functionPatterns) {
        QStringList retList;
        const int patternNum = functionPatterns.size();
        for(int i=0; i<patternNum; ++i) {
            const QString& runFunction = functionPatterns[i];
            QStringList list = find_methods(testCaseFunctions, runFunction);
            retList.append( list );
        }

        // remove duplicates
        QSet<QString> stringSet = retList.toSet();
        retList = stringSet.toList();
        retList.sort();

        return retList;
    }


    // ========================================================================


    int QmlUnitRunner::run(const QStringList& arguments, const char *sourceDir, QObject* setup) {
        CmdParser params( arguments );
        return run(params, sourceDir, setup);
    }

    int QmlUnitRunner::run(const CmdParser& arguments, const char *sourceDir, QObject* setup) {
        //qDebug() << "running qml with args:" << arguments << sourceDir;
        if (arguments.contains("-functions")) {
            // print functions
            return executeQml(arguments, sourceDir, setup);
        }

        const QStringList runFunctions = arguments.extractFunctions();
        if ( runFunctions.empty() ) {
            // execute all tests
            return executeQml(arguments, sourceDir, setup);
        }

        CmdParser commonArgs( arguments );
        commonArgs.removeAll( runFunctions );

        const QStringList testCaseFunctions = getFunctionsList( commonArgs );

        //qDebug() << "found functions:" << testCaseFunctions;

        const QStringList foundMethods = find_methods( testCaseFunctions, runFunctions );
        if (foundMethods.size() < 1) {
            // test case does not have searching functions
//            qDebug() << "no test cases found";
            return 0;
        }

        // qDebug() << "found methods:" << foundMethods;

        commonArgs.append( foundMethods );
        return executeQml(commonArgs, sourceDir, setup);
    }

    int QmlUnitRunner::executeQml(const CmdParser& arguments, const char *sourceDir, QObject* setup) {
        if ( QFile::exists(sourceDir) == false ) {
            if (arguments.contains("-input") == false)
                qWarning() << "Given source directory does not exists:" << sourceDir;
        }
        QStringList params = arguments.extractQmlParams();
        Args argsObj(params);
        const int argc = argsObj.argc();
        char** argv = argsObj.argv();
        const char* name = testsName.c_str();
        return TESTS_MAIN(argc, argv, name, sourceDir, setup);
    }

    QStringList QmlUnitRunner::getFunctionsList(const testutils::CmdParser& arguments) const {
        return get_functions(arguments);
    }

}
