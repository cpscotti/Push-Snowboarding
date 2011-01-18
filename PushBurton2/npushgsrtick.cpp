#include "npushgsrtick.h"

NPushGSRTick::NPushGSRTick(int a_reading)
{
    reading = a_reading;

    uint secs_to_epoch;
    uint msecs;

    secs_to_epoch = QDateTime::currentDateTime().toUTC().toTime_t();
    msecs = QTime::currentTime().msec();
    uint aft = QDateTime::currentDateTime().toUTC().toTime_t();

    //ugly work around, will be fized with msecsToEpoc from Qt 4.7
    if(aft != secs_to_epoch)//on a second slip
    {
        msecs = 0;
        secs_to_epoch = aft;
    }

    msecsToEpoch = (double)secs_to_epoch*1000.0+(double)msecs;
}

NPushGSRTick::NPushGSRTick(int a_reading, quint64 a_msecsToEpoch)
{
    reading = a_reading;
    msecsToEpoch = a_msecsToEpoch;
}

NPushGSRTick::~NPushGSRTick()
{

}

void NPushGSRTick::append_to_xml(QDomDocument& doc, QDomElement& root) const
{
    QDomElement tickDom = doc.createElement("gsr_data");
    tickDom.setAttribute("reading", QString::number(reading));
    tickDom.setAttribute("tstamp", (double)(msecsToEpoch*0.001));
    root.appendChild(tickDom);
}

void NPushGSRTick::dump_to_xml(QXmlStreamWriter& xml) const
{
    xml.writeStartElement("gsr_data");

    xml.writeAttribute("tstamp", QString::number(((double)msecsToEpoch)/(double)1000.0, 'f', 3));
    xml.writeAttribute("reading", QString::number(reading));

    xml.writeEndElement();//gsr_data
}

QString NPushGSRTick::get_pretty_print() const
{
    return QString("GSR: [%1]")
            .arg(QString::number(reading));
}
