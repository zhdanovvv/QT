#include <QApplication>
#include <QString>
#include <QTextStream>
#include "list.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    User window;
    window.show();
    return a.exec();
}
