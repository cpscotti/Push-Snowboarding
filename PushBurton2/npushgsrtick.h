#ifndef NPUSHGSRTICK_H
#define NPUSHGSRTICK_H

#include "npushlogtick.h"

#include <QtXml>
#include <QTime>
#include <QString>

class NPushGSRTick : public NPushLogTick
{
public:
    NPushGSRTick(int a_reading);
    NPushGSRTick(int a_reading, quint64 a_msecsToEpoch);
    ~NPushGSRTick();

    void append_to_xml(QDomDocument& doc, QDomElement& root) const;
    void dump_to_xml(QXmlStreamWriter& xml) const;

    QString get_pretty_print() const;

    int reading;

    quint64 msecsToEpoch;

};

#endif // NPUSHGSRTICK_H
