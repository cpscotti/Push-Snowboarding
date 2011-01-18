#ifndef NPUSHGENREPORT_H
#define NPUSHGENREPORT_H

#include <QObject>
#include <QString>
#include <QtXml>

class NPushGenReport : public QObject
{
public:
    virtual ~NPushGenReport() = 0;

    virtual bool save_to_dir(QString& dirName) = 0;
    virtual bool load_from_dir(QString& dirName) = 0;

    virtual bool start_run() = 0;
    virtual bool end_run() = 0;

    //virtual bool paint_report(QPainter, QPointF)

};

#endif // NPUSHGENREPORT_H
