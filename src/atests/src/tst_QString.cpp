///
/// File taken from Qt examples
///

#include "QtTestRegister.h"


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
