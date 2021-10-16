#ifndef SLAVE_H
#define SLAVE_H

#define STATE_WALK 1
#define STATE_HOME 2

#include <QObject>
#include "master.h"

class Slave : public QObject
{
    Q_OBJECT
public:
    explicit Slave(QString name , QObject *parent = nullptr);
    QString getName() const;
    void setName(const QString &value);
    void printState();

    int getState() const;
public slots:
    void setState(int value);
    void onMasterStatusChanged(int masterStatus);

private:
    QString name;
    int state;

signals:

};

#endif // SLAVE_H
