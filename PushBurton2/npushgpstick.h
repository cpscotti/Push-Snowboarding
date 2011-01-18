#ifndef NPUSHGPSTICK_H
#define NPUSHGPSTICK_H

#include "npushlogtick.h"
#include <qmobilityglobal.h>
#include <QGeoPositionInfoSource>
#include <QDebug>
#include <QtXml>

QTM_BEGIN_NAMESPACE
class QGeoPositionInfo;
QTM_END_NAMESPACE

QTM_USE_NAMESPACE
class NPushGpsTick : public NPushLogTick
{
public:
    NPushGpsTick(const QGeoPositionInfo &info);
    ~NPushGpsTick();

    void append_to_xml(QDomDocument& doc, QDomElement& root) const;
    void dump_to_xml(QXmlStreamWriter& xml) const;

    QString get_pretty_print() const;

    QGeoPositionInfo tick;
};

#endif // NPUSHGPSTICK_H
