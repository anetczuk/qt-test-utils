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

#include "QtTestRegister.h"

#include "FunctionParam.h"
#include "TestUtils.h"

#include <QtTest/QtTest>


namespace testutils {

    QStringList find_methods(const QObject* testCase, const QString& function) {
        return find_methods(testCase, "", function);
    }

    QStringList find_methods(const QObject* testCase, const QString& className, const QString& functionName) {
        QRegularExpression classRE;
        if (className.isEmpty() == false)
            classRE = prepare_regex( className );
        else
            classRE = prepare_regex( "*" );

        const QString testName = testCase->metaObject()->className();
        QRegularExpressionMatch classMatch = classRE.match( testName );
        if (classMatch.hasMatch() == false) {
            // class does not match
            return QStringList();
        }

        QRegularExpression functionRE = prepare_regex( functionName );

        QStringList retList;
        const QMetaObject* meta = testCase->metaObject();
        const int methodsNum = meta->methodCount();
        for(int i=0; i<methodsNum; ++i) {
            const QMetaMethod method = meta->method(i);
            if (method.methodType() != QMetaMethod::Slot) {
                // skip members other than slots
                continue;
            }
            if (method.access() != QMetaMethod::Private) {
                // skip non-private menbers
                continue;
            }
            const QByteArray mName = method.name();
            if (mName.startsWith("_q_")) {
                // internal Qt member
                continue;
            }
            // qDebug() << "executing pattern against value:" << functionName << mName;
            QRegularExpressionMatch match = functionRE.match( mName );
            if (match.hasMatch()) {
                retList.append( mName );
            }
        }
        return retList;
    }

    QStringList find_methods(const QObject* testCase, const QStringList& functions) {
        QStringList retList;
        const int aSize = functions.size();
        for(int i=0; i<aSize; ++i) {
            const QString& arg = functions[i];
            const QStringList names = find_methods(testCase, arg);
            retList.append( names );
        }
        return retList;
    }

    QStringList find_methods(const QObject* testCase, const QStringList& classes, const QStringList& functions) {
        QStringList retList;
        const int aSize = functions.size();
        for(int i=0; i<aSize; ++i) {
            const QString& className = classes[i];
            const QString& funcName = functions[i];
            const QStringList names = find_methods(testCase, className, funcName);
            retList.append( names );
        }
        return retList;
    }


    // ======================================================


    QStringList find_methods(const QObject* testCase, const std::vector<FunctionParam>& functionsParams) {
        QStringList retList;
        for(const FunctionParam& item: functionsParams) {
            switch(item.state) {
            case FunctionParam::V_None:
                break;       // invalid param
            case FunctionParam::V_Class: {
                const QStringList names = find_methods(testCase, item.className, "*");
                retList.append( names );
                break;
            }
            case FunctionParam::V_Func: {
                const QStringList cNames = find_methods(testCase, "*", item.funcName);
                retList.append( cNames );
                break;
            }
            case FunctionParam::V_Both: {
                const QStringList names = find_methods(testCase, item.className, item.funcName);
                retList.append( names );
                break;
            }
            }
        }

        // remove duplicates
        QSet<QString> stringSet = retList.toSet();
        retList = stringSet.toList();

        return retList;
    }

    QtTestsRegistry::QtTestsRegistry(): ObjectRegistry<QObject>(), showSummaryMode(true) {
    }

    int QtTestsRegistry::run_tests(const QStringList& arguments) {
        testutils::CmdParser params( arguments );
        return run_tests( params );
    }

    int QtTestsRegistry::run_tests(const CmdParser& arguments) {
        const std::size_t registrySize = size();
        if (registrySize < 1) {
            qWarning() << "no registered tests found";
        }

        showSummaryMode = arguments.shouldShowSummary();

        if (arguments.contains("-functions")) {
            // print functions

            // example of "-functions" output:
            //   test_function001()
            //   test_function002()

            showSummaryMode = false;
            for (std::size_t i = 0; i<registrySize; ++i) {
                QObject* testCase = get(i);
                const QStringList methods = find_methods( testCase, "*" );
                const std::string className = testCase->metaObject()->className();
                for (auto item: methods) {
                    qInfo().noquote().nospace() << className.c_str() << "::" << item << "()";
                }
            }
            return 0;
        }

        const QStringList functions = arguments.extractFunctions();
        if (functions.size()<1) {
            //run all tests
            int status = 0;
            for (std::size_t i = 0; i<registrySize; ++i) {
                QObject* testCase = get(i);
                status += execute_test_case(testCase, arguments);
            }
            return std::min(status, 127);
        }

        CmdParser commonArgs( arguments );
        commonArgs.removeAll(functions);

        const std::vector<FunctionParam> class_function = FunctionParam::splitFunctionsForQTest(functions);

        bool foundCases = false;
        int status = 0;
        for (std::size_t i = 0; i<registrySize; ++i) {
            QObject* testCase = get(i);
            const QStringList foundMethods = find_methods( testCase, class_function );
            if (foundMethods.size() < 1) {
                // test case does not have searching functions
                continue;
            }
            foundCases = true;
            CmdParser testArgs( commonArgs );
            testArgs.append( foundMethods );
            status += execute_test_case(testCase, testArgs);
        }
        if (foundCases == false) {
            showSummaryMode = false;
        }
        return std::min(status, 127);
    }

    QtTestsRegistry& QtTestsRegistry::get_tests_registry() {
        static QtTestsRegistry testsRegistry;
        return testsRegistry;
    }

    int QtTestsRegistry::execute_test_case(Type* testCase, const CmdParser& arguments) {
        const QStringList& params = arguments.list();
        return QTest::qExec(testCase, params);
    }

}
