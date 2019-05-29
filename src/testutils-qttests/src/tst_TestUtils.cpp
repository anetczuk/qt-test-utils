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

#include "QtTest.h"

#include "TestUtils.h"


class TestTestUtils: public QObject {
    Q_OBJECT

private slots:

    void test_removeItems() {
        QStringList arguments = {"a1", "a2", "a3", "a4", "a2"};

        const int ret = arguments.removeAll( "a2" );

        QCOMPARE(ret, 2);
        QCOMPARE(arguments.size(), 3);
        QCOMPARE(arguments[0], QString("a1"));
        QCOMPARE(arguments[1], QString("a3"));
        QCOMPARE(arguments[2], QString("a4"));
    }

//    void test_extract_functions_from_arguments() {
//        QStringList arguments = {"bin_path", "-v1", "test_aaa001", "-median", "30", "-xxx", "test_aaa002"};
//        const QStringList functions = testutils::extract_functions_from_arguments(arguments);

//        qDebug() << "functions:" << functions;
//        QCOMPARE(functions.size(), 2);
//        QCOMPARE(functions[0], QString("test_aaa001"));
//        QCOMPARE(functions[1], QString("test_aaa002"));
//    }

//    void test_extract_functions_from_arguments_input() {
//        QStringList arguments = {"bin_path", "test_aaa001", "-input", "test_unit"};
//        const QStringList functions = testutils::extract_functions_from_arguments(arguments);

//        QCOMPARE(functions.size(), 1);
//        QCOMPARE(functions[0], QString("test_aaa001"));
//    }

};


QTEST_MAIN(TestTestUtils)
#include "tst_TestUtils.moc"
