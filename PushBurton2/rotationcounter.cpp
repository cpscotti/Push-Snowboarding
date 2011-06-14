#include "rotationcounter.h"

RotationCounter::RotationCounter(QObject *parent) : PushBurtonGenericDevice(parent)
{
    onAir = false;
    pastSpinBuffPt = 0;
    lastTstamp = 0;

    afterJumpDecCnter = 0;

    for(int i=0;i<PRE_SPIN_BUFFER_SIZE;i++)
        pastSpinBuff[i] = 0.0;
}



RotationCounter::~RotationCounter()
{
    qDebug() << "RotationCounter\'s destructor";
}

QString RotationCounter::getName()
{
    return QString("push.abstract.snowb.rotation");
}

bool RotationCounter::is_online()
{
    return true;
}

bool RotationCounter::subscribesToAny()
{
    return true;
}

//This function will return true when the given device is a AirTimeDetector or a Imu
bool RotationCounter::subscribesTo(PushBurtonGenericDevice* deviceType)
{
    if(typeid(*deviceType) == typeid(PushN8AirTimeDetector)){
        return true;
    } else if(typeid(*deviceType) == typeid(PushN8IMUDevice)){
        return true;
    } else {
        return false;
    }
}


void RotationCounter::incoming_reading(NPushLogTick * tick)
{
    if(typeid(*tick) == typeid(NPushAirTimeTick)) {

        NPushAirTimeTick * airTimeTick = (NPushAirTimeTick *)tick;

        if(airTimeTick->isOnlyHint && afterJumpDecCnter == 0) {
//            qDebug() << "SpinToWinKid got hint";
            //This 'hint' points to a possible airTime starting now. But it'll only be confirmed with the landed=true
            onAir = true;
            spinAcc = 0.0;
            afterJumpDecCnter = 0;

            for(int i=0;i<PRE_SPIN_BUFFER_SIZE;i++){

//                spinAcc += pastSpinBuff[i];
                integrateSpinAcc(pastSpinBuff[i]);

            }

//            qDebug() << "Starting at; SpinAcc = " << spinAcc;

            lastTstamp = airTimeTick->msecsToEpoch;
        } else if(airTimeTick->landed) {
//                qDebug() << "Landed Trick; SpinAcc = " << spinAcc;
            onAir = false;
            afterJumpDecCnter = POS_SPIN_BUFFER_SIZE;
        }
    } else if(typeid(*tick) == typeid(NPushIMUTick)) {
//        && onAir){

        NPushIMUTick * imuTick = (NPushIMUTick*)tick;
        double spinInc = 0.0;
        if(imuTick->msecsToEpoch > lastTstamp && lastTstamp != 0) {

            //Which axis are we lookin?
            spinInc = ((double)imuTick->gyro[2])/14.375;
            spinInc *= (imuTick->msecsToEpoch - lastTstamp)*0.001;


        }

        lastTstamp = imuTick->msecsToEpoch;


        if(onAir) {
//            spinAcc += spinInc;
            integrateSpinAcc(spinInc);

            //emit intermediary rotation tick? if so, emit here.
        }
        if(afterJumpDecCnter > 0) {
//            spinAcc += spinInc;
            integrateSpinAcc(spinInc);

            afterJumpDecCnter--;
            if(afterJumpDecCnter == 0) {
                emitRotTick();
            }
        } else {
            pastSpinBuffPt++;
            pastSpinBuffPt %= PRE_SPIN_BUFFER_SIZE;
            pastSpinBuff[pastSpinBuffPt] = spinInc;
        }
    }
}

void RotationCounter::integrateSpinAcc(double meas)
{
    static double lastMeas = 0.0;
    spinAcc += (meas + lastMeas)/2;
    lastMeas = meas;
}

void RotationCounter::emitRotTick()
{
    NPushRotationsTick * newTick = 0;
    newTick = new NPushRotationsTick(spinAcc, lastTstamp);

    if(newTick != 0) {
        if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
        {
            emit reading_ready(newTick);
        } else {
            delete newTick;
        }
    }
}
