#include "npushgpstick.h"

NPushGpsTick::NPushGpsTick(const QGeoPositionInfo &info)
{
    tick = info;
    QDateTime correctedTstamp;
    correctedTstamp = tick.timestamp().addSecs(3600);
    tick.setTimestamp(correctedTstamp);
}

NPushGpsTick::~NPushGpsTick()
{
}

void NPushGpsTick::append_to_xml(QDomDocument& doc, QDomElement& root) const
{
    QDomElement tickDom = doc.createElement("gps_data");
    tickDom.setAttribute("tstamp", tick.timestamp().toUTC().toTime_t());//TODO... Only works in Switzerland!
    tickDom.setAttribute("longitude", QString::number(tick.coordinate().longitude()));
    tickDom.setAttribute("latitude", QString::number(tick.coordinate().latitude()));
    tickDom.setAttribute("altitude", QString::number(tick.coordinate().altitude()));
    tickDom.setAttribute("ground_speed", QString::number(tick.attribute(QGeoPositionInfo::GroundSpeed)));
    root.appendChild(tickDom);
}

void NPushGpsTick::dump_to_xml(QXmlStreamWriter& xml) const
{
    xml.writeStartElement("gps_data");
    xml.writeAttribute("tstamp", QString::number(tick.timestamp().toUTC().toTime_t()));//TODO... LATER!Only works in Switzerland
    xml.writeAttribute("longitude", QString::number(tick.coordinate().longitude()));
    xml.writeAttribute("latitude", QString::number(tick.coordinate().latitude()));
    xml.writeAttribute("altitude", QString::number(tick.coordinate().altitude()));
    xml.writeAttribute("ground_speed", QString::number(tick.attribute(QGeoPositionInfo::GroundSpeed)));

    xml.writeEndElement();//gps_data
}

QString NPushGpsTick::get_pretty_print() const
{
    return QString("Speed = %1, Alt = %2").arg(QString::number(tick.attribute(QGeoPositionInfo::GroundSpeed)*3.6)).arg(QString::number(tick.coordinate().altitude()));
}
