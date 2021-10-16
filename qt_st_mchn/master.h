#ifndef MASTER_H
#define MASTER_H
#include <QString>
#include <QObject>

#define STATE_HUNT 1
#define STATE_REST 2


class Master:public QObject
{
    Q_OBJECT
public:   
    Master(QString name);
    void setState(int state);
    int getState() const;
signals:
    void stateChanged(int state);
public slots:
    void printState();
private:
    int state;
    QString name;
};

#endif // MASTER_H
