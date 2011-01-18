#include "npushhearttick.h"

NPushHeartTick::NPushHeartTick(int a_hr_3, int a_hr_2, int a_hr_1)
{

    hr_3 = a_hr_3;
    hr_2 = a_hr_2;
    hr_1 = a_hr_1;

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

NPushHeartTick::NPushHeartTick(int a_hr_3, int a_hr_2, int a_hr_1, quint64 a_msecsToEpoch)
{
    hr_3 = a_hr_3;
    hr_2 = a_hr_2;
    hr_1 = a_hr_1;

    msecsToEpoch = a_msecsToEpoch;
}

NPushHeartTick::~NPushHeartTick()
{
}

void NPushHeartTick::append_to_xml(QDomDocument& doc, QDomElement& root) const
{
    QDomElement tickDom = doc.createElement("heart_data");
    tickDom.setAttribute("last32", QString::number(hr_3));
    tickDom.setAttribute("last16", QString::number(hr_2));
    tickDom.setAttribute("lastBeat", QString::number(hr_1));

    tickDom.setAttribute("tstamp", (double)(msecsToEpoch*0.001));

    root.appendChild(tickDom);
}

void NPushHeartTick::dump_to_xml(QXmlStreamWriter& xml) const
{
    xml.writeStartElement("heart_data");
    xml.writeAttribute("tstamp", QString::number(((double)msecsToEpoch)/(double)1000.0, 'f', 3));

    xml.writeAttribute("last32", QString::number(hr_3));
    xml.writeAttribute("last16", QString::number(hr_2));
    xml.writeAttribute("lastBeat", QString::number(hr_1));

    xml.writeEndElement();//heart_data
}

QString NPushHeartTick::get_pretty_print() const
{
    return QString("Heart= %1")
            .arg(QString::number(hr_1));
}
