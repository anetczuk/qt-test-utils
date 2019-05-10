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

#include "QmlImage.h"

#include <QImageWriter>
#include <QCryptographicHash>


bool QmlImage::equals(QmlImage *image) const {
    if (image == nullptr)
        return false;
    return equals( *image );
}

bool QmlImage::equals(const QmlImage &image) const {
    const QImage& img = qimage();
    const QImage& img2 = image.qimage();
    return (img == img2);
}

bool QmlImage::equals(QmlImage &image) const {
    const QImage& img = qimage();
    const QImage& img2 = image.qimage();
    return (img == img2);
}

bool QmlImage::equals(const QObject *image) const {
    const QImage* img2 = getImageFromTestImage(image);
    if (img2 == nullptr)
        return false;
    const QImage& img = qimage();
    return (img == *img2);
}

bool QmlImage::equals(QObject *image) const {
    const QImage* img2 = getImageFromTestImage(image);
    if (img2 == nullptr)
        return false;
    const QImage& img = qimage();
    return (img == *img2);
}

void QmlImage::save(const QString& filePath) const {
    if (image.isNull()) {
        QString message = QStringLiteral("Can't save to %1: empty image").arg(filePath);
        const std::string msg = message.toStdString();
        qFatal( "%s", msg.c_str() );
    }
    QImageWriter writer(filePath);
    if (!writer.write(image)) {
        QString message = QStringLiteral("Can't save to %1: %2").arg(filePath, writer.errorString());
        const std::string msg = message.toStdString();
        qFatal( "%s", msg.c_str() );
    }
}

QString QmlImage::hash() const {
    QByteArray byteArray = QByteArray::fromRawData((const char*)image.bits(), image.byteCount());
    QString data = QString::fromUtf8(byteArray.toBase64().data());
    QCryptographicHash md5(QCryptographicHash::Md5);
    md5.addData(data.toUtf8());
    QString hash = QString(md5.result().toHex());
    return hash;
}

const QImage* QmlImage::getImageFromTestImage(const QObject* object) {
    const QString objClassName( object->metaObject()->className() );
    if (objClassName == QString("QuickTestImageObject")) {
        const QuickTestImageObjectMock* testImage = static_cast<const QuickTestImageObjectMock*>( object );
        const QImage& img = testImage->m_image;
        return &img;
    }
    if ( objClassName == QString("QmlImage") ) {
        const QmlImage* image = static_cast<const QmlImage*>( object );
        const QImage& img = image->qimage();
        return &img;
    }
    return nullptr;
}

QImage* QmlImage::getImageFromTestImage(QObject* object) {
    const QString objClassName( object->metaObject()->className() );
    if (objClassName == QString("QuickTestImageObject")) {
        QuickTestImageObjectMock* testImage = static_cast<QuickTestImageObjectMock*>( object );
        QImage& img = testImage->m_image;
        return &img;
    }
    if ( objClassName == QString("QmlImage") ) {
        QmlImage* image = static_cast<QmlImage*>( object );
        QImage& img = image->qimage();
        return &img;
    }
    return nullptr;
}
