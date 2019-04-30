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

#include "QTestRegister.h"


namespace testutils {

    typedef std::vector<QObject*> TestCasesList;


    static const std::set<std::string> NO_PARAMS_ARGS = {"-txt", "-cvs", "-xunitxml", "-lightxml", "-teamcity", "-silent",
                                                         "-v1", "-v2", "-vs", "-functions", "-datatags",
                                                         "-nocrashhandler", "-callgrind",
                                                         "-perf", "-perfcounterlist", "-tickcounter", "-eventcounter", "-vb", "-help"};

    static const std::set<std::string> ONE_PARAM_ARGS = {"-o", "-eventdelay", "-keydelay", "-mousedelay", "-maxwarnings",
                                                         "-perfcounter", "-minimumvalue", "-minimumtotal", "-iterations", "-median"};


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
        QStringList arguments;
        for(int i=0; i<argc; ++i) {
            arguments.append( argv[i] );
        }
        return testsRegistry.run_tests(arguments);
    }

    QStringList find_methods(const QObject* testCase, const QString& function) {
        QString pattern = function;
        pattern.replace("*", ".*");
        QRegularExpression re( pattern );

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
            QRegularExpressionMatch match = re.match( mName );
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

    QStringList extract_functions(const QStringList& arguments) {
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

    bool should_show_summary() {
        TestsRegistry& testsRegistry = get_tests_registry();
        return (testsRegistry.is_functions_list_mode() == false);
    }


    // ======================================================


    TestsRegistry::TestsRegistry(): casesList(), listFunctionsMode(false) {
    }

    int TestsRegistry::run_tests(const QStringList& arguments) {
        const std::size_t registrySize = size();
        if (registrySize < 1) {
            qWarning() << "no registered tests found";
        }

        if (arguments.contains("-functions")) {
            // print functions
            listFunctionsMode = true;
            for (std::size_t i = 0; i<registrySize; ++i) {
                QObject* testCase = get(i);
                const QStringList methods = find_methods( testCase, "*" );
                for (auto item: methods) {
                    qInfo().noquote() << item;
                }
            }
            return 0;
        }

        const QStringList functions = extract_functions(arguments);
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
        for(auto item: functions) {
            commonArgs.removeAll(item);
        }

        int status = 0;
        for (std::size_t i = 0; i<registrySize; ++i) {
            QObject* testCase = get(i);
            const QStringList foundMethods = find_methods( testCase, functions );
            if (foundMethods.size() < 1) {
                // test case does not have searching functions
                continue;
            }
            QStringList testArgs = commonArgs;
            testArgs.append( foundMethods );
            status += execute_test_case(testCase, testArgs);
        }
        return std::min(status, 127);
    }

    int TestsRegistry::execute_test_case(QObject* testCase, const QStringList& arguments) {
        return QTest::qExec(testCase, arguments);
    }

}
