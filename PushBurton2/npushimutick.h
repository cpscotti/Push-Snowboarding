#ifndef NPUSHIMUTICK_H
#define NPUSHIMUTICK_H

#include "npushlogtick.h"

#include <QtXml>
#include <QDateTime>

class NPushIMUTick : public NPushLogTick
{
public:
    NPushIMUTick(QStringList& values);
    NPushIMUTick(QStringList &values, quint64 a_msecsToEpoch);
    ~NPushIMUTick();

    void append_to_xml(QDomDocument& doc, QDomElement& root) const;
    void dump_to_xml(QXmlStreamWriter& xml) const;
    QString get_pretty_print() const;

    int gyro[3];
    int accel[3];
    int mag[3];

    quint64 msecsToEpoch;
};

#endif // NPUSHIMUTICK_H
