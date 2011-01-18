#ifndef NPUSHREPORTALTITUDE_H
#define NPUSHREPORTALTITUDE_H

#include "npushgenreport.h"
#include <QVector>
#include <QObject>
#include <QMutex>
#include <QMutexLocker>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>

#include "selfshrinkinglist.h"

class NPushReportAltitude : public NPushGenReport
{
    Q_OBJECT
public:
    NPushReportAltitude();
    ~NPushReportAltitude();

    bool save_to_dir(QString& dirName);
    bool load_from_dir(QString& dirName);

    bool start_run();
    bool end_run();

    void append_new_reading(double speed);

    bool running;

    SelfShrinkingList graphPoints;

    double Altitude_max;
    double Altitude_min;

private:

    void read_points_from_xml(QXmlStreamReader& xml);

    QMutex dataAccessMutex;
};

#endif // NPUSHREPORTALTITUDE_H
