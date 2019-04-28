#ifndef SRC_ALIB_SRC_QTESTREGISTER_H_
#define SRC_ALIB_SRC_QTESTREGISTER_H_

#include <QtTest/QtTest>
#include <QSharedPointer>


#ifndef EXEC_PER_TESTCASE

    // new behaviour -- run all tests in one executable

    #define QTEST_REGISTER( test_class )    static testutils::RegisterTestCaseInvocation<test_class> TEST_CASE__FILE__;


    #define QTEST_RUN_TESTS()                                                                   \
                                int main(int argc, char *argv[]) {                              \
                                    return testutils::run_regustered_tests(argc, argv);         \
                                }

    // redefine standard macro to keep compatibile with old code
    #undef QTEST_MAIN
    #define QTEST_MAIN( test_class )        QTEST_REGISTER( test_class )

#else

    // old behaviour -- create executable per test case

    #define QTEST_REGISTER( test_class )    QTEST_MAIN( test_class )

    #define QTEST_RUN_TESTS()               // do nothing

#endif


namespace testutils {

    void register_test_case(QObject* testCase);

    int run_regustered_tests(int argc, char *argv[]);


    template <class TC>
    class RegisterTestCaseInvocation {
        QSharedPointer<TC> testCase;

    public:

        RegisterTestCaseInvocation(): testCase( new TC() ) {
            register_test_case( testCase.data() );
        }

    };

}


#endif /* SRC_ALIB_SRC_QTESTREGISTER_H_ */
