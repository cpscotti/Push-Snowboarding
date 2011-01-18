#include "npushfoottick.h"

NPushFootTick::NPushFootTick(char a_side, int a_toes, int a_heel)
{
    side = a_side;
    toes = a_toes;
    heel = a_heel;

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

NPushFootTick::NPushFootTick(char a_side, int a_toes, int a_heel, quint64 a_msecsToEpoch)
{
    side = a_side;
    toes = a_toes;
    heel = a_heel;

    msecsToEpoch = a_msecsToEpoch;

}

NPushFootTick::~NPushFootTick()
{

}

void NPushFootTick::append_to_xml(QDomDocument& doc, QDomElement& root) const
{
    QDomElement tickDom = doc.createElement("foot_data");
    tickDom.setAttribute("side", QString(side));
    tickDom.setAttribute("toes", QString::number(toes));
    tickDom.setAttribute("heel", QString::number(heel));
    tickDom.setAttribute("tstamp", (double)(msecsToEpoch*0.001));

    root.appendChild(tickDom);
}

void NPushFootTick::dump_to_xml(QXmlStreamWriter& xml) const
{
    xml.writeStartElement("foot_data");
    xml.writeAttribute("tstamp", QString::number(((double)msecsToEpoch)/(double)1000.0, 'f', 3));

    xml.writeAttribute("side", QString(side));
    xml.writeAttribute("toes", QString::number(toes));
    xml.writeAttribute("heel", QString::number(heel));

    xml.writeEndElement();//foot_data
}

QString NPushFootTick::get_pretty_print() const
{
    return QString("Foot: [%1;%2;%3]")
            .arg(QString(side))
            .arg(QString::number(toes))
            .arg(QString::number(heel));
}
