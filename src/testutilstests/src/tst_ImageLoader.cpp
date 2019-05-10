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

#include "ImageLoader.h"


class TestImageLoader: public QObject {
    Q_OBJECT

private slots:

    void test_makeDiff_null() {
        const QmlImage image("data/blue.png");
        QVERIFY( image.empty() == false );

        ImageLoader loader;

        {
            const QmlImage* result = loader.makeDiff(nullptr, &image);
            QVERIFY( result != nullptr );
            QVERIFY( result->empty() );
        }

        {
            const QmlImage* result = loader.makeDiff(&image, nullptr);
            QVERIFY( result != nullptr );
            QVERIFY( result->empty() );
        }
    }

    void test_makeDiff_same() {
        QmlImage image("data/blue.png");
        QVERIFY( image.empty() == false );

        ImageLoader loader;

        const QmlImage* result = loader.makeDiff(image, image);
        QVERIFY( result != nullptr );

//        result->save("tests/blue_diff.png");

        const QmlImage data("data/diff/blue_diff.png");
        QVERIFY( data.empty() == false );

        QVERIFY( result->equals( data ) );
    }

    void test_makeDiff_diff() {
        QmlImage imageA("data/red.png");
        QVERIFY( !imageA.empty() );

        QmlImage imageB("data/blue.png");
        QVERIFY( !imageB.empty() );

        ImageLoader loader;

        QmlImage* result = loader.makeDiff(imageA, imageB);
        QVERIFY( result != nullptr );

//        result->save("tests/red_blue_diff.png");

        QmlImage data("data/diff/red_blue_diff.png");
        QVERIFY( data.empty() == false );

        QVERIFY( result->equals( data ) );
    }

    void test_makeDiff_emptyA() {
        QmlImage imageA;                    // empty
        QVERIFY( imageA.empty() );

        QmlImage imageB("data/blue.png");
        QVERIFY( !imageB.empty() );

        ImageLoader loader;

        QmlImage* result = loader.makeDiff(imageA, imageB);
        QVERIFY( result != nullptr );

//        result->save("tests/emptyA_diff.png");

        QmlImage data("data/diff/emptyA_diff.png");
        QVERIFY( data.empty() == false );

        QVERIFY( result->equals( data ) );
    }

    void test_makeDiff_emptyB() {
        QmlImage imageA("data/blue.png");
        QVERIFY( !imageA.empty() );

        QmlImage imageB;                    // empty
        QVERIFY( imageB.empty() );

        ImageLoader loader;

        QmlImage* result = loader.makeDiff(imageA, imageB);
        QVERIFY( result != nullptr );

//        result->save("tests/emptyB_diff.png");

        QmlImage data("data/diff/emptyB_diff.png");
        QVERIFY( data.empty() == false );

        QVERIFY( result->equals( data ) );
    }

    void test_makeDiff_empty_both() {
        QmlImage imageA;                    // empty
        QVERIFY( imageA.empty() );

        QmlImage imageB;                    // empty
        QVERIFY( imageB.empty() );

        ImageLoader loader;

        QmlImage* result = loader.makeDiff(imageA, imageB);
        QVERIFY( result != nullptr );

//        result->save("tests/emptyC_diff.png");

        QmlImage data("data/diff/emptyC_diff.png");
        QVERIFY( data.empty() == false );

        QVERIFY( result->equals( data ) );
    }

};

QTEST_MAIN(TestImageLoader)
#include "tst_ImageLoader.moc"
