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


class DummyTestCase: public QObject {
    Q_OBJECT


public slots:

    void slot_public() {}


private slots:

    void function001() {}

    void function002() {}

};


class TestQTestRegister: public QObject {
    Q_OBJECT

private slots:

    void test_find_methods_exact() {
        DummyTestCase object;
        const QStringList mNames = testutils::find_methods(&object, "function001");

        QCOMPARE(mNames.size(), 1);
        QCOMPARE(mNames[0], QString("function001"));
    }

    void test_find_methods_wildcard() {
        DummyTestCase object;
        const QStringList mNames = testutils::find_methods(&object, "function*");

        QCOMPARE(mNames.size(), 2);
        QCOMPARE(mNames[0], QString("function001"));
        QCOMPARE(mNames[1], QString("function002"));
    }

    void test_find_methods_signal() {
        DummyTestCase object;
        const QStringList mNames = testutils::find_methods(&object, "objectNameChanged");

        QCOMPARE(mNames.size(), 0);
    }

    void test_find_methods_slot_public() {
        DummyTestCase object;
        const QStringList mNames = testutils::find_methods(&object, "slot_public");

        QCOMPARE(mNames.size(), 0);
    }

    void test_find_methods_destructor() {
        DummyTestCase object;
        const QStringList mNames = testutils::find_methods(&object, "objectNameChanged");

        QCOMPARE(mNames.size(), 0);
    }

    void test_find_methods_wildcard_all() {
        DummyTestCase object;
        const QStringList mNames = testutils::find_methods(&object, "*");

        QCOMPARE(mNames.size(), 2);
        QCOMPARE(mNames[0], QString("function001"));
        QCOMPARE(mNames[1], QString("function002"));
    }

    void test_find_methods_list() {
        const QStringList functionsToFind = {"function001", "function002", "function333"};
        DummyTestCase object;
        const QStringList mNames = testutils::find_methods(&object, functionsToFind);

        QCOMPARE(mNames.size(), 2);
        QCOMPARE(mNames[0], QString("function001"));
        QCOMPARE(mNames[1], QString("function002"));
    }

    void test_extract_functions_from_arguments() {
        QStringList arguments = {"bin_path", "-v1", "test_aaa001", "-median", "30", "-xxx", "test_aaa002"};
        const QStringList functions = testutils::extract_functions_from_arguments(arguments);

        QCOMPARE(functions.size(), 2);
        QCOMPARE(functions[0], QString("test_aaa001"));
        QCOMPARE(functions[1], QString("test_aaa002"));
    }

};


// =======================================================


class TestsRegistryMock: public testutils::TestsRegistry {
public:

    TestsRegistryMock(): TestsRegistry() {
    }


protected:

    virtual int execute_test_case(QObject* /*testCase*/, const QStringList& /*arguments*/) override {
        //qDebug() << "arguments:" << arguments;
        return 1;
    }

};


class TestTestsRegistry: public QObject {
    Q_OBJECT

private slots:

    void test_run_tests_single() {
        TestsRegistryMock registry;
        DummyTestCase object;
        registry.push_back( &object );
        QStringList arguments = {"binary_path"};

        const int ret = registry.run_tests(arguments);
        QCOMPARE(ret, 1);
    }

    void test_run_tests_multi() {
        TestsRegistryMock registry;
        DummyTestCase object001;
        registry.push_back( &object001 );
        DummyTestCase object002;
        registry.push_back( &object002 );
        QStringList arguments = {"binary_path"};

        const int ret = registry.run_tests(arguments);
        QCOMPARE(ret, 2);
    }

    void test_run_tests_filtered() {
        TestsRegistryMock registry;
        QObject object001;
        registry.push_back( &object001 );
        DummyTestCase object002;
        registry.push_back( &object002 );
        QStringList arguments = {"binary_path", "function001"};

        const int ret = registry.run_tests(arguments);
        QCOMPARE(ret, 1);
    }

    void test_run_tests_filtered_with_class() {
        TestsRegistryMock registry;
        QObject object001;
        registry.push_back( &object001 );
        DummyTestCase object002;
        registry.push_back( &object002 );
        QStringList arguments = {"binary_path", "Dummy*::function001"};

        const int ret = registry.run_tests(arguments);
        QCOMPARE(ret, 1);
    }

    void test_run_tests_list_functions() {
        TestsRegistryMock registry;
        QObject object001;
        registry.push_back( &object001 );
        DummyTestCase object002;
        registry.push_back( &object002 );
        QStringList arguments = {"binary_path", "-functions"};

        const int ret = registry.run_tests(arguments);
        QCOMPARE(ret, 0);
    }
};


QTEST_MAIN(TestQTestRegister)
QTEST_MAIN(TestTestsRegistry)
#include "tst_QTestRegister.moc"
