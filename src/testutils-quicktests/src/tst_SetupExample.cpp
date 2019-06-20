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

#ifdef EXEC_PER_TESTCASE
    #include <QtTest>
#else
    #include <QtQuickTest/QtQuickTest>
#endif

#include "QuickTest.h"
#include "UnitTest.h"

#include <iostream>

#include <QApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QDebug>

#include "Args.h"


using namespace quicktestutils;


namespace {

    class Setup: public QObject {
        Q_OBJECT


    public slots:

        void applicationAvailable() {
            // application level initialization code
        }

        void qmlEngineAvailable(QQmlEngine* engine) {
            QQmlContext *context = engine->rootContext();
            context->setContextProperty("contextRectangleColor", "blue");
        }

        void cleanupTestCase() {
            // cleanup code
        }

    };

}


class TestSetupExample: public QObject, public QmlSingleTestUnit {
    Q_OBJECT

public:

    Setup setup;


    TestSetupExample(): QObject(), QmlSingleTestUnit(QUICK_TESTS_QML_ROOT_DIR, "/src/tst_SetupExample.qml"), setup() {
        attachSetupObject(setup);
    }


private slots:

    void initTestCase() {
        // for QtCreator to detect as test class
    }

};


QTEST_MAIN(TestSetupExample)
#include "tst_SetupExample.moc"
