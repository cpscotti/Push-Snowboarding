#include "pushn8heartdevice.h"

PushN8HeartDevice::PushN8HeartDevice(QBtDevice a_device, QObject *parent) :
        PushN8BtDevice(a_device, parent)
{
    timerId = 0;
    timerPeriod = 1000;

    //Assumes messages are like: "HRM,80,75,90,END;"
    packetLen = 5;
    packetStart = "HRM";
    packetEnd = "END";
}

PushN8HeartDevice::~PushN8HeartDevice()
{

}

void PushN8HeartDevice::gotDataReceived(QString data)
{
    QStringList params;
    if(getPackage(data, params))
    {
        int hr_3 = params[1].toInt();
        int hr_2 = params[2].toInt();
        int hr_1 = params[3].toInt();

        NPushHeartTick * newHeartTick = new NPushHeartTick(hr_3, hr_2, hr_1);

        //qDebug() << "Data received: " << data;

        if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
        {
            //anyone listening to us
            emit reading_ready(newHeartTick);
        } else {
            delete newHeartTick;
        }
    }
}
