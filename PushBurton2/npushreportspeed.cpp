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

#include "npushreportspeed.h"

NPushReportSpeed::NPushReportSpeed() : graphPoints(100, -1.0)
{
    running = false;
    Speed_max = 0;
    Speed_avg = 0;

    Speed_acc = 0;
    Speed_cnt = 0;
}

NPushReportSpeed::~NPushReportSpeed()
{
}

bool NPushReportSpeed::start_run()
{
    QMutexLocker locker(&dataAccessMutex);

    if(running)
        return false;

    graphPoints.clear();

    Speed_max = 0.0;
    Speed_avg = 0.0;

    Speed_acc = 0.0;
    Speed_cnt = 0;

    graphPoints.clear();

    running = true;
    return true;
}

bool NPushReportSpeed::end_run()
{
    QMutexLocker locker(&dataAccessMutex);

    if(!running)
        return false;

    if(Speed_cnt > 0)
        Speed_avg = (double)(Speed_acc/(long double)Speed_cnt);
    else
        Speed_avg = 0;

    running = false;
    return true;
}

void NPushReportSpeed::append_new_reading(double speed)
{
    QMutexLocker locker(&dataAccessMutex);

    if(speed > Speed_max) {
        Speed_max = speed;
    }

    Speed_acc += speed;
    Speed_cnt++;

    graphPoints.push_back(speed);
}

bool NPushReportSpeed::save_to_dir(QString &dirName)
{

    QString reportFilename = (dirName + FSC_FSYS_SLASH) + "Speed.xml";
    QFile reportFile(reportFilename);

    reportFile.open(QFile::WriteOnly | QFile::Text);

    QXmlStreamWriter xml;

    xml.setDevice(&reportFile);
    xml.setAutoFormatting(true);

    xml.writeStartDocument();

    dataAccessMutex.lock();


    xml.writeStartElement("SpeedReport");

    xml.writeStartElement("stats");

    xml.writeAttribute("max", QString::number(Speed_max));
    xml.writeAttribute("avg", QString::number(Speed_avg));

    xml.writeEndElement();//stats

    xml.writeStartElement("graph");

    SelfShrinkingList::iterator it;
    for(it = graphPoints.begin(); it != graphPoints.end() ; ++it)
    {
        xml.writeStartElement("point");
        xml.writeAttribute("val", QString::number(*it));
        xml.writeEndElement();
    }

    xml.writeEndElement();//graph

    xml.writeEndElement();//SpeedReport
    dataAccessMutex.unlock();

    xml.writeEndDocument();

    reportFile.close();

    qDebug() << "Speed -> Ended";
    return true;
}

bool NPushReportSpeed::load_from_dir(QString &dirName)
{

    QString data_in_filename = (dirName + FSC_FSYS_SLASH) + "Speed.xml";
    QFile data_input(data_in_filename);
    data_input.open(QFile::ReadOnly | QFile::Text);

    QXmlStreamReader xml;
    xml.setDevice(&data_input);

    if(xml.readNextStartElement())
    {
        if(xml.name() != "SpeedReport") {
            qDebug() << "Invalid file format";
            return false;
        }
    } else {
        qDebug() << "Could not read first start Element";
        return false;
    }

    do
    {
        while(xml.readNextStartElement())
        {
            if(xml.name() == "stats")
            {
                Speed_max = xml.attributes().value("max").toString().toDouble();
                Speed_avg = xml.attributes().value("avg").toString().toDouble();
            } else if(xml.name() == "graph") {
                read_points_from_xml(xml);
            } else {
                xml.skipCurrentElement();
            }
        }
    }   while(xml.name() != "SpeedReport");

    return true;
//    qDebug() << "Done; " << Speed_max << "/" << Speed_avg << "; " << graphPoints.count() << "points";
}

void NPushReportSpeed::read_points_from_xml(QXmlStreamReader& xml)
{
    graphPoints.clear();

    do
    {
        while(xml.readNextStartElement()) {
            if(xml.name() == "point") {
                double val = xml.attributes().value("val").toString().toDouble();
                if(val != -1.0)
                    graphPoints.push_back(val);
            } else {
                xml.skipCurrentElement();
            }
        }
    } while(xml.name() != "graph");
}
