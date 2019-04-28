#include "QTestRegister.h"


namespace testutils {

    typedef std::vector<QObject*> TestsRegistry;


    TestsRegistry& registered_tests() {
        static std::vector<QObject*> list;
        return list;
    }

    void register_test_case(QObject* testCase) {
        TestsRegistry& tests = registered_tests();
        tests.push_back(testCase);
    }

    int run_regustered_tests(int argc, char *argv[]) {
        TestsRegistry& tests = registered_tests();
        if (tests.size() < 1) {
            qWarning() << "no registered tests found";
        }
        int status = 0;
        for (auto it = tests.begin(); it != tests.end(); ++it) {
            status += QTest::qExec(*it, argc, argv);
        }
        return status;
    }
}
