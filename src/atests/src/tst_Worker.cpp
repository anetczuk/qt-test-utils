#include "QTestRegister.h"

#include "Worker.h"


class TestWorker: public QObject {
    Q_OBJECT

private slots:

    void test_work() {
    	Worker worker;
        QCOMPARE(worker.work(), true);
    }

};

//QTEST_MAIN(TestWorker)
QTEST_REGISTER(TestWorker)
#include "tst_Worker.moc"
