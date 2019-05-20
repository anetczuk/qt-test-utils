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

#include "QuickTestRegister.h"
#include <sstream>

#include "QtTestRegister.h"
#include "FunctionParam.h"


using namespace testutils;


namespace quicktestutils {

    QStringList find_methods(const QStringList& testCaseFunctions, const QString& pattern) {
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
        return retList;
    }

    QStringList find_methods(const QStringList& testCaseFunctions, const std::vector<FunctionParam>& functionsParams) {
        QStringList retList;
        for(const FunctionParam& item: functionsParams) {
            const QString pattern = item.getPattern();
            const QStringList names = find_methods(testCaseFunctions, pattern);
            retList.append( names );
        }

        // remove duplicates
        QSet<QString> stringSet = retList.toSet();
        retList = stringSet.toList();

        return retList;
    }

    QStringList get_functions(const QStringList& args) {
        QStringList argsList;
        argsList << args;
        argsList.insert(1, "-functions");

        QString program = argsList[0];
        argsList.pop_front();

        QProcess myProcess;
        myProcess.setProcessChannelMode(QProcess::MergedChannels);
        myProcess.start(program, argsList);

        if (myProcess.waitForFinished() == false) {
            std::stringstream message;
            message << "function subprocess could not finish: " << myProcess.errorString().toStdString();
            throw std::runtime_error( message.str() );
        }

        const QProcess::ExitStatus exitStatus = myProcess.exitStatus();
        if ( exitStatus != QProcess::NormalExit ) {
            std::stringstream message;
            message << "function subprocess finished with exit status: " << exitStatus;
            throw std::runtime_error( message.str() );
        }

        const int exitCode = myProcess.exitCode();
        if ( exitCode != 0 ) {
            std::stringstream message;
            message << "function subprocess finished with exit code: " << exitCode;
            throw std::runtime_error( message.str() );
        }

        // example of "-functions" output:
        //   qml: RectangleSize::test_height()
        //   qml: RectangleSize::test_width()

        QString output( myProcess.readAll() );
        output.replace("\r", "");
        output.replace("qml: ", "");
        output.replace("()", "");
        QStringList foundFunctions = output.split("\n");
        return foundFunctions;
    }

    int execute_tests(const QStringList& args, const char *sourceDir) {
        Args argsObj(args);
        const int argc = argsObj.argc();
        char** argv = argsObj.argv();

        return quick_test_main(argc, argv, "qmltests", sourceDir);
    }

    int run_tests(int argc, char *argv[], const char *sourceDir) {
        const QStringList arguments = convertArgs( argc, argv );

        const QStringList runFunctions = extract_functions_from_arguments(arguments);
        if ( runFunctions.empty() ) {
            // execute all tests
            return execute_tests(arguments, sourceDir);
        }

        QStringList commonArgs = arguments;
        for(const QString& item: runFunctions) {
            commonArgs.removeAll(item);
        }

        if (arguments.contains("-functions")) {
            // print functions
            return execute_tests(commonArgs, sourceDir);
        }

        const QStringList testCaseFunctions = get_functions( commonArgs );

        const std::vector<FunctionParam> class_function = FunctionParam::splitFunctionsForQuickTest(runFunctions);
        const QStringList foundMethods = find_methods( testCaseFunctions, class_function );
        if (foundMethods.size() < 1) {
            // test case does not have searching functions
            return 0;
        }

        // qDebug() << "found methods:" << foundMethods;

        QStringList testArgs = commonArgs;
        testArgs.append( foundMethods );
        return execute_tests(testArgs, sourceDir);
    }

}

//#include "QuickTestRegister.moc"
