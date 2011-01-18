#include "pushn8absnormfeetdevice.h"

PushN8AbsNormFeetDevice::PushN8AbsNormFeetDevice()
{

    lastReading = 0;
    lastReadingSide = 'R';

    reset_bounds();
}

PushN8AbsNormFeetDevice::~PushN8AbsNormFeetDevice()
{

}


void PushN8AbsNormFeetDevice::reset_bounds()
{
    pdata.reset();

    normRunCount = 0;

    flying = false;
}

bool PushN8AbsNormFeetDevice::subscribesToAny()
{
    return true;
}

bool PushN8AbsNormFeetDevice::subscribesTo(PushBurtonGenericDevice* deviceType)
{
    if(typeid(*deviceType) == typeid(PushN8FootDevice))
    {
        return true;
    } else {
        return false;
    }
}

QString PushN8AbsNormFeetDevice::get_description()
{
    return "NormalizedFeetAnalizer";
}

void PushN8AbsNormFeetDevice::start_readings()
{
    reset_bounds();
}

void PushN8AbsNormFeetDevice::stop_readings()
{

}

bool PushN8AbsNormFeetDevice::is_online()
{
    return (normRunCount > 0);
}


void PushN8AbsNormFeetDevice::incomming_reading(NPushLogTick * gtick)
{
    if(typeid(*gtick) == typeid(NPushFootTick)) {
        NPushFootTick * ftick = (NPushFootTick *)gtick;

        if(ftick->side == 'L') {
            pdata.LToe.update(ftick->toes);
            pdata.LHeel.update(ftick->heel);
        } else {
            pdata.RToe.update(ftick->toes);
            pdata.RHeel.update(ftick->heel);
        }

        if((ftick->msecsToEpoch - lastReading) < 100 &&
           (ftick->side != lastReadingSide)) {

            pdata.computeSum();

            normRunCount++;

            NPushNormFeetTick * newTick = new NPushNormFeetTick(pdata, lastReading);

            if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
            {
                emit reading_ready(newTick);
            } else {
                delete newTick;
            }

        }

        lastReading = ftick->msecsToEpoch;
        lastReadingSide = ftick->side;

//        if((ftick->msecsToEpoch - lastReading) < 100 &&
//           (ftick->side != lastReadingSide)) {

//            int sensorsRawSum = pdata.computeSum();

//            normRunCount++;

//            NPushAirTimeTick * newTick = 0;
//            if(sensorsRawSum < 2300) {
//                if(!flying) {
//                    takeOffTime = ftick->msecsToEpoch;
//                    lastAirborneTime = takeOffTime;
//                    flying = true;
//                } else {
//                    lastAirborneTime = ftick->msecsToEpoch;
//                }
//                quint64 airTime = lastAirborneTime-takeOffTime;
//                if(airTime > 400) {
//                    newTick = new NPushAirTimeTick(airTime, false, ftick->msecsToEpoch);
//                }
//            } else {
//                if(flying) {
//                    quint64 airTime = lastAirborneTime-takeOffTime;
//                    newTick = new NPushAirTimeTick(airTime, true, ftick->msecsToEpoch);
//                    flying = false;
//                }
//            }



//            qDebug() << "(range) Rt[" << (pdata.RToe.maxVal - pdata.RToe.minVal) << "] Rh [" << (pdata.RHeel.maxVal - pdata.RHeel.minVal) << "]";
//            qDebug() << "(range) Lt[" << (pdata.LToe.maxVal - pdata.LToe.minVal) << "] Lh [" << (pdata.LHeel.maxVal - pdata.LHeel.minVal) << "]";

//            qDebug() << "(norml) Rt[" << pdata.RToe.normVal << "] Rh [" << pdata.RHeel.normVal << "]";
//            qDebug() << "(norml) Lt[" << pdata.LToe.normVal << "] Lh [" << pdata.LHeel.normVal << "]";

//            qDebug() << "(sum) " << ftick->msecsToEpoch << " -> [" << sensorsRawSum << "] normCount = " << normRunCount;

//        }
    }
}
