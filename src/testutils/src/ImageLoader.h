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

#ifndef IMAGE_LOADER_H_
#define IMAGE_LOADER_H_

#include <QObject>
#include <QImage>
#include <QDebug>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickWindow>

#include "QmlImage.h"


class ImageLoader: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ImageLoader)


public:

    // instance needed for unit tests
    ImageLoader(): QObject() {
    }

    virtual ~ImageLoader() {
    }

    Q_INVOKABLE QmlImage* loadImage(const QString& path) {
        QmlImage* obj = new QmlImage(this);
        obj->load(path);
        return obj;
    }

    Q_INVOKABLE QmlImage* saveImageOfItem(QQuickItem *item, const QString& path) {
        QmlImage* img = grabImage(item);
        if (img == nullptr)
            return nullptr;
        img->save( path );
        return img;
    }

    Q_INVOKABLE QmlImage* grabImage(QQuickItem *item) {
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

    QmlImage* makeDiff(const QmlImage& imgA, const QmlImage& imgB);

    Q_INVOKABLE QmlImage* makeDiff(QmlImage& imgA, QmlImage& imgB);

    QmlImage* makeDiff(const QmlImage* imgA, const QmlImage* imgB);

    Q_INVOKABLE QmlImage* makeDiff(QmlImage* imgA, QmlImage* imgB);

    QImage makeDiff(const QImage& imgA, const QImage& imgB);


    // singleton needed for registration in QML
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

};


#endif /* IMAGE_LOADER_H_ */
