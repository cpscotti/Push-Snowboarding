#include "npushimutick.h"

NPushIMUTick::NPushIMUTick(QStringList& values)
{
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

    if(values.count() != 11)
    {
        qDebug() << "EARLIER PARSING ERROR!";
        return;
    }

    for(int i=1;i<values.count();i++)
    {
        if(i < 4)
            gyro[i-1] = values[i].toInt();
        else if(i < 7)
            accel[i-4] = values[i].toInt();
        else if(i < 10)
            mag[i-7] = values[i].toInt();
    }

    msecsToEpoch = (quint64)((double)secs_to_epoch*1000.0+(double)msecs);

//    NPushIMUTick(values, );
}


NPushIMUTick::NPushIMUTick(QStringList &values, quint64 a_msecsToEpoch)
{
    if(values.count() != 11)
    {
        qDebug() << "EARLIER PARSING ERROR!";
        return;
    }

    for(int i=1;i<values.count();i++)
    {
        if(i < 4)
            gyro[i-1] = values[i].toInt();
        else if(i < 7)
            accel[i-4] = values[i].toInt();
        else if(i < 10)
            mag[i-7] = values[i].toInt();
    }

    msecsToEpoch = a_msecsToEpoch;
}

NPushIMUTick::~NPushIMUTick()
{

}

void NPushIMUTick::append_to_xml(QDomDocument& doc, QDomElement& root) const
{
    QDomElement tickDom = doc.createElement("imu_data");

    QDomNode gyroNode = doc.createElement("gyro");
    QDomText gyroText = doc.createTextNode(QString("%1,%2,%3")
                                           .arg(QString::number(gyro[0]))
                                           .arg(QString::number(gyro[1]))
                                           .arg(QString::number(gyro[2])));
    gyroNode.appendChild(gyroText);
    tickDom.appendChild(gyroNode);

    QDomNode accNode = doc.createElement("acc");
    QDomText accText = doc.createTextNode(QString("%1,%2,%3")
                                          .arg(QString::number(accel[0]))
                                          .arg(QString::number(accel[1]))
                                          .arg(QString::number(accel[2])));

    accNode.appendChild(accText);
    tickDom.appendChild(accNode);

    QDomNode magNode = doc.createElement("mag");
    QDomText magText = doc.createTextNode(QString("%1,%2,%3")
                                          .arg(QString::number(mag[0]))
                                          .arg(QString::number(mag[1]))
                                          .arg(QString::number(mag[2])));

    magNode.appendChild(magText);
    tickDom.appendChild(magNode);

    tickDom.setAttribute("tstamp", (double)(msecsToEpoch*0.001));
    root.appendChild(tickDom);
}

void NPushIMUTick::dump_to_xml(QXmlStreamWriter& xml) const
{
    xml.writeStartElement("imu_data");

    xml.writeAttribute("tstamp", QString::number(((double)msecsToEpoch)/(double)1000.0, 'f', 3));

        xml.writeStartElement("gyro");
        xml.writeCharacters(QString("%1,%2,%3")
                            .arg(QString::number(gyro[0]))
                            .arg(QString::number(gyro[1]))
                            .arg(QString::number(gyro[2])));
        xml.writeEndElement();

        xml.writeStartElement("acc");
        xml.writeCharacters(QString("%1,%2,%3")
                            .arg(QString::number(accel[0]))
                            .arg(QString::number(accel[1]))
                            .arg(QString::number(accel[2])));
        xml.writeEndElement();

        xml.writeStartElement("mag");
        xml.writeCharacters(QString("%1,%2,%3")
                            .arg(QString::number(mag[0]))
                            .arg(QString::number(mag[1]))
                            .arg(QString::number(mag[2])));
        xml.writeEndElement();

    xml.writeEndElement();//imu_data
}


QString NPushIMUTick::get_pretty_print() const
{
    return QString("GYRO: %1,%2,%3\nACCEL: %4,%5,%6\nMAG: %7,%8,%9")
            .arg(QString::number(gyro[0]))
            .arg(QString::number(gyro[1]))
            .arg(QString::number(gyro[2]))
            .arg(QString::number(accel[0]))
            .arg(QString::number(accel[1]))
            .arg(QString::number(accel[2]))
            .arg(QString::number(mag[0]))
            .arg(QString::number(mag[1]))
            .arg(QString::number(mag[2]));
}
