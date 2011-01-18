#ifndef NPUSHREPORTGSR_H
#define NPUSHREPORTGSR_H

#include "npushgenreport.h"
#include <QVector>
#include <QObject>
#include <QMutex>
#include <QMutexLocker>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>

#include "selfshrinkinglist.h"

class NPushReportGSR : public NPushGenReport
{
    Q_OBJECT
public:
    NPushReportGSR();

    ~NPushReportGSR();

    bool save_to_dir(QString& dirName);
    bool load_from_dir(QString& dirName);

    bool start_run();
    bool end_run();

    void append_new_reading(int reading);

    bool running;

    SelfShrinkingList graphPoints;

    double GSR_max;
    double GSR_avg;
//    int GSR_min;

private:

    long int GSR_acc;
    int GSR_cnt;

    void read_points_from_xml(QXmlStreamReader& xml);

    QMutex dataAccessMutex;


};

#endif // NPUSHREPORTGSR_H
