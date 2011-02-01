/*
 * Copyright (c) 2011 Nokia Corporation
 *
 * This file is part of the Push Snowboarding Project, More info at:
 * www.pushsnowboading.com
 *
 * Author: Clovis Scotti <scotti@ieee.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */

#include "pushn8simulationdevice.h"

PushN8SimulationDevice::PushN8SimulationDevice()
{

    QString simulationFile = QFileDialog::getOpenFileName(0, "Select where to save log", "E:\\", "Log Files (*.xml)");
    if(simulationFile == "") {
        simulationFile = "E:/sample_input.xml";
    }
    qDebug() << "Simulation input stream is: " << simulationFile;

    data_input = new QFile(simulationFile, this);

    data_input->open(QFile::ReadOnly | QFile::Text);

    if(data_input)
        xml.setDevice(data_input);

    if(xml.readNextStartElement()) {
        if(xml.name() == "N8SensorsLog") {
            qDebug() << "Format is ok";
        } else {
            qDebug() << "Format is not ok";
        }
    } else {
        qDebug() << "Could not read first start Element";
    }

    timerId = 0;
    timerPeriod = 20;
}

PushN8SimulationDevice::~PushN8SimulationDevice()
{

    data_input->close();
    delete data_input;
}

QString PushN8SimulationDevice::get_description()
{
    return QString("Devices Simulation Stream");
}

bool PushN8SimulationDevice::is_online()
{
    return (data_input != NULL);
}

void PushN8SimulationDevice::timerEvent(QTimerEvent * event)
{
    bool reachedEnd = false;
    while(xml.readNextStartElement())
    {
        NPushLogTick * newTick = 0;
        if(xml.name() == "acc_data")
        {
            newTick = readAccTick();
        } else if(xml.name() == "mag_data")
        {
            newTick = readMagTick();
        } else if(xml.name() == "gps_data")
        {
            newTick = readGpsTick();
        } else if(xml.name() == "imu_data")
        {
            newTick = readImuTick();
            //Imu tick has inner elements, when it returns we can quit
            reachedEnd = true;
        } else if(xml.name() == "gsr_data")
        {
            newTick = readGsrTick();
        } else if(xml.name() == "heart_data")
        {
            newTick = readHeartTick();
        } else if(xml.name() == "foot_data")
        {
            newTick = readFootTick();
        } else {
//            qDebug() << "Unrecognized tag " << xml.name();
            xml.skipCurrentElement();
        }

        if(newTick != 0) {
            if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
            {
                emit reading_ready(newTick);
            } else {
                delete newTick;
            }
        }

        if(reachedEnd)
            break;
    }

    if(xml.isEndElement() && xml.name() == "N8SensorsLog")
        qDebug() << "Simulation Ended!!";
}

NPushAccTick * PushN8SimulationDevice::readAccTick()
{
    QAccelerometerReading fake;

    fake.setX(xml.attributes().value("x").toString().toFloat());
    fake.setY(xml.attributes().value("y").toString().toFloat());
    fake.setZ(xml.attributes().value("z").toString().toFloat());
    quint64 tstamp = (quint64)((double)xml.attributes().value("tstamp").toString().toDouble()*1000.0);
    NPushAccTick * tick = new NPushAccTick(&fake, tstamp);
    return tick;
}

NPushFootTick * PushN8SimulationDevice::readFootTick()
{

    QChar side = xml.attributes().value("side").toString().at(0);
    int heel = xml.attributes().value("heel").toString().toInt();
    int toes = xml.attributes().value("toes").toString().toInt();

    quint64 tstamp = (quint64)((double)xml.attributes().value("tstamp").toString().toDouble()*1000.0);

    NPushFootTick * tick = new NPushFootTick(side.toAscii(), toes, heel, tstamp);
    return tick;
}

NPushGpsTick * PushN8SimulationDevice::readGpsTick()
{
//    <gps_data tstamp="1291460032" longitude="11.2806" ground_speed="5.85" latitude="47.1801" altitude="2360" />

    QGeoPositionInfo fake;
    QDateTime tstamp;
    QGeoCoordinate fakeCoord;
    fakeCoord.setAltitude(xml.attributes().value("altitude").toString().toDouble());
    fakeCoord.setLatitude(xml.attributes().value("latitude").toString().toDouble());
    fakeCoord.setLongitude(xml.attributes().value("longitude").toString().toDouble());

    tstamp = QDateTime::fromTime_t(floor(xml.attributes().value("tstamp").toString().toDouble()));//-3600.0));

    fake.setTimestamp(tstamp);
    fake.setCoordinate(fakeCoord);

    fake.setAttribute(QGeoPositionInfo::GroundSpeed, xml.attributes().value("ground_speed").toString().toFloat());

    NPushGpsTick * tick =  new NPushGpsTick(fake);

    return tick;
}

NPushGSRTick * PushN8SimulationDevice::readGsrTick()
{
    int reading = xml.attributes().value("reading").toString().toInt();

    quint64 tstamp = (quint64)((double)xml.attributes().value("tstamp").toString().toDouble()*1000.0);

    NPushGSRTick * tick = new NPushGSRTick(reading, tstamp);
    return tick;
}

NPushHeartTick * PushN8SimulationDevice::readHeartTick()
{
    int last32 = xml.attributes().value("last32").toString().toInt();
    int last16 = xml.attributes().value("last16").toString().toInt();
    int lastBeat = xml.attributes().value("lastBeat").toString().toInt();

    quint64 tstamp = (quint64)((double)xml.attributes().value("tstamp").toString().toDouble()*1000.0);

    NPushHeartTick * tick = new NPushHeartTick(last32, last16, lastBeat, tstamp);
    return tick;
}

NPushIMUTick * PushN8SimulationDevice::readImuTick()
{
    QStringList values;
    values.push_back("IMU");

    quint64 tstamp = (quint64)((double)xml.attributes().value("tstamp").toString().toDouble()*1000.0);

    while(xml.readNextStartElement()) {
        if(xml.name() == "acc") {
            values.append(xml.readElementText().split(","));
        } else if(xml.name() == "gyro") {
            values.append(xml.readElementText().split(","));
        } else if(xml.name() == "mag") {
            values.append(xml.readElementText().split(","));
        } else {
            xml.skipCurrentElement();
        }
    }
    values.push_back("END");

    NPushIMUTick * tick = new NPushIMUTick(values, tstamp);
    return tick;
}

NPushMagTick * PushN8SimulationDevice::readMagTick()
{
    QMagnetometerReading fake;
    fake.setX(xml.attributes().value("x").toString().toFloat()*0.001);
    fake.setY(xml.attributes().value("y").toString().toFloat()*0.001);
    fake.setZ(xml.attributes().value("z").toString().toFloat()*0.001);

    quint64 tstamp = (quint64)((double)xml.attributes().value("tstamp").toString().toDouble()*1000.0);

    NPushMagTick * tick = new NPushMagTick(&fake, tstamp);
    return tick;
}
