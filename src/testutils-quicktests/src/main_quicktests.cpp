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

#include <QtQuickTest/QtQuickTest>          // for QtCreator to detect test cases
#include "QtTestMainStub.h"
#include "QuickTest.h"

#include "QuickTestRunner.h"


QUICK_TEST_MAIN(quicktests);                // redefined empty macro to inform QtCrator to detect test cases


//QT_BEGIN_NAMESPACE
//QTEST_ADD_GPU_BLACKLIST_SUPPORT_DEFS
//QT_END_NAMESPACE

int main(int argc, char *argv[]) {
//    QTEST_DISABLE_KEYPAD_NAVIGATION
//    QTEST_ADD_GPU_BLACKLIST_SUPPORT
//    QTEST_SET_MAIN_SOURCE_PATH

    registerTestUtilsQmlResources();

    std::string sourceDir( QUICK_TEST_SOURCE_DIR );
    sourceDir.append("/qml");

    return quicktestutils::run_tests( argc, argv, sourceDir.c_str() );
}
