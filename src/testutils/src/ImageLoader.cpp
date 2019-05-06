#include "ImageLoader.h"


QObject *ImageLoader::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    static ImageLoader* instance = new ImageLoader();
    return instance;
}
