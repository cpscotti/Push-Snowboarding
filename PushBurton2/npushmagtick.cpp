#include "npushmagtick.h"

NPushMagTick::NPushMagTick(QMagnetometerReading * reading)
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
}

NPushMagTick::NPushMagTick(QMagnetometerReading *reading, quint64 a_msecsToEpoch)
{
    tick.copyValuesFrom(reading);

    msecsToEpoch = a_msecsToEpoch;
}

NPushMagTick::~NPushMagTick()
{
}

void NPushMagTick::append_to_xml(QDomDocument& doc, QDomElement& root) const
{
    QDomElement tickDom = doc.createElement("mag_data");
    tickDom.setAttribute("x", QString::number(tick.x()*1000.0,'f',5));
    tickDom.setAttribute("y", QString::number(tick.y()*1000.0,'f',5));
    tickDom.setAttribute("z", QString::number(tick.z()*1000.0,'f',5));
    tickDom.setAttribute("r", QString::number(qSqrt(tick.x()*tick.x()+
                                                    tick.y()*tick.y()+
                                                    tick.z()*tick.z())*1000.0,'f',5));
    tickDom.setAttribute("tstamp", (double)(msecsToEpoch*0.001));

    root.appendChild(tickDom);
}

void NPushMagTick::dump_to_xml(QXmlStreamWriter& xml) const
{
    xml.writeStartElement("mag_data");

    xml.writeAttribute("tstamp", QString::number(((double)msecsToEpoch)/(double)1000.0, 'f', 3));

    xml.writeAttribute("x", QString::number(tick.x()*1000.0,'f',5));
    xml.writeAttribute("y", QString::number(tick.y()*1000.0,'f',5));
    xml.writeAttribute("z", QString::number(tick.z()*1000.0,'f',5));
    xml.writeAttribute("r", QString::number(qSqrt(tick.x()*tick.x()+
                                                    tick.y()*tick.y()+
                                                    tick.z()*tick.z())*1000.0,'f',5));
    xml.writeEndElement();//mag_data
}

QString NPushMagTick::get_pretty_print() const
{
    return QString("Magnetometer Update: [x;y;z]mPhi = [%1;%2;%3]")
            .arg(QString::number(tick.x()*1000.0,'f',4))
            .arg(QString::number(tick.y()*1000.0,'f',4))
            .arg(QString::number(tick.z()*1000.0,'f',4));
}

