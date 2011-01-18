#ifndef PUSHN8PHONEACC_H
#define PUSHN8PHONEACC_H

#include "pushburtongenericdevice.h"

#include "npushacctick.h"

#include <QAccelerometer>
#include <QDebug>

class PushN8PhoneAcc : public PushBurtonGenericDevice
{
    Q_OBJECT
public:
    PushN8PhoneAcc();
    ~PushN8PhoneAcc();

    QString get_description();

    bool is_online();

private:
    QAccelerometer * accSensor;

protected:
    void timerEvent(QTimerEvent *);
};

#endif // PUSHN8PHONEACC_H
