#ifndef PUSHN8AIRTIMEDETECTOR_H
#define PUSHN8AIRTIMEDETECTOR_H

#define AIR_TIME_THRESHOLD 220

#include "npushlogtick.h"

#include "npushimutick.h"
#include "npushacctick.h"
#include "npushnormfeettick.h"

#include "npushairtimetick.h"

#include "pushburtongenericdevice.h"

#include "AbsNormFeetTypes.h"

#include "pushn8absnormfeetdevice.h"
#include "pushn8phoneacc.h"
#include "pushn8imudevice.h"

#include "fuzzydetector.h"


#include "npushreportairtime.h"

class PushN8AirTimeDetector : public PushBurtonGenericDevice
{
    Q_OBJECT
public:
    PushN8AirTimeDetector();

    ~PushN8AirTimeDetector();

    bool subscribesToAny();
    bool subscribesTo(PushBurtonGenericDevice* deviceType);

    QString get_description();

    void start_readings();
    void stop_readings();

    bool is_online();

    SnowboradPressureData last_pdata;

    NPushReportAirTime * atReport;

protected:
    bool start_run();
    bool end_run();

private:

    void handle_response(Response);
    bool flying;

    FuzzyDetector fuzzyDetector;

    quint64 currentTstamp;
    quint64 takeOffTime;
    quint64 lastAirborneTime;

    double tRunStart;
    double tRunCurr;

    double fp,pa,ia;

public slots:
    void incomming_reading(NPushLogTick *);

};

#endif // PUSHN8AIRTIMEDETECTOR_H
