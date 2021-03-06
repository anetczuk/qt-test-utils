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


static QImage::Format DIFF_IMG_FORMAT = QImage::Format_RGB32;

static const int CHESS_GRID_SIZE = 10;
static const int DIFF_IMAGES_SPACING = CHESS_GRID_SIZE * 2;


QmlImage* ImageLoader::loadImage(const QString& path) {
    QmlImage* obj = new QmlImage(this);
    obj->load(path);
    return obj;
}

QmlImage* ImageLoader::saveImageOfItem(QQuickItem *item, const QString& path) {
    QmlImage* img = grabImage(item);
    if (img == nullptr)
        return nullptr;
    img->save( path );
    return img;
}

QmlImage* ImageLoader::grabImage(QQuickItem *item) {
    if (item == nullptr) {
        qDebug() << "unable to grab image from null item";
        return nullptr;
    }
    if (item->window() == nullptr) {
        qDebug() << "unable to grab image from item with no shwon window";
        return nullptr;
    }
    QQuickWindow *window = item->window();
    QImage grabbed = window->grabWindow();
    QRectF rf(item->x(), item->y(), item->width(), item->height());
    rf = rf.intersected(QRectF(0, 0, grabbed.width(), grabbed.height()));
    QImage img = grabbed.copy(rf.toAlignedRect());
    return new QmlImage(img, this);
}

QImage generateChessboard(const int width, const int height) {
    QImage chess(width, height, DIFF_IMG_FORMAT);
    chess.fill( "#666666" );
    QPainter painter(&chess);
    const int imgMinGrid = std::min( width / 3, height / 3 );
    const int gridSize = (imgMinGrid < CHESS_GRID_SIZE) ? 2 : CHESS_GRID_SIZE;
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
        QImage emptyDiff(2, 1, DIFF_IMG_FORMAT);
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

    QImage diff(widthMax, heightMax, DIFF_IMG_FORMAT);
    diff.fill( Qt::transparent );
    {
        QPainter painter(&diff);
        painter.setCompositionMode( QPainter::CompositionMode_Difference );
        painter.drawImage(0, 0, imgA);
        painter.drawImage(0, 0, imgB);
    }

    QImage threshold(widthMax, heightMax, DIFF_IMG_FORMAT);
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

    //QImage join(widthMax*2 + DIFF_IMAGES_SPACING, heightMax*2 + DIFF_IMAGES_SPACING, DIFF_IMG_FORMAT);
    //join.fill( Qt::transparent );
    QImage join = generateChessboard(widthMax*2 + DIFF_IMAGES_SPACING, heightMax*2 + DIFF_IMAGES_SPACING);
    {
        QPainter painter(&join);
        if (imgA.isNull() == false) {
            painter.drawImage(0, 0, imgA);
        }
//        else {
//            const QImage chess = generateChessboard(widthMax, heightMax);
//            painter.drawImage(0, 0, chess);
//        }
        if (imgB.isNull() == false) {
            painter.drawImage(widthMax + DIFF_IMAGES_SPACING, 0, imgB);
        }
//        else {
//            const QImage chess = generateChessboard(widthMax, heightMax);
//            painter.drawImage(widthMax, 0, chess);
//        }
        painter.drawImage(0, heightMax + DIFF_IMAGES_SPACING, threshold);
        painter.drawImage(widthMax + DIFF_IMAGES_SPACING, heightMax + DIFF_IMAGES_SPACING, diff);
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

    return new ImageLoader(engine);
}
