#ifndef SPINTOWINKID_H
#define SPINTOWINKID_H

#include <QDebug>
#include <QString>
#include <typeinfo>

#include "pushburtongenericdevice.h"

#include "npushairtimetick.h"
#include "pushn8airtimedetector.h"

#include "npushrotationstick.h"

#include "pushn8imudevice.h"
#include "npushimutick.h"

#define PRE_SPIN_BUFFER_SIZE 2

#define POS_SPIN_BUFFER_SIZE 2

/*
 Chip offset:
 -121,114,-39
 -144,107,-32
 -150,110,-32

 */

class RotationCounter : public PushBurtonGenericDevice
{
    Q_OBJECT
public:
    explicit RotationCounter(QObject *parent = 0);

    ~RotationCounter();

    QString get_description();
    bool is_online();

    bool subscribesToAny();//returns true..

    //This function will return true when the given device is a AirTimeDetector or a Imu
    bool subscribesTo(PushBurtonGenericDevice* deviceType);

signals:

public slots:
    void incoming_reading(NPushLogTick *);

private:

    void integrateSpinAcc(double meas);

    void emitRotTick();

    quint64 lastTstamp;
    double spinAcc;
    bool onAir;

    int afterJumpDecCnter;

    double pastSpinBuff[PRE_SPIN_BUFFER_SIZE];
    unsigned int pastSpinBuffPt;

};

#endif // SPINTOWINKID_H
