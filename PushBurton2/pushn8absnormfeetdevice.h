#ifndef PUSHN8ABSNORMFEETDEVICE_H
#define PUSHN8ABSNORMFEETDEVICE_H

#include "pushburtongenericdevice.h"

#include "pushn8footdevice.h"

#include "npushfoottick.h"
#include "npushlogtick.h"

#include "npushnormfeettick.h"

#include "AbsNormFeetTypes.h"

class PushN8AbsNormFeetDevice : public PushBurtonGenericDevice
{
    Q_OBJECT
public:
    PushN8AbsNormFeetDevice();
    ~PushN8AbsNormFeetDevice();

    bool subscribesToAny();
    bool subscribesTo(PushBurtonGenericDevice* deviceType);

    QString get_description();

    void start_readings();
    void stop_readings();

    bool is_online();

    SnowboradPressureData pdata;

    int normRunCount;

    quint64 lastReading;
    char lastReadingSide;


    bool flying;
    quint64 takeOffTime;
    quint64 lastAirborneTime;

public slots:
    void incomming_reading(NPushLogTick *);

private:
    void reset_bounds();
};

#endif // PUSHN8ABSNORMFEETDEVICE_H
