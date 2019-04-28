#include "QTestRegister.h"
#include <QtTest/QtTest>


class TestQString: public QObject
{
    Q_OBJECT

private slots:

    void toUpper() {
        QString str = "Hello";
        QCOMPARE(str.toUpper(), QString("HELLO"));
    }
};


QTEST_MAIN(TestQString)
#include "tst_QString.moc"
