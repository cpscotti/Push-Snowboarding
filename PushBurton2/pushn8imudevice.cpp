#include "pushn8imudevice.h"

PushN8IMUDevice::PushN8IMUDevice(QBtDevice a_device, QObject *parent) :
        PushN8BtDevice(a_device, parent)
{
    timerId = 0;
    timerPeriod = 50;


    //Assumes messages are like: "IMU,231,23,43,54,454,645,54,23,54,END;"
    packetLen = 11;
    packetStart = "IMU";
    packetEnd = "END";
}

PushN8IMUDevice::~PushN8IMUDevice()
{

}


void PushN8IMUDevice::gotDataReceived(QString data)
{

    QStringList params;
    if(getPackage(data, params))
    {

        NPushIMUTick * newIMUTick = new NPushIMUTick(params);

//        qDebug() << "Parms: " << params[1] << "," << params[2] << "," << params[3];

        if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
        {
            //anyone listening to us
            emit reading_ready(newIMUTick);
        } else {
            delete newIMUTick;
        }
    }

}

