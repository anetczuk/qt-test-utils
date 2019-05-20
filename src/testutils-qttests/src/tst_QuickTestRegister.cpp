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

#include "QuickTestRegister.h"
#include "QtTestRegister.h"
#include <QApplication>


class TestQuickTestRegister: public QObject {
    Q_OBJECT

private slots:

    void test_get_functions() {
        QVERIFY( qApp != nullptr );
        const QStringList args = qApp->arguments();

        const QStringList functions = quicktestutils::get_functions( args );

        const QString currMethod( Q_FUNC_INFO );
        QString methodName = currMethod.split(" ")[1];        // current test case method (remove "void " prefix)
        methodName = methodName.split("()")[0];               // return function name without parenthesis

        const int methodIndex = functions.indexOf( methodName );
        QVERIFY2( methodIndex >= 0, "current test case method should be in return list of test functions" );
    }

    void test_find_methods_class_wildcard() {
        QStringList testCases;
        testCases.push_back("TestA::aabbcc001");
        testCases.push_back("TestA::aabbcc002");
        testCases.push_back("TestB::aabbcc");
        const QStringList mNames = quicktestutils::find_methods(testCases, "T*A*");

        QCOMPARE(mNames.size(), 2);
        QCOMPARE(mNames[0], QString("TestA::aabbcc001"));
        QCOMPARE(mNames[1], QString("TestA::aabbcc002"));
    }

    void test_find_methods_class_exact() {
        QStringList testCases;
        testCases.push_back("TestA::aabbcc001");
        testCases.push_back("TestA::aabbcc002");
        testCases.push_back("TestB::aabbcc");
        const QStringList mNames = quicktestutils::find_methods(testCases, "TestA");

        QCOMPARE(mNames.size(), 0);
    }

    void test_find_methods_class_partial() {
        QStringList testCases;
        testCases.push_back("TestA::aabbcc001");
        testCases.push_back("TestA::aabbcc002");
        testCases.push_back("TestB::aabbcc");
        const QStringList mNames = quicktestutils::find_methods(testCases, "Test");

        QCOMPARE(mNames.size(), 0);
    }

    void test_find_methods_function_partial() {
        QStringList testCases;
        testCases.push_back("TestA::aabbcc001");
        testCases.push_back("TestA::aabbcc002");
        testCases.push_back("TestB::aabbcc");
        const QStringList mNames = quicktestutils::find_methods(testCases, "aabb");

        QCOMPARE(mNames.size(), 0);
    }

};


QTEST_MAIN(TestQuickTestRegister)
//QTEST_MAIN(TestTestsRegistry)
#include "tst_QuickTestRegister.moc"
