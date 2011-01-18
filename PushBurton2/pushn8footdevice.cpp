#include "pushn8footdevice.h"

PushN8FootDevice::PushN8FootDevice(QBtDevice a_device, QObject *parent) :
            PushN8BtDevice(a_device, parent)
{

    if(device.getName().count("FOOT_L") > 0)
    {
        side = 'L';
    } else if(device.getName().count("FOOT_R") > 0){
        side = 'R';
    } else {
        side = '?';
    }

    timerId = 0;
    timerPeriod = 50;

    //                                toes,heel
    //Assumes messages are like: "FOOT,231,3123,END;"
    packetLen = 4;
    packetStart = "FOOT";
    packetEnd = "END";

}

PushN8FootDevice::~PushN8FootDevice()
{
}

void PushN8FootDevice::gotDataReceived(QString data)
{

    QStringList params;
    if(getPackage(data, params))
    {
        int ntoes = params[1].toInt();
        int nheel = params[2].toInt();

        NPushFootTick * newFootTick = new NPushFootTick(side, ntoes, nheel);

        qDebug() << "Foot data received: " << ntoes << ", " << nheel;

        if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
        {
            //anyone listening to us
            emit reading_ready(newFootTick);
        } else {
            delete newFootTick;
        }
    }
}

char PushN8FootDevice::get_side()
{
    return side;
}
