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

#ifndef SRC_ALIB_SRC_QTTEST_H_
#define SRC_ALIB_SRC_QTTEST_H_

#include <QtTest/QtTest>

#include "QtTestRegister.h"
#include "ConcatMacro.h"


#ifdef EXEC_PER_TESTCASE

    // old behaviour -- create executable per test case

    #define QTEST_REGISTER( test_class )    QTEST_MAIN( test_class )

    #define QTEST_RUN_TESTS()               // do nothing

#else

    // new behaviour -- run all tests in one executable


    #define QTEST_REGISTER( test_class )    static testutils::RegisterQtTest<test_class> PPCAT(TEST_CASE, __LINE__);


    #define QTEST_RUN_TESTS()                                                                   \
                                QT_BEGIN_NAMESPACE                                              \
                                QTEST_ADD_GPU_BLACKLIST_SUPPORT_DEFS                            \
                                QT_END_NAMESPACE                                                \
                                int main(int argc, char *argv[]) {                              \
                                    QApplication app(argc, argv);                               \
                                    QTEST_DISABLE_KEYPAD_NAVIGATION                             \
                                    QTEST_ADD_GPU_BLACKLIST_SUPPORT                             \
                                    QTEST_SET_MAIN_SOURCE_PATH                                  \
                                    return testutils::run_registered_tests(argc, argv);         \
                                }

    // redefine standard macro to keep compatibile with old code
    #undef QTEST_MAIN
    #define QTEST_MAIN( test_class )        QTEST_REGISTER( test_class )

#endif


namespace testutils {

    int run_registered_tests(int argc, char *argv[]);

}


#endif /* SRC_ALIB_SRC_QTTEST_H_ */
