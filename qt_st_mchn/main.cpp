#include <QCoreApplication>
#include "master.h"
#include "slave.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Master master("Zerk");
    Slave slave("John") , slave1("Bob");
    QObject::connect(&master , &Master::stateChanged , &slave , &Slave::onMasterStatusChanged);
    QObject::connect(&master , &Master::stateChanged , &slave1 , &Slave::onMasterStatusChanged);
    master.printState();
    slave.printState();
    slave1.printState();

    printf("=====\n");
    master.setState(STATE_HUNT);
    master.printState();
    slave.printState();
    slave1.printState();

    printf("=====\n");
    master.setState(STATE_REST);
    master.printState();
    slave.printState();
    slave1.printState();


    getchar();

    return a.exec();
}
