#ifndef NPUSHREPORTAIRTIME_H
#define NPUSHREPORTAIRTIME_H

#include "npushgenreport.h"
#include <QList>
#include <QObject>
#include <QMutex>
#include <QMutexLocker>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>

class NPushReportAirTime : public NPushGenReport
{
    Q_OBJECT
public:
    NPushReportAirTime();
    ~NPushReportAirTime();

    void append_air(quint64 air_time, quint64 tstamp);

    bool save_to_dir(QString& dirName);
    bool load_from_dir(QString& dirName);

    bool start_run();
    bool end_run();

    void set_start_tstamp(double t);
    void set_end_tstamp(double t);

    bool running;

    //All values in SECONDS/and/or/fractions of seconds
    double longest_air_time;
    double total_air_time;
    int n_of_airs;

    //start-end timestamps
    double tRunStart;
    double tRunEnd;

    QList<double> air_times;
    QList<double> tstamps;


private:

    void read_bar_graph_from_xml(QXmlStreamReader& xml);

    QMutex dataAccessMutex;
};

#endif // NPUSHREPORTAIRTIME_H
