#include <QtTest/QtTest>
#include "../dataeater.hpp"

class Test_dataeater: public QObject {
    Q_OBJECT

private slots:
    void eat_correct_packet() {
        QString h = "HELLO";
        QVERIFY(h == "HELLO");   
    }
};
