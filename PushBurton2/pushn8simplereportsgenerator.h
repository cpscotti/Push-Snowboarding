#ifndef PUSHN8SIMPLEREPORTSGENERATOR_H
#define PUSHN8SIMPLEREPORTSGENERATOR_H

#include "pushburtongenericdevice.h"

//Subscribes to
#include "pushn8gsrdevice.h"
#include "pushn8heartdevice.h"
#include "pushn8phonegps.h"

//Inputs
#include "npushgsrtick.h"
#include "npushhearttick.h"
#include "npushgpstick.h"

//Reports
#include "npushreportgsr.h"
#include "npushreportheart.h"
#include "npushreportspeed.h"
#include "npushreportaltitude.h"

class PushN8SimpleReportsGenerator : public PushBurtonGenericDevice
{
    Q_OBJECT
public:
    PushN8SimpleReportsGenerator();
    ~PushN8SimpleReportsGenerator();

    bool subscribesToAny();
    bool subscribesTo(PushBurtonGenericDevice* deviceType);

    QString get_description();

    void start_readings();
    void stop_readings();

    bool is_online();


    NPushReportGSR * gsrReport;
    NPushReportHeart * heartReport;
    NPushReportSpeed * speedReport;
    NPushReportAltitude * altitudeReport;

protected:
    bool start_run();
    bool end_run();

public slots:
    void incomming_reading(NPushLogTick *);


};

#endif // PUSHN8SIMPLEREPORTSGENERATOR_H
