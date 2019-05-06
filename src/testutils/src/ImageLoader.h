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

    Q_INVOKABLE QmlImage* loadImage(const QString& path) {
        QmlImage* obj = new QmlImage(this);
        obj->load(path);
        return obj;
    }

    Q_INVOKABLE QmlImage* grabImage(QQuickItem *item) {
        if (item && item->window()) {
            QQuickWindow *window = item->window();
            QImage grabbed = window->grabWindow();
            QRectF rf(item->x(), item->y(), item->width(), item->height());
            rf = rf.intersected(QRectF(0, 0, grabbed.width(), grabbed.height()));
            QImage img = grabbed.copy(rf.toAlignedRect());
            return new QmlImage(img, this);
        }
        return nullptr;
    }

    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);


private:

    ImageLoader(): QObject() {
        qDebug() << "creating ImageLoader singleton";
    }

};


#endif /* IMAGE_LOADER_H_ */
