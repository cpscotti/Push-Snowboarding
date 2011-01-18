#include "pushn8gsrdevice.h"

PushN8GSRDevice::PushN8GSRDevice(QBtDevice a_device, QObject *parent) :
        PushN8BtDevice(a_device, parent)
{
    timerId = 0;
    timerPeriod = 500;

    //Assumes messages are like: "GSR,231,END;"
    packetLen = 3;
    packetStart = "GSR";
    packetEnd = "END";

}

PushN8GSRDevice::~PushN8GSRDevice()
{
}

void PushN8GSRDevice::gotDataReceived(QString data)
{

    QStringList params;
    if(getPackage(data, params))
    {
        int gsrr = params[1].toInt();
        if(gsrr > 0 && gsrr < 1024)
        {
            NPushGSRTick * newGSRTick = new NPushGSRTick(gsrr);

            if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
            {
                //anyone listening to us
                emit reading_ready(newGSRTick);
            } else {
                delete newGSRTick;
            }
        }
    }
//    qDebug() << "Data received: " << data;
}
