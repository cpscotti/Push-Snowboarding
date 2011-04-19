#ifndef SPINTOWINKID_H
#define SPINTOWINKID_H

#include <QDebug>
#include <QString>
#include <typeinfo>

#include "pushburtongenericdevice.h"

#include "npushairtimetick.h"
#include "pushn8airtimedetector.h"

#include "pushn8imudevice.h"
#include "npushimutick.h"

class SpinToWinKid : public PushBurtonGenericDevice
{
    Q_OBJECT
public:
    explicit SpinToWinKid(QObject *parent = 0);

    ~SpinToWinKid();

    QString get_description();
    bool is_online();

    bool subscribesToAny();//returns true..

    //This function will return true when the given device is a AirTimeDetector or a Imu
    bool subscribesTo(PushBurtonGenericDevice* deviceType);

signals:

public slots:
    void incoming_reading(NPushLogTick *);

private:
    quint64 lastTstamp;
    double spinAcc;
    bool onAir;


};

#endif // SPINTOWINKID_H
