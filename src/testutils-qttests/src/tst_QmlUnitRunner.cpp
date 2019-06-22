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

#include <QtTest>
#include "UnitTest.h"

#include "QmlUnitRunner.h"
#include <QApplication>


using namespace quicktestutils;


class QmlUnitRunnerMock: public QmlUnitRunner {
public:

    QStringList allTestCases;
    int execQmlCounter;
    QString execQmlArgs;


    QmlUnitRunnerMock(): QmlUnitRunner(), allTestCases(), execQmlCounter(0), execQmlArgs("") {
    }

    virtual ~QmlUnitRunnerMock() {
    }


protected:

    virtual int executeQml(const testutils::CmdParser& arguments, const char* /*sourceDir*/, QObject* /*setup*/) override {
        execQmlCounter += 1;
        execQmlArgs = arguments.toQString();
        return 0;
    }

    virtual QStringList getFunctionsList(const testutils::CmdParser& /*arguments*/) const override {
        return allTestCases;
    }

};


class TestQmlUnitRunner: public QObject {
    Q_OBJECT

private slots:

    void test_get_functions() {
        QVERIFY( qApp != nullptr );
        const QStringList args = qApp->arguments();

        const QStringList functions = get_functions( args );
        const QString currMethod( Q_FUNC_INFO );

        QString methodName = currMethod.split(" ").back();    // current test case method (remove "void " prefix)
        methodName = methodName.split("::").back();           // exclude class name
        methodName = methodName.split("()").front();          // return function name without parenthesis

        const QString className = metaObject()->className();
        methodName = "(" + className + "::)*" + methodName;                  // optionally consider class name
        const QRegularExpression re( methodName );

        const int methodIndex = functions.indexOf( re );
        QVERIFY2( methodIndex >= 0, "current test case method should be in return list of test functions" );
    }

//    void test_get_functions_input() {
//        QVERIFY( qApp != nullptr );
//        QStringList args = qApp->arguments();
//        args.append("-input");
//        args.append("test_unit");

//        const QStringList functions = get_functions( args );

//        const QString currMethod( Q_FUNC_INFO );
//        QString methodName = currMethod.split(" ")[1];        // current test case method (remove "void " prefix)
//        methodName = methodName.split("()")[0];               // return function name without parenthesis

//        const int methodIndex = functions.indexOf( methodName );
//        QVERIFY2( methodIndex >= 0, "current test case method should be in return list of test functions" );
//    }

    void test_find_methods_class_wildcard() {
        QStringList testCases;
        testCases.push_back("TestA::aabbcc001");
        testCases.push_back("TestA::aabbcc002");
        testCases.push_back("TestB::aabbcc");
        const QStringList mNames = find_methods(testCases, "T*A*");

        QCOMPARE(mNames.size(), 2);
        QCOMPARE(mNames[0], QString("TestA::aabbcc001"));
        QCOMPARE(mNames[1], QString("TestA::aabbcc002"));
    }

    void test_find_methods_class_exact() {
        QStringList testCases;
        testCases.push_back("TestA::aabbcc001");
        testCases.push_back("TestA::aabbcc002");
        testCases.push_back("TestB::aabbcc");
        const QStringList mNames = find_methods(testCases, "TestA");

        QCOMPARE(mNames.size(), 2);
        QCOMPARE(mNames[0], QString("TestA::aabbcc001"));
        QCOMPARE(mNames[1], QString("TestA::aabbcc002"));
    }

    void test_find_methods_class_partial() {
        QStringList testCases;
        testCases.push_back("TestA::aabbcc001");
        testCases.push_back("TestA::aabbcc002");
        testCases.push_back("TestB::aabbcc");
        const QStringList mNames = find_methods(testCases, "Test");

        QCOMPARE(mNames.size(), 0);
    }

    void test_find_methods_function_partial() {
        QStringList testCases;
        testCases.push_back("TestA::aabbcc001");
        testCases.push_back("TestA::aabbcc002");
        testCases.push_back("TestB::aabbcc");
        const QStringList mNames = find_methods(testCases, "aabb");

        QCOMPARE(mNames.size(), 0);
    }


    void test_run_normal() {
        QmlUnitRunnerMock runner;

        testutils::CmdParser arguments;
        arguments.push_back("program");

        const int ret = runner.run(arguments);
        QCOMPARE(ret, 0);
        QCOMPARE(runner.execQmlCounter, 1);
        QCOMPARE(runner.execQmlArgs, "program");
    }

    void test_run_listFunctions() {
        QmlUnitRunnerMock runner;

        testutils::CmdParser arguments;
        arguments.push_back("program");
        arguments.push_back("-functions");

        const int ret = runner.run(arguments);
        QCOMPARE(ret, 0);
        QCOMPARE(runner.execQmlCounter, 1);
        QCOMPARE(runner.execQmlArgs, "program -functions");
    }

    void test_run_functions() {
        QmlUnitRunnerMock runner;
        runner.allTestCases.push_back("Class::test_case_001");
        runner.allTestCases.push_back("Class::test_case_002");
        runner.allTestCases.push_back("Class::test_case_003");

        testutils::CmdParser arguments;
        arguments.push_back("program");
        arguments.push_back("Class::test_case_001");
        arguments.push_back("Class::test_case_002");

        const int ret = runner.run(arguments);
        QCOMPARE(ret, 0);
        QCOMPARE(runner.execQmlCounter, 1);
        QCOMPARE(runner.execQmlArgs, "program Class::test_case_001 Class::test_case_002");
    }

    void test_run_functions_notFound() {
        QmlUnitRunnerMock runner;
        runner.allTestCases.push_back("Class::test_case_001");
        runner.allTestCases.push_back("Class::test_case_002");
        runner.allTestCases.push_back("Class::test_case_003");

        testutils::CmdParser arguments;
        arguments.push_back("program");
        arguments.push_back("Class::test_case_missing");

        const int ret = runner.run(arguments);
        QCOMPARE(ret, 0);
        QCOMPARE(runner.execQmlCounter, 0);
    }

};


QTEST_MAIN(TestQmlUnitRunner)
#include "tst_QmlUnitRunner.moc"
