#include "pushn8phonemag.h"

PushN8PhoneMag::PushN8PhoneMag()
{
    magSensor = new QMagnetometer();

    magSensor->connectToBackend();
    if(!magSensor->isConnectedToBackend()) {
        qDebug() << "Could not connect Magnetometer to backend";
    }

    if(!magSensor->start()) {
        qDebug() << "Could not start Magnetometer";
    }

    if(!magSensor->isActive()) {
        qDebug() << "Magnetometer is not active";
    }

    emit connected();

    timerId = 0;
    timerPeriod = 200;
}

PushN8PhoneMag::~PushN8PhoneMag()
{
    if(magSensor)
        delete magSensor;
}

QString PushN8PhoneMag::get_description()
{
    return QString("Phone Internal Magnetometer");
}

bool PushN8PhoneMag::is_online()
{
    if(magSensor)
        return magSensor->isActive();
    else
        return false;
}

void PushN8PhoneMag::timerEvent(QTimerEvent * event)
{
    NPushMagTick * newMagTick = new NPushMagTick(magSensor->reading());

    if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
    {
        emit reading_ready(newMagTick);
    } else {
        //TODO memory leak danger here put on "stand by"
        delete newMagTick;
    }
}
