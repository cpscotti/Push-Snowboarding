#include "pushn8phonegps.h"

PushN8PhoneGPS::PushN8PhoneGPS(QObject * parent)
{
    gpsSource = QGeoPositionInfoSource::createDefaultSource(0);

    connect(gpsSource, SIGNAL(positionUpdated(QGeoPositionInfo)),
            this, SLOT(positionUpdated(QGeoPositionInfo)));

    gpsSource->requestUpdate(60000);
    gpsSource->setUpdateInterval(500);

    gpsOnline = false;

    timerId = -1;//disable any periodic events
}

PushN8PhoneGPS::~PushN8PhoneGPS()
{
    gpsSource->deleteLater();
}

void PushN8PhoneGPS::start_readings()
{
    gpsSource->startUpdates();
}

void PushN8PhoneGPS::stop_readings()
{
    gpsSource->stopUpdates();
}

void PushN8PhoneGPS::timerEvent(QTimerEvent *)
{

}

QString PushN8PhoneGPS::get_description()
{
    return QString("Phone's Internal Gps");
}

bool PushN8PhoneGPS::is_online()
{
    return gpsOnline;
}

void PushN8PhoneGPS::positionUpdated(QGeoPositionInfo info)
{
    if(info.isValid())
    {
        //emitting connected signal when receiving first reading
        if(!gpsOnline)
            emit connected();

        gpsOnline = true;
        NPushGpsTick * newGPSTick = new NPushGpsTick(info);

        if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
        {
            emit reading_ready(newGPSTick);
        } else {
            //TODO memory leak danger here put on "stand by"
            delete newGPSTick;
        }
    }

}
