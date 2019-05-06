#include <iostream>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "Worker.h"


int main(int argc, char *argv[]) {
	QGuiApplication app(argc, argv);

	Q_INIT_RESOURCE(alibres);
    Q_INIT_RESOURCE(qmake_qmake_immediate);

	Worker worker;
    worker.work();

	QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/window.qml")));

    const int exitcode = app.exec();
    std::cout << "Done" << std::endl;
    return exitcode;
}
