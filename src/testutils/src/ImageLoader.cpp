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

#include "ImageLoader.h"

#include <QPainter>


static QImage::Format format = QImage::Format_RGB32;


QImage generateChessboard(const int width, const int height) {
    QImage chess(width, height, format);
    chess.fill( "black" );
    QPainter painter(&chess);
    static const int minGrid = 10;
    const int imgMinGrid = std::min( width / 3, height / 3 );
    const int gridSize = (imgMinGrid < minGrid) ? 2 : minGrid;
    const int wSteps = width / gridSize;
    const int hSteps = height / gridSize;
    for (int wo=0; wo<=wSteps; wo++) {
        const int wrest = wo % 2;
        for (int ho=0; ho<=hSteps; ho++) {
            if (ho % 2 == wrest) {
                const int currWidth = wo*gridSize;
                const int currHeight = ho*gridSize;
                painter.fillRect(currWidth, currHeight, gridSize, gridSize, "white");
            } else {
                //
            }
        }
    }
    return chess;
}

QImage ImageLoader::makeDiff(const QImage& imgA, const QImage& imgB) {
    if(imgA.isNull() && imgB.isNull()) {
        QImage emptyDiff(2, 1, format);
        emptyDiff.setPixelColor(0, 0, "black");
        emptyDiff.setPixelColor(1, 0, "black");
        return emptyDiff;
    }
    const int widthA = imgA.width();
    const int heightA = imgA.height();

    const int widthB = imgB.width();
    const int heightB = imgB.height();

    const int widthMax = std::max(widthA, widthB);
    const int heightMax = std::max(heightA, heightB);

    QImage diff(widthMax, heightMax, format);
    diff.fill( Qt::transparent );
    {
        QPainter painter(&diff);
        painter.setCompositionMode( QPainter::CompositionMode_Difference );
        painter.drawImage(0, 0, imgA);
        painter.drawImage(0, 0, imgB);
    }

    QImage threshold(widthMax, heightMax, format);
    threshold.fill( Qt::transparent );
    for (int wo=0; wo<widthMax; wo++) {
        for (int ho=0; ho<heightMax; ho++) {
            QRgb color = diff.pixel(wo, ho);
            if ( qRed(color) > 0 || qGreen(color) > 0 || qBlue(color) > 0 ) {
                threshold.setPixelColor(wo, ho, "white");
            } else {
                threshold.setPixelColor(wo, ho, "black");
            }
        }
    }

    QImage join(widthMax*2, heightMax*2, format);
    join.fill( Qt::transparent );
    {
        QPainter painter(&join);
        if (imgA.isNull()) {
            const QImage chess = generateChessboard(widthMax, heightMax);
            painter.drawImage(0, 0, chess);
        } else {
            painter.drawImage(0, 0, imgA);
        }
        if (imgB.isNull()) {
            const QImage chess = generateChessboard(widthMax, heightMax);
            painter.drawImage(widthMax, 0, chess);
        } else {
            painter.drawImage(widthMax, 0, imgB);
        }
        painter.drawImage(0, heightMax, threshold);
        painter.drawImage(widthMax, heightMax, diff);
    }

    return join;
}

QmlImage* ImageLoader::makeDiff(const QmlImage& imgA, const QmlImage& imgB) {
    const QImage& imageA = imgA.qimage();
    const QImage& imageB = imgB.qimage();
    QImage diff = makeDiff(imageA, imageB);
    return new QmlImage( diff, this );
}

QmlImage* ImageLoader::makeDiff(QmlImage& imgA, QmlImage& imgB) {
    QImage& imageA = imgA.qimage();
    QImage& imageB = imgB.qimage();
    QImage diff = makeDiff(imageA, imageB);
    return new QmlImage( diff, this );
}

QmlImage* ImageLoader::makeDiff(const QmlImage* imgA, const QmlImage* imgB) {
    if (imgA == nullptr || imgB == nullptr)
        return new QmlImage(this);
    return makeDiff( *imgA, *imgB );
}

QmlImage* ImageLoader::makeDiff(QmlImage* imgA, QmlImage* imgB) {
    if (imgA == nullptr || imgB == nullptr)
        return new QmlImage(this);
    return makeDiff( *imgA, *imgB );
}

QObject *ImageLoader::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    static ImageLoader* instance = nullptr;
    if (instance == nullptr) {
        //qDebug() << "creating ImageLoader singleton";
        instance = new ImageLoader();
    }
    return instance;
}
