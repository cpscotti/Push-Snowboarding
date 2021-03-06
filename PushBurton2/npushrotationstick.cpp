#include "npushrotationstick.h"

NPushRotationsTick::NPushRotationsTick(double a_rawDegs, quint64 a_msecsToEpoch)
{
    msecsToEpoch = a_msecsToEpoch;
    rawDegs = a_rawDegs;

    updateCleanDegs();
    qDebug() << "Landed a " << cleanDegs;
}

NPushRotationsTick::~NPushRotationsTick()
{
}

void NPushRotationsTick::read_from_xml( QXmlStreamReader& xml)
{
    Q_UNUSED(xml);
}

void NPushRotationsTick::dump_to_xml(QXmlStreamWriter& xml) const
{
    xml.writeStartElement("rot_count");

    xml.writeAttribute("tstamp", QString::number(((double)msecsToEpoch)/(double)1000.0, 'f', 3));
    xml.writeAttribute("clean_degrees", QString::number(cleanDegs, 'f', 1) );
    xml.writeAttribute("raw_degrees", QString::number(rawDegs, 'f', 3) );
    xml.writeEndElement();//rot_count
}

QString NPushRotationsTick::get_pretty_print() const
{
    return QString(("Spinning [%1]")).arg(QString::number(cleanDegs));
}

void NPushRotationsTick::updateCleanDegs()
{

//    if(rawDegs > 0) {
//        rawDegs *= 1.3;
//    } else {
//        rawDegs *= 1.4;
//    }

    if(fabs(rawDegs) > 90.0) {
        if(rawDegs > 0)
            cleanDegs = ceil(rawDegs/180.0);
        else
            cleanDegs = floor(rawDegs/180.0);
    } else {
        if(fabs(rawDegs) > 45.0) {
            if(rawDegs>0)
                cleanDegs = 0.5;
            else
                cleanDegs = -0.5;
        } else {
            cleanDegs = 0.0;
        }
    }

    cleanDegs *= 180;
}
