#ifndef PUSHN8PHONEGPS_H
#define PUSHN8PHONEGPS_H

#include <qmobilityglobal.h>
#include <QGeoPositionInfoSource>
#include <QDateTime>

#include "npushgpstick.h"

#include "pushburtongenericdevice.h"

#include <tz.h>
#include <TzLocalizer.h>
#include <TzLocalizationDataTypes.h>

QTM_BEGIN_NAMESPACE
class QGeoPositionInfo;
QTM_END_NAMESPACE

QTM_USE_NAMESPACE

class PushN8PhoneGPS : public PushBurtonGenericDevice
{
    Q_OBJECT
public:
    PushN8PhoneGPS(QObject * parent);

    ~PushN8PhoneGPS();

    QString get_description();

    bool is_online();

    virtual void start_readings();
    virtual void stop_readings();

public slots:
    void positionUpdated(QGeoPositionInfo);

private:

    void computeUTCOffset();

    QGeoPositionInfoSource * gpsSource;
    bool gpsOnline;
    int UTCOffset;

protected:
    void timerEvent(QTimerEvent *);
};

#endif // PUSHN8PHONEGPS_H
