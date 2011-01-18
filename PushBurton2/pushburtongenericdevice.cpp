#include "pushburtongenericdevice.h"

PushBurtonGenericDevice::PushBurtonGenericDevice(QObject * parent) : QObject(parent)
{
}

PushBurtonGenericDevice::~PushBurtonGenericDevice()
{
}

void PushBurtonGenericDevice::start_readings()
{
    if(timerId == 0)
        timerId = startTimer(timerPeriod);
}

void PushBurtonGenericDevice::stop_readings()
{
    if(timerId != 0)
        killTimer(timerId);
    timerId = 0;
}

void PushBurtonGenericDevice::disconnect_from_backend()
{
}

bool PushBurtonGenericDevice::subscribesTo(PushBurtonGenericDevice* deviceType)
{
    return false;
}

bool PushBurtonGenericDevice::subscribesToAny()
{
    return false;
}

void PushBurtonGenericDevice::incomming_reading(NPushLogTick *)
{

}

bool PushBurtonGenericDevice::start_run()
{

}


bool PushBurtonGenericDevice::end_run()
{

}
