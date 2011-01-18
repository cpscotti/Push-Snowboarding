#include "npushacctick.h"

NPushAccTick::NPushAccTick(QAccelerometerReading * reading)
{
    tick.copyValuesFrom(reading);

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

    pAccAbsMag = qSqrt(tick.x()*tick.x()+
                        tick.y()*tick.y()+
                        tick.z()*tick.z());
}

//Used to generate simulated ticks (tstamp comes directly from the xml file)
NPushAccTick::NPushAccTick(QAccelerometerReading * reading, quint64 a_msecsToEpoch)
{
    tick.copyValuesFrom(reading);
    msecsToEpoch = a_msecsToEpoch;

    pAccAbsMag = qSqrt(tick.x()*tick.x()+
                        tick.y()*tick.y()+
                        tick.z()*tick.z());
}

NPushAccTick::~NPushAccTick()
{
}

void NPushAccTick::append_to_xml(QDomDocument& doc, QDomElement& root) const
{
    QDomElement tickDom = doc.createElement("acc_data");
    tickDom.setAttribute("x", QString::number(tick.x()));
    tickDom.setAttribute("y", QString::number(tick.y()));
    tickDom.setAttribute("z", QString::number(tick.z()));
    tickDom.setAttribute("r", QString::number(pAccAbsMag));

    tickDom.setAttribute("tstamp", (double)(msecsToEpoch*0.001));

    if(pAccAbsMag < freefall_threshold)
    {
        tickDom.setAttribute("free_fall", "true");
    }

    root.appendChild(tickDom);
}

void NPushAccTick::dump_to_xml(QXmlStreamWriter& xml) const
{
    xml.writeStartElement("acc_data");

    xml.writeAttribute("tstamp", QString::number(((double)msecsToEpoch)/(double)1000.0, 'f', 3));

    xml.writeAttribute("x", QString::number(tick.x()));
    xml.writeAttribute("y", QString::number(tick.y()));
    xml.writeAttribute("z", QString::number(tick.z()));
    xml.writeAttribute("r", QString::number(pAccAbsMag));
    xml.writeEndElement();//acc_data
}

QString NPushAccTick::get_pretty_print() const
{
    return QString("P.Acc= %1;%2;%3")
            .arg(QString::number(tick.x()))
            .arg(QString::number(tick.y()))
            .arg(QString::number(tick.z()));
}

