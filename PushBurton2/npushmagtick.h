#ifndef NPUSHMAGTICK_H
#define NPUSHMAGTICK_H

#include "npushlogtick.h"
#include <QMagnetometer>
#include <QtXml>
#include <QDateTime>


QTM_USE_NAMESPACE
class NPushMagTick : public NPushLogTick
{
public:
    NPushMagTick(QMagnetometerReading * reading);
    NPushMagTick(QMagnetometerReading * reading, quint64 a_msecsToEpoch);
    ~NPushMagTick();

    void append_to_xml(QDomDocument& doc, QDomElement& root) const;
    void dump_to_xml(QXmlStreamWriter& xml) const;

    QString get_pretty_print() const;


    QMagnetometerReading tick;

    quint64 msecsToEpoch;
};

#endif // NPUSHMAGTICK_H
