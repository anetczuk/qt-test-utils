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
#include <iostream>

#include "FunctionParam.h"


namespace testutils {

    typedef std::vector<QObject*> TestCasesList;


    static const std::set<std::string> NO_PARAMS_ARGS = {"-txt", "-cvs", "-xunitxml", "-lightxml", "-teamcity", "-silent",
                                                         "-v1", "-v2", "-vs", "-functions", "-datatags",
                                                         "-nocrashhandler", "-callgrind",
                                                         "-perf", "-perfcounterlist", "-tickcounter", "-eventcounter", "-vb", "-help"};

    static const std::set<std::string> ONE_PARAM_ARGS = {"-o", "-eventdelay", "-keydelay", "-mousedelay", "-maxwarnings",
                                                         "-perfcounter", "-minimumvalue", "-minimumtotal", "-iterations", "-median"};

    static const std::set<std::string> NO_SUMMARY_OPTIONS = {"-csv", "-xml", "-xunitxml", "-lightxml", "-teamcity"};


    TestsRegistry& get_tests_registry() {
        static TestsRegistry testsRegistry;
        return testsRegistry;
    }

    void register_test_case(QObject* testCase) {
        TestsRegistry& testsRegistry = get_tests_registry();
        testsRegistry.push_back(testCase);
    }

    int run_registered_tests(int argc, char *argv[]) {
        TestsRegistry& testsRegistry = get_tests_registry();
        QStringList arguments = convertArgs(argc, argv);
        const int numFailedTests = testsRegistry.run_tests(arguments);
        if (testsRegistry.should_show_summary() == false) {
            return numFailedTests;
        }
        std::cout << std::endl;
        std::cout << "********* Summary *********" << std::endl;
        std::cout << "      tests failures: " << numFailedTests << std::endl;
        std::cout << "********* Finished summary *********" << std::endl;
        return numFailedTests;
    }

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

    QStringList extract_functions_from_arguments(const QStringList& arguments) {
        QStringList functions;

        const int aSize = arguments.size();
        for(int i=1; i<aSize; ++i) {
            const QString& arg = arguments[i];
            const std::string& stdArg = arg.toStdString();
            auto pos = NO_PARAMS_ARGS.find( stdArg );
            if (pos != NO_PARAMS_ARGS.end()) {
                /// known argument that does not have parameters
                continue;
            }
            pos = ONE_PARAM_ARGS.find( stdArg );
            if (pos != ONE_PARAM_ARGS.end()) {
                /// known argument that does take one parameter
                ++i;
                continue;
            }
            if (stdArg.length() < 1) {
                continue;
            }
            if (stdArg[0] == '-') {
                /// unknown argument -- assume no params arg
                continue;
            }

            /// unknown string -- assume function name
            functions.append( stdArg.c_str() );
        }

        return functions;
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
                const QStringList fNames = find_methods(testCase, "*", item.funcName);
                retList.append( fNames );
                const QStringList cNames = find_methods(testCase, item.funcName, "*");
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

    TestsRegistry::TestsRegistry(): casesList(), showSummaryMode(true) {
    }

    bool showSummary(const QStringList& arguments) {
        for( const auto& item: arguments) {
            const auto pos_iter = NO_SUMMARY_OPTIONS.find( item.toStdString() );
            if (pos_iter != NO_SUMMARY_OPTIONS.end()) {
                return false;
            }
        }
        return true;
    }

    int TestsRegistry::run_tests(const QStringList& arguments) {
        const std::size_t registrySize = size();
        if (registrySize < 1) {
            qWarning() << "no registered tests found";
        }

        showSummaryMode = showSummary(arguments);

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

        const QStringList functions = extract_functions_from_arguments(arguments);
        if (functions.size()<1) {
            //run all tests
            int status = 0;
            for (std::size_t i = 0; i<registrySize; ++i) {
                QObject* testCase = get(i);
                status += execute_test_case(testCase, arguments);
            }
            return std::min(status, 127);
        }

        QStringList commonArgs = arguments;
        for(const QString& item: functions) {
            commonArgs.removeAll(item);
        }

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
            QStringList testArgs = commonArgs;
            testArgs.append( foundMethods );
            status += execute_test_case(testCase, testArgs);
        }
        if (foundCases == false) {
            showSummaryMode = false;
        }
        return std::min(status, 127);
    }

    int TestsRegistry::execute_test_case(QObject* testCase, const QStringList& arguments) {
        return QTest::qExec(testCase, arguments);
    }

}
