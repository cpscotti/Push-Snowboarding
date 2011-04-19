#include "spintowinkid.h"

SpinToWinKid::SpinToWinKid(QObject *parent) : PushBurtonGenericDevice(parent)
{
    onAir = false;
    qDebug() << "SpinToWinKid started";
}



SpinToWinKid::~SpinToWinKid()
{
    qDebug() << "SpinToWinKid closed";
}

QString SpinToWinKid::get_description()
{
    return QString("SpinToWinKid");
}

bool SpinToWinKid::is_online()
{
    return true;
}

bool SpinToWinKid::subscribesToAny()
{
    return true;
}

//This function will return true when the given device is a AirTimeDetector or a Imu
bool SpinToWinKid::subscribesTo(PushBurtonGenericDevice* deviceType)
{
    if(typeid(*deviceType) == typeid(PushN8AirTimeDetector)){
        return true;
    } else if(typeid(*deviceType) == typeid(PushN8IMUDevice)){
        return true;
    } else {
        return false;
    }
}


void SpinToWinKid::incoming_reading(NPushLogTick * tick)
{
    if(typeid(*tick) == typeid(NPushAirTimeTick)) {

        NPushAirTimeTick * airTimeTick = (NPushAirTimeTick *)tick;

        if(airTimeTick->isOnlyHint) {
            qDebug() << "SpinToWinKid got hint";
            onAir = true;
            spinAcc = 0.0;
            lastTstamp = airTimeTick->msecsToEpoch;
        } else if(airTimeTick->landed) {
            qDebug() << "Landed Trick; SpinAcc = " << spinAcc;
            onAir = false;
        }
    } else if(typeid(*tick) == typeid(NPushIMUTick)
        && onAir){

        NPushIMUTick * imuTick = (NPushIMUTick*)tick;
        double spinInc = 0.0;
        if(imuTick->msecsToEpoch > lastTstamp) {
            spinInc = ((double)imuTick->gyro[2])/14.375;
            qDebug() << "T = " << (imuTick->msecsToEpoch - lastTstamp)*0.001;
            spinInc *= (imuTick->msecsToEpoch - lastTstamp)*0.001;
            lastTstamp = imuTick->msecsToEpoch;
        }
        spinAcc += spinInc;
    }
}
