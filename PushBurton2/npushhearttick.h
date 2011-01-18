#ifndef NPUSHHEARTTICK_H
#define NPUSHHEARTTICK_H

#include "npushlogtick.h"

#include <QtXml>
#include <QDateTime>

class NPushHeartTick : public NPushLogTick
{
public:
    NPushHeartTick(int a_hr_3, int a_hr_2, int a_hr_1);

    NPushHeartTick(int a_hr_3, int a_hr_2, int a_hr_1, quint64 a_msecsToEpoch);

    ~NPushHeartTick();

    void append_to_xml(QDomDocument& doc, QDomElement& root) const;
    void dump_to_xml(QXmlStreamWriter& xml) const;

    QString get_pretty_print() const;

    int hr_3;
    int hr_2;
    int hr_1;

    quint64 msecsToEpoch;
};

#endif // NPUSHHEARTTICK_H
