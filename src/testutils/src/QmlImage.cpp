#include "QmlImage.h"

#include <QImageWriter>


bool QmlImage::equals(QmlImage *image) const {
    if (image == nullptr)
        return false;
    return equals( *image );
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

void QmlImage::save(const QString& filePath) {
    QImageWriter writer(filePath);
    if (!writer.write(image)) {
        QString message = QStringLiteral("Can't save to %1: %2").arg(filePath, writer.errorString());
        const std::string msg = message.toStdString();
        qFatal( "%s", msg.c_str() );
    }
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
