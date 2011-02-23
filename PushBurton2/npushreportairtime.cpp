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

#include "npushreportairtime.h"

NPushReportAirTime::NPushReportAirTime()
{
    running = false;
    n_of_airs = 0;
}

NPushReportAirTime::~NPushReportAirTime()
{
    dataAccessMutex.lock();//locks forever so noone "tryes" to go into dead data
}


void NPushReportAirTime::append_air(quint64 _air_time, quint64 _tstamp)
{
    QMutexLocker locker(&dataAccessMutex);
    if(!running)
        return;

    double dairTime = _air_time/1000.0;
    double dtstamp = _tstamp/1000.0;

    if(dairTime > longest_air_time)
        longest_air_time = dairTime;

    total_air_time += dairTime;

    air_times.push_back(dairTime);
    tstamps.push_back(dtstamp);
    n_of_airs++;
}

bool NPushReportAirTime::start_run()
{
    QMutexLocker locker(&dataAccessMutex);

    if(running)
        return false;

    running = true;
    longest_air_time = 0;
    total_air_time = 0;
    n_of_airs = 0;

    air_times.clear();
    tstamps.clear();

    return true;
}

bool NPushReportAirTime::end_run()
{
    QMutexLocker locker(&dataAccessMutex);

    if(!running)
        return false;

    running = false;
    return true;
}


bool NPushReportAirTime::save_to_dir(QString& dirName)
{
    QString reportFilename = (dirName + FSC_FSYS_SLASH) + "AirTime.xml";
    QFile reportFile(reportFilename);

    qDebug() << "reportFilename = " << reportFilename;
    qDebug() << "slash is = " << FSC_FSYS_SLASH;

    reportFile.open(QFile::WriteOnly | QFile::Text);

    QXmlStreamWriter xml;

    xml.setDevice(&reportFile);
    xml.setAutoFormatting(true);

    xml.writeStartDocument();

    dataAccessMutex.lock();


    xml.writeStartElement("AirTimeReport");

    xml.writeStartElement("stats");

    xml.writeAttribute("n_of_airs", QString::number(air_times.count()));
    xml.writeAttribute("biggest_air", QString::number(longest_air_time, 'f', 3));
    xml.writeAttribute("total_air", QString::number(total_air_time, 'f', 3));
    xml.writeEndElement();//stats

    xml.writeStartElement("watch");
    xml.writeAttribute("run_start", QString::number(tRunStart, 'f', 3));
    xml.writeAttribute("run_end", QString::number(tRunEnd, 'f', 3));
    xml.writeEndElement();

    if(air_times.count() != tstamps.count()) {
        xml.writeComment("Corrupted air_times vs tstamps");
    } else {

        xml.writeStartElement("bar_graph");
        QList<double>::iterator it;
        QList<double>::iterator tsit;

        tsit = tstamps.begin();
        for(it = air_times.begin();it != air_times.end(); ++it) {
            xml.writeStartElement("air");

            xml.writeAttribute("air_time", QString::number(*it, 'f', 3));
            xml.writeAttribute("tstamp", QString::number(*tsit, 'f', 3));

            xml.writeEndElement();
            ++tsit;
            if(tsit == tstamps.end())
                break;
        }

        xml.writeEndElement();//bar_graph

    }

    xml.writeEndElement();//AirTimeReport
    dataAccessMutex.unlock();

    xml.writeEndDocument();

    reportFile.close();
    return true;
}

bool NPushReportAirTime::load_from_dir(QString& dirName)
{

    QString data_in_filename = (dirName + FSC_FSYS_SLASH) + "AirTime.xml";

    QFile data_input(data_in_filename);
    data_input.open(QFile::ReadOnly | QFile::Text);

    QXmlStreamReader xml;
    xml.setDevice(&data_input);

    if(xml.readNextStartElement())
    {
        if(xml.name() != "AirTimeReport") {
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
                n_of_airs = xml.attributes().value("n_of_airs").toString().toInt();
                longest_air_time = xml.attributes().value("biggest_air").toString().toDouble();
                total_air_time = xml.attributes().value("total_air").toString().toDouble();
            } else if(xml.name() == "bar_graph") {
                read_bar_graph_from_xml(xml);
            } else if(xml.name() == "watch") {
                tRunStart = xml.attributes().value("run_start").toString().toDouble();
                tRunEnd = xml.attributes().value("run_end").toString().toDouble();
            } else {
                xml.skipCurrentElement();
            }
        }
    }   while(xml.name() != "AirTimeReport");
    return true;
}

void NPushReportAirTime::read_bar_graph_from_xml(QXmlStreamReader& xml)
{
    air_times.clear();
    tstamps.clear();

    do
    {
        while(xml.readNextStartElement()) {
            if(xml.name() == "air") {
                air_times.push_back(xml.attributes().value("air_time").toString().toDouble());
                tstamps.push_back(xml.attributes().value("tstamp").toString().toDouble());
            } else {
                xml.skipCurrentElement();
            }
        }
    } while(xml.name() != "bar_graph");
}


void NPushReportAirTime::set_start_tstamp(double t)
{
    tRunStart = t;
}

void NPushReportAirTime::set_end_tstamp(double t)
{
    tRunEnd = t;
}
