#ifndef SRC_ALIB_SRC_QTESTREGISTER_H_
#define SRC_ALIB_SRC_QTESTREGISTER_H_

#include <QtTest/QtTest>
#include <QSharedPointer>



#define QTEST_REGISTER( test_class )    static testutils::RegisterInvocation<test_class> TEST_CASE__FILE__;


#define QTEST_RUN_TESTS()                                                                   \
                            int main(int argc, char *argv[]) {                              \
                                return testutils::run_regustered_tests(argc, argv);         \
                            }


namespace testutils {

    void register_test_case(QObject* testCase);

    int run_regustered_tests(int argc, char *argv[]);

    //std::vector<QObject*> &registered_tests();


    template <class TC>
    class RegisterInvocation {

        QSharedPointer<TC> testCase;

    public:


        RegisterInvocation(): testCase(nullptr) {
            testCase.reset( new TC() );
            register_test_case( testCase.data() );
        }

    };

}


#endif /* SRC_ALIB_SRC_QTESTREGISTER_H_ */
