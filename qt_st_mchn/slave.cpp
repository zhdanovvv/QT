#include "slave.h"

Slave::Slave(QString name, QObject *parent) : QObject(parent) , name(name) , state(STATE_WALK)
{

}

QString Slave::getName() const
{
    return name;
}

void Slave::setName(const QString &value)
{
    name = value;
}

void Slave::printState()
{
    printf("State of Slave %s is " ,name.toStdString().c_str());
    switch(state){
        case STATE_WALK: {
            printf("Walk");
        } break;
        case STATE_HOME: {
            printf("Home");
        } break;
        default: {
            printf("Unnone");
        }
    }
    printf("\n");
}

int Slave::getState() const
{
    return state;
}

void Slave::setState(int value)
{
    state = value;
}

void Slave::onMasterStatusChanged(int masterStatus)
{
    switch (masterStatus) {
    case STATE_HUNT:{
        this->state = STATE_HOME;
    }break;
    case STATE_REST:{
        this->state = STATE_WALK;
    }break;
    }
}
