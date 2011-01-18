#ifndef NPUSHREPORTHEART_H
#define NPUSHREPORTHEART_H

#include "npushgenreport.h"
#include <QVector>
#include <QObject>
#include <QMutex>
#include <QMutexLocker>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>

#include "selfshrinkinglist.h"

class NPushReportHeart : public NPushGenReport
{
    Q_OBJECT
public:
    NPushReportHeart();
    ~NPushReportHeart();

    bool save_to_dir(QString& dirName);
    bool load_from_dir(QString& dirName);

    bool start_run();
    bool end_run();

    void append_new_reading(int reading);

    bool running;

    int Heart_max;
    int Heart_avg;
    SelfShrinkingList graphPoints;

private:

    long int Heart_acc;
    int Heart_cnt;

    QMutex dataAccessMutex;

    void read_points_from_xml(QXmlStreamReader& xml);

    quint64 start;
    quint64 end;

};

#endif // NPUSHREPORTHEART_H
