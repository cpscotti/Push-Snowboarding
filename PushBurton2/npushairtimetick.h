#ifndef NPUSHAIRTIMETICK_H
#define NPUSHAIRTIMETICK_H

#include "npushlogtick.h"

#include <QtXml>
#include <QTime>
#include <QString>

class NPushAirTimeTick : public NPushLogTick
{
public:
    NPushAirTimeTick(quint64 a_msecsInAir, bool a_landed, quint64 a_msecsToEpoch);
    ~NPushAirTimeTick();

    void append_to_xml(QDomDocument& doc, QDomElement& root) const;
    void dump_to_xml(QXmlStreamWriter& xml) const;
    QString get_pretty_print() const;

public:

    quint64 msecsToEpoch;
    quint64 msecsOnAir;
    bool landed;
};

#endif // NPUSHAIRTIMETICK_H
