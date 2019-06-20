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

#ifndef SRC_ALIB_SRC_QUICKTEST_H_
#define SRC_ALIB_SRC_QUICKTEST_H_

#include "QuickTestRegister.h"
#include "QuickTestRunner.h"
#include "ConcatMacro.h"

#include "ImageLoader.h"


#define QUICK_TEST_REGISTER( test_class )    static quicktestutils::RegisterTestUnit<test_class> PPCAT(TEST_CASE, __LINE__);


#ifdef QUICK_TESTS_QML_ROOT_DIR
    #define QUICK_TESTS_QML_SUBDIR ""
#else
    #define QUICK_TESTS_QML_ROOT_DIR QUICK_TEST_SOURCE_DIR
    #define QUICK_TESTS_QML_SUBDIR "/qml"
#endif


inline void registerTestUtilsQmlResources() {
    Q_INIT_RESOURCE(testutilsres);

    qmlRegisterSingletonType<ImageLoader>("testutils", 1, 0, "ImageLoader", &ImageLoader::qmlInstance);
    qmlRegisterInterface<QmlImage>("QmlImage");
}


#undef QUICK_TEST_MAIN
#define QUICK_TEST_MAIN( tests_name )                                                                                                   \
                            int main(int argc, char *argv[]) {                                                                          \
                                registerTestUtilsQmlResources();                                                                        \
                                                                                                                                        \
                                std::string sourceDir( QUICK_TESTS_QML_ROOT_DIR );                                                      \
                                sourceDir.append( QUICK_TESTS_QML_SUBDIR );                                                             \
                                                                                                                                        \
                                return quicktestutils::run_tests( argc, argv, sourceDir.c_str() );                                      \
                            }


#ifdef EXEC_PER_TESTCASE

    #undef QTEST_MAIN
    #define QTEST_MAIN( test_class )                                                                                                    \
                            int main(int argc, char *argv[]) {                                                                          \
                                registerTestUtilsQmlResources();                                                                        \
                                QApplication app(argc, argv);                                                                           \
                                test_class test;                                                                                        \
                                const QStringList& appArgs = app.arguments();                                                           \
                                return test.run( appArgs );                                                                             \
                            }

#else

    // redefine standard macro to keep compatibile with old code
    #undef QTEST_MAIN
    #define QTEST_MAIN( test_class )        QUICK_TEST_REGISTER( test_class )

#endif


#endif /* SRC_ALIB_SRC_QUICKTEST_H_ */
