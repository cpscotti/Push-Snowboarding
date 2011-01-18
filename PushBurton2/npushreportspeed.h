#ifndef NPUSHREPORTSPEED_H
#define NPUSHREPORTSPEED_H

#include "npushgenreport.h"
#include <QVector>
#include <QObject>
#include <QMutex>
#include <QMutexLocker>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>

#include "selfshrinkinglist.h"

class NPushReportSpeed : public NPushGenReport
{
    Q_OBJECT
public:
    NPushReportSpeed();
    ~NPushReportSpeed();

    bool save_to_dir(QString& dirName);
    bool load_from_dir(QString& dirName);

    bool start_run();
    bool end_run();

    void append_new_reading(double speed);

    bool running;

    double Speed_max;
    double Speed_avg;
    SelfShrinkingList graphPoints;

private:
    long double Speed_acc;
    long int Speed_cnt;

    void read_points_from_xml(QXmlStreamReader& xml);

    QMutex dataAccessMutex;

};

#endif // NPUSHREPORTSPEED_H
