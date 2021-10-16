#include "master.h"


Master::Master(QString name):state(STATE_REST) , name(name)
{
    connect(this , &Master::stateChanged , this , &Master::printState);
}

void Master::setState(int state)
{
    this->state = state;
    emit this->stateChanged(state);
}

int Master::getState() const
{
    return state;
}

void Master::printState()
{
    printf("State of Master %s is " ,name.toStdString().c_str());
    switch(state){
        case STATE_HUNT: {
            printf("Hunt");
        } break;
        case STATE_REST: {
            printf("Rest");
        } break;
        default: {
            printf("Unnone");
        }
    }
    printf("\n");
}
