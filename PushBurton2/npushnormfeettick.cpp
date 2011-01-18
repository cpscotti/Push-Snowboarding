#include "npushnormfeettick.h"

NPushNormFeetTick::NPushNormFeetTick(SnowboradPressureData a_pdata, quint64 a_msecsToEpoc)
    :msecsToEpoch(a_msecsToEpoc), pdata(a_pdata)

{
}


NPushNormFeetTick::~NPushNormFeetTick()
{

}

void NPushNormFeetTick::append_to_xml(QDomDocument& doc, QDomElement& root) const
{
    QDomElement tickDom = doc.createElement("feet_sum");
    tickDom.setAttribute("tstamp", (double)(msecsToEpoch*0.001));
    tickDom.setAttribute("sum", pdata.totalRawSum);

    root.appendChild(tickDom);
}

void NPushNormFeetTick::dump_to_xml(QXmlStreamWriter& xml) const
{
    xml.writeStartElement("feet_sum");

    xml.writeAttribute("tstamp", QString::number(((double)msecsToEpoch)/(double)1000.0, 'f', 3));
    xml.writeAttribute("sum", QString::number(pdata.totalRawSum));

    xml.writeEndElement();//feet_sum
}

QString NPushNormFeetTick::get_pretty_print() const
{
    return QString("Feet PSum [%1]")
            .arg(QString::number(pdata.totalRawSum));
}
