#ifndef ROTATIONSTICK_H
#define ROTATIONSTICK_H

#include <QtXml>
#include <QString>
#include <QObject>

#include "npushlogtick.h"
#include "math.h"

class NPushRotationsTick : public NPushLogTick
{
public:
    NPushRotationsTick(double a_rawDegs, quint64 a_msecsToEpoch);

    virtual ~NPushRotationsTick();

    virtual void read_from_xml( QXmlStreamReader& xml);
    virtual void dump_to_xml(QXmlStreamWriter& xml) const;

    virtual QString get_pretty_print() const;


public:
    quint64 msecsToEpoch;

    double rawDegs;
    double cleanDegs;

private:
    void updateCleanDegs();

};

#endif // ROTATIONSTICK_H
