#ifndef NPUSHNORMFEETTICK_H
#define NPUSHNORMFEETTICK_H

#include "npushlogtick.h"

#include "AbsNormFeetTypes.h"

class NPushNormFeetTick : public NPushLogTick
{
public:
    NPushNormFeetTick(SnowboradPressureData, quint64);
    ~NPushNormFeetTick();


    void append_to_xml(QDomDocument& doc, QDomElement& root) const;
    void dump_to_xml(QXmlStreamWriter& xml) const;

    QString get_pretty_print() const;

public:

    quint64 msecsToEpoch;
    SnowboradPressureData pdata;
};

#endif // NPUSHNORMFEETTICK_H
