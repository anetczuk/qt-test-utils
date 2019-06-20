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

#include "QmlImage.h"


class TestQmlImage: public QObject {
    Q_OBJECT

private slots:

    void test_compare_same_instance() {
        const QmlImage object;

        QCOMPARE(object, object);
    }

    void test_compare_empty() {
        const QmlImage object1;
        const QmlImage object2;

        QCOMPARE(object1, object2);
    }

    void test_compare_differ() {
        QmlImage object1;
        object1.load("data/red.png");
        QVERIFY( !object1.empty() );

        QmlImage object2;
        object2.load("data/blue.png");
        QVERIFY( !object2.empty() );

        QVERIFY( !(object1 == object2) );
    }

    void test_loadImage() {
        QmlImage object;
        const bool loaded = object.load("data/blue.png");

        QVERIFY( loaded );
        QVERIFY( !object.empty() );
        QCOMPARE(object.width(), 300);
        QCOMPARE(object.height(), 300);
        QCOMPARE(object.blue(0, 0), 255);
    }

    void test_loadImage_fail() {
        QmlImage object;
        const bool loaded = object.load("not_existing_file.png");

        QVERIFY( !loaded );
        QVERIFY( object.empty() );
        QCOMPARE(object.width(), 0);
        QCOMPARE(object.height(), 0);
    }

};

QTEST_MAIN(TestQmlImage)
#include "tst_QmlImage.moc"
