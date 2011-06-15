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

PushN8SimulationDevice::PushN8SimulationDevice(const QString& fname)
{
    simulationFile = fname;
    if(simulationFile == "")
        simulationFile = QFileDialog::getOpenFileName(0, "Select where to read the log from", FSC_RUNS_FOLDERS_ROOT, "Log Files (RawLog.xml)");

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
#ifdef Q_OS_LINUX
    timerPeriod = 0;
#else
    timerPeriod = 10;
#endif

}

PushN8SimulationDevice::~PushN8SimulationDevice()
{

    data_input->close();
    delete data_input;
}

QString PushN8SimulationDevice::getName()
{
    return QString("push.simulation");
}

bool PushN8SimulationDevice::is_online()
{
    return (data_input != NULL);
}

void PushN8SimulationDevice::timerEvent(QTimerEvent *)
{
    if(!xml.readNextStartElement()) {
        if(xml.isEndElement() && xml.name() == "N8SensorsLog") {
            qDebug() << "Simulation Ended!!";
            emit simulationEnded();
            this->stop_readings();
            return;
        }
    }

    NPushLogTick * newTick = 0;
    if(xml.name() == "acc_data")
    {
        newTick = new NPushAccTick();
        newTick->read_from_xml(xml);
    } else if(xml.name() == "mag_data")
    {
        newTick = new NPushMagTick();
        newTick->read_from_xml(xml);
    } else if(xml.name() == "gps_data")
    {
        newTick = new NPushGpsTick();
        newTick->read_from_xml(xml);
    } else if(xml.name() == "imu_data")
    {
        newTick = new NPushIMUTick();
        newTick->read_from_xml(xml);
    } else if(xml.name() == "gsr_data")
    {
        newTick = new NPushGSRTick();
        newTick->read_from_xml(xml);
    } else if(xml.name() == "heart_data")
    {
        newTick = new NPushHeartTick();
        newTick->read_from_xml(xml);
    } else if(xml.name() == "foot_data")
    {
        newTick = new NPushFootTick();
        newTick->read_from_xml(xml);
    } else {
        xml.skipCurrentElement();
    }

    //This ensures that the current tag is finalized
    if(!xml.isEndElement())
        xml.readNext();

    if(newTick != 0) {
        if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
        {
            emit reading_ready(newTick);
        } else {
            delete newTick;
        }
    }
}
