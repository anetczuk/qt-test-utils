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

#ifndef SRC_ALIB_SRC_QTESTREGISTER_H_
#define SRC_ALIB_SRC_QTESTREGISTER_H_

#include <QtTest/QtTest>
#include <QSharedPointer>


#ifndef EXEC_PER_TESTCASE

    // new behaviour -- run all tests in one executable

    #define QTEST_REGISTER( test_class )    static testutils::RegisterTestCaseInvocation<test_class> TEST_CASE__FILE__;


    #define QTEST_RUN_TESTS()                                                                   \
                                int main(int argc, char *argv[]) {                              \
                                    return testutils::run_registered_tests(argc, argv);         \
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

    int run_registered_tests(int argc, char *argv[]);


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
