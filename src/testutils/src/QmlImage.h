#ifndef QMLIMAGE_H_
#define QMLIMAGE_H_

#include <QImage>
#include <QDebug>


class QmlImage: public QObject {
    Q_OBJECT

    QImage image;


public:

    QmlImage(QObject* parent = nullptr): QObject(parent), image() {
    }

    QmlImage(QImage& aImage, QObject* parent = nullptr): QObject(parent), image(aImage) {
    }

    virtual ~QmlImage() {
    }

    bool operator==(const QImage &otherImage) const {
        return (image == otherImage);
    }

    bool operator==(const QmlImage &otherImage) const {
        const QImage& other = otherImage.qimage();
        return (image == other);
    }

    bool operator==(const QObject *otherImage) const {
        return equals(otherImage);
    }

    bool operator==(QObject *otherImage) const {
        return equals(otherImage);
    }

    Q_INVOKABLE bool equals(QmlImage *image) const;

    Q_INVOKABLE bool equals(QmlImage &image) const;

    Q_INVOKABLE bool equals(const QObject *image) const;

    Q_INVOKABLE bool equals(QObject *image) const;

    Q_INVOKABLE bool empty() const {
        return image.isNull();
    }

    Q_INVOKABLE const QImage& qimage() const {
        return image;
    }

    Q_INVOKABLE QImage& qimage() {
        return image;
    }

    Q_INVOKABLE int width() const {
        return image.width();
    }

    Q_INVOKABLE int height() const {
        return image.height();
    }

    Q_INVOKABLE QSize size() const {
        return image.size();
    }

    Q_INVOKABLE int red(const int x, const int y) const {
        const QColor& color = image.pixelColor(x, y);
        return color.red();
    }

    Q_INVOKABLE int green(const int x, const int y) const {
        const QColor& color = image.pixelColor(x, y);
        return color.green();
    }

    Q_INVOKABLE int blue(const int x, const int y) const {
        const QColor& color = image.pixelColor(x, y);
        return color.blue();
    }

    Q_INVOKABLE int alpha(const int x, const int y) const {
        const QColor& color = image.pixelColor(x, y);
        return color.alpha();
    }

    Q_INVOKABLE QVariant pixel(int x, int y) const {
        if (image.isNull()
         || x >= image.width()
         || y >= image.height()
         || x < 0
         || y < 0
         || x * y >= image.width() * image.height())
            return QVariant();
        return QColor::fromRgba(image.pixel(QPoint(x, y)));
    }

    Q_INVOKABLE bool load(const QString& path) {
        return image.load( path );
    }

    Q_INVOKABLE void save(const QString& filePath);


private:

    static const QImage* getImageFromTestImage(const QObject* object);

    static QImage* getImageFromTestImage(QObject* object);

};


// ========================================================


//
// Mock of QuickTestImageObject to access it's QImage.
//
class QuickTestImageObjectMock: public QObject {
    Q_OBJECT

public:

    QuickTestImageObjectMock(const QImage& img, QObject *parent = nullptr): QObject(parent), m_image(img) {
    }

    virtual ~QuickTestImageObjectMock() {
    }

    QImage m_image;

};


#endif /* QMLIMAGE_H_ */
