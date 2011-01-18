#ifndef NPUSHACCTICK_H
#define NPUSHACCTICK_H

#include "npushlogtick.h"
#include <qmath.h>
#include <QAccelerometer>
#include <QtXml>
#include <QTime>

QTM_USE_NAMESPACE
class NPushAccTick : public NPushLogTick
{
public:
    NPushAccTick(QAccelerometerReading * reading);
    NPushAccTick(QAccelerometerReading * reading, quint64 a_msecsToEpoch);
    ~NPushAccTick();

    void append_to_xml(QDomDocument& doc, QDomElement& root) const;
    void dump_to_xml(QXmlStreamWriter& xml) const;

    QString get_pretty_print() const;

    QAccelerometerReading tick;
    qreal pAccAbsMag;

    quint64 msecsToEpoch;

    static const qreal freefall_threshold = 7.0;
};

#endif // NPUSHACCTICK_H
