#include "npushairtimetick.h"

NPushAirTimeTick::NPushAirTimeTick(quint64 a_msecsInAir, bool a_landed, quint64 a_msecsToEpoch) :
        msecsOnAir(a_msecsInAir), landed(a_landed), msecsToEpoch(a_msecsToEpoch)
{
}


NPushAirTimeTick::~NPushAirTimeTick()
{
}


void NPushAirTimeTick::append_to_xml(QDomDocument& doc, QDomElement& root) const
{
    QDomElement tickDom = doc.createElement("air_time");
    tickDom.setAttribute("tstamp", (double)(msecsToEpoch*0.001));
    tickDom.setAttribute("in_air", (double)(msecsOnAir*0.001));
    tickDom.setAttribute("landed", ((landed)?"true":"false") );

    root.appendChild(tickDom);
}

void NPushAirTimeTick::dump_to_xml(QXmlStreamWriter& xml) const
{
    xml.writeStartElement("air_time");

    xml.writeAttribute("tstamp", QString::number(((double)msecsToEpoch)/(double)1000.0, 'f', 3));
    xml.writeAttribute("in_air", QString::number(((double)msecsOnAir)/(double)1000.0, 'f', 3));
    xml.writeAttribute("landed", ((landed)?"true":"false") );
    xml.writeEndElement();//air_time
}

QString NPushAirTimeTick::get_pretty_print() const
{
    return QString(((landed)?"Landed [%1]":"AIR [%1]"))
            .arg(QString::number(msecsOnAir));
}
