#ifndef PUSHBURTONGENERICDEVICE_H
#define PUSHBURTONGENERICDEVICE_H



#include <QObject>
#include <QString>
#include <QVector>
#include "npushlogtick.h"
#include "npushgenreport.h"

class PushBurtonGenericDevice : public QObject
{
    Q_OBJECT
public:

    PushBurtonGenericDevice(QObject * parent = 0);

    virtual ~PushBurtonGenericDevice() = 0;

    virtual QString get_description() = 0;

    virtual void start_readings();
    virtual void stop_readings();

    virtual bool is_online() = 0;

    virtual void disconnect_from_backend();

    virtual bool start_run();
    virtual bool end_run();

    virtual bool subscribesToAny();
    virtual bool subscribesTo(PushBurtonGenericDevice* deviceType);

protected:
    int timerId;
    int timerPeriod;



public slots:
    virtual void incomming_reading(NPushLogTick *);

signals:
    void reading_ready(NPushLogTick *);

    void report_ready(NPushGenReport *);

    void connected();
};

#endif // PUSHBURTONGENERICDEVICE_H
