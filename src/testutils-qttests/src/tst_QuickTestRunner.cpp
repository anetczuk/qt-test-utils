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

#include "QuickTestRunner.h"
#include "QuickTestRegister.h"


using namespace quicktestutils;


class QuickTestsRunnerMock: public QuickTestsRunner {
public:

    QuickTestsRegistry tests;

    int spawnQmlCounter;
    int execQmlCounter;
    int spawnRegisteredCounter;
    int execRegisteredCounter;

    int retStatus;


    QuickTestsRunnerMock(): QuickTestsRunner(), tests(), spawnQmlCounter(0), execQmlCounter(0), spawnRegisteredCounter(0), execRegisteredCounter(0), retStatus(0) {
    }

    virtual ~QuickTestsRunnerMock() override {
    }


protected:

    virtual int spawn_qml_tests(const testutils::CmdParser& /*arguments*/) override {
        spawnQmlCounter += 1;
        return 0;
    }

    virtual int execute_qml_tests(const testutils::CmdParser& /*arguments*/, const char */*sourceDir*/) override {
        execQmlCounter += 1;
        return retStatus;
    }

    virtual int spawn_registered_tests(const testutils::CmdParser& /*arguments*/, const std::string& /*unit*/) override {
        spawnRegisteredCounter += 1;
        return 0;
    }

    virtual int execute_registered_tests(const testutils::CmdParser& arguments) override {
        execRegisteredCounter += 1;
        return QuickTestsRunner::execute_registered_tests(arguments);
    }

    virtual QuickTestsRegistry& getTestsRegistry() override {
        return tests;
    }

};


class QmlSingleTestUnitMock: public QmlSingleTestUnit {
public:

    int returnStatus;


    QmlSingleTestUnitMock(const std::string& unitPath): QmlSingleTestUnit( unitPath ), returnStatus(0) {
    }

    QmlSingleTestUnitMock(const std::string& unitPath, const int retStatus): QmlSingleTestUnit( unitPath ), returnStatus(retStatus) {
    }

    virtual int run(const QStringList& /*arguments*/) override {
        return returnStatus;
    }

};


class TestQuickTestsRunner: public QObject {
    Q_OBJECT

private slots:

    void test_run_spawn() {
        QmlSingleTestUnitMock unitA("mock_unit_001", 3);
        QmlSingleTestUnitMock unitB("mock_unit_002", 33);
        QuickTestsRunnerMock runner;
        runner.tests.push_back( &unitA );
        runner.tests.push_back( &unitB );

        QStringList args;
        args.push_back("program");
        args.push_back("-xml");

        const int ret = runner.run_tests(args);

        QCOMPARE( ret, 0 );
        QCOMPARE( runner.spawnQmlCounter, 1 );
        QCOMPARE( runner.execQmlCounter, 0 );
        QCOMPARE( runner.spawnRegisteredCounter, 2 );
        QCOMPARE( runner.execRegisteredCounter, 0 );
    }

    void test_run_qml() {
        QuickTestsRunnerMock runner;
        runner.retStatus = 1000;

        testutils::CmdParser arguments;
        arguments.push_back("program");
        arguments.appendQmlRunner();

        const int ret = runner.run_tests(arguments);

        QCOMPARE( ret, 127 );
        QCOMPARE( runner.spawnQmlCounter, 0 );
        QCOMPARE( runner.execQmlCounter, 1 );
        QCOMPARE( runner.spawnRegisteredCounter, 0 );
        QCOMPARE( runner.execRegisteredCounter, 0 );
    }

    void test_run_registered() {
        QmlSingleTestUnitMock unitA("mock_unit_001", 3);
        QmlSingleTestUnitMock unitB("mock_unit_002", 33);
        QuickTestsRunnerMock runner;
        runner.tests.push_back( &unitA );
        runner.tests.push_back( &unitB );
        runner.retStatus = 1000;

        testutils::CmdParser arguments;
        arguments.push_back("program");
        arguments.appendCppUnit("mock_unit_002");

        const int ret = runner.run_tests(arguments);

        QCOMPARE( ret, 33 );
        QCOMPARE( runner.spawnQmlCounter, 0 );
        QCOMPARE( runner.execQmlCounter, 0 );
        QCOMPARE( runner.spawnRegisteredCounter, 0 );
        QCOMPARE( runner.execRegisteredCounter, 1 );
    }

};


QTEST_MAIN(TestQuickTestsRunner)
#include "tst_QuickTestRunner.moc"
