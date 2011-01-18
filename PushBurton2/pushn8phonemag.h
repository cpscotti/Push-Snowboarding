#ifndef PUSHN8PHONEMAG_H
#define PUSHN8PHONEMAG_H

#include "pushburtongenericdevice.h"

#include "npushmagtick.h"

#include <QMagnetometer>
#include <QDebug>

class PushN8PhoneMag : public PushBurtonGenericDevice
{
    Q_OBJECT
public:
    PushN8PhoneMag();
    ~PushN8PhoneMag();

    QString get_description();

    bool is_online();

private:
    QMagnetometer * magSensor;

protected:
    void timerEvent(QTimerEvent *);
};

#endif // PUSHN8PHONEMAG_H
