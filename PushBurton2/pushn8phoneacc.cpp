#include "pushn8phoneacc.h"

PushN8PhoneAcc::PushN8PhoneAcc()
{
    accSensor = new QAccelerometer();
    accSensor->connectToBackend();
    if(!accSensor->isConnectedToBackend()) {
        qDebug() << "Could not connect mAccel to backend";
    }

    if(!accSensor->start()) {
        qDebug() << "Could not start mAccel";
    }

    if(!accSensor->isActive()) {
        qDebug() << "mAccel is not active";
    }

    emit connected();

    timerId = 0;
    timerPeriod = 100;
}

PushN8PhoneAcc::~PushN8PhoneAcc()
{
    if(accSensor)
        delete accSensor;
}

QString PushN8PhoneAcc::get_description()
{
    return QString("Phone Internal Accelerometer");
}

bool PushN8PhoneAcc::is_online()
{
    if(accSensor)
        return accSensor->isActive();
    else
        return false;
}

void PushN8PhoneAcc::timerEvent(QTimerEvent * event)
{
    NPushAccTick * newAccTick = new NPushAccTick(accSensor->reading());

    if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
    {
        emit reading_ready(newAccTick);
    } else {
        //TODO memory leak danger here put on "stand by"
        delete newAccTick;
    }
}
