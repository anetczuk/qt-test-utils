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

#include "QTestRegister.h"
#include <QtQuickTest/quicktest.h>
#include <iostream>

#include "ImageLoader.h"


QT_BEGIN_NAMESPACE
QTEST_ADD_GPU_BLACKLIST_SUPPORT_DEFS
QT_END_NAMESPACE

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QTEST_DISABLE_KEYPAD_NAVIGATION
    QTEST_ADD_GPU_BLACKLIST_SUPPORT
    QTEST_SET_MAIN_SOURCE_PATH

    Q_INIT_RESOURCE(testutilsres);

    qmlRegisterSingletonType<ImageLoader>("testutils", 1, 0, "ImageLoader", &ImageLoader::qmlInstance);
    qmlRegisterInterface<QmlImage>("QmlImage");
    qmlRegisterType( QUrl("qrc:/qml/CustomTestCase.qml"), "testutils", 1, 0, "CustomTestCase");

    const int statusCpp = testutils::run_registered_tests(argc, argv);

    std::cout << std::endl;

    const int statusQml = quick_test_main(argc, argv, "qmltests", QUICK_TEST_SOURCE_DIR);

    testutils::print_summary(statusCpp, statusQml);

    return std::min(statusCpp+statusQml, 127);
}
