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

#include "npushreportaltitude.h"

NPushReportAltitude::NPushReportAltitude() : graphPoints(100, 0.0)
{
    Altitude_max = 0.0;
    Altitude_min = 0.0;

    running = false;
}

NPushReportAltitude::~NPushReportAltitude()
{

}

bool NPushReportAltitude::start_run()
{
    QMutexLocker locker(&dataAccessMutex);

    if(running)
        return false;

    graphPoints.clear();

    Altitude_max = 0.0;
    Altitude_min = 1000000.0;//Everest is 10,000 so...1,000,000 should be pretty ok!

    running = true;
    return true;
}

bool NPushReportAltitude::end_run()
{
    QMutexLocker locker(&dataAccessMutex);

    if(!running)
        return false;

    running = false;
    return true;
}

void NPushReportAltitude::append_new_reading(double altitude)
{
    QMutexLocker locker(&dataAccessMutex);

    if(altitude == 0.0) {
        qDebug() << "Altitude == 0";
    }

    if(altitude > Altitude_max) {
        Altitude_max = altitude;
    }

    if(altitude < Altitude_min) {
        Altitude_min = altitude;
    }

    graphPoints.push_back(altitude);
}

bool NPushReportAltitude::save_to_dir(QString &dirName)
{

    QString reportFilename = (dirName + FSC_FSYS_SLASH) + "Altitude.xml";
    QFile reportFile(reportFilename);

    reportFile.open(QFile::WriteOnly | QFile::Text);

    qDebug() << "Altitude -> Saving to dir";

    QXmlStreamWriter xml;

    xml.setDevice(&reportFile);
    xml.setAutoFormatting(true);

    xml.writeStartDocument();

    dataAccessMutex.lock();


    xml.writeStartElement("AltitudeReport");

    xml.writeStartElement("stats");

    xml.writeAttribute("max", QString::number(Altitude_max));
    xml.writeAttribute("min", QString::number(Altitude_min));

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

    xml.writeEndElement();//AltitudeReport
    dataAccessMutex.unlock();

    xml.writeEndDocument();

    reportFile.close();

    qDebug() << "Altitude -> Ended";
    return true;
}

bool NPushReportAltitude::load_from_dir(QString &dirName)
{

    QString data_in_filename = (dirName + FSC_FSYS_SLASH) + "Altitude.xml";

    QFile data_input(data_in_filename);
    data_input.open(QFile::ReadOnly | QFile::Text);

    QXmlStreamReader xml;
    xml.setDevice(&data_input);

    if(xml.readNextStartElement())
    {
        if(xml.name() != "AltitudeReport") {
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
                Altitude_max = xml.attributes().value("max").toString().toDouble();
                Altitude_min = xml.attributes().value("min").toString().toDouble();
            } else if(xml.name() == "graph") {
                read_points_from_xml(xml);
            } else {
                xml.skipCurrentElement();
            }
        }
    }   while(xml.name() != "AltitudeReport");
    return true;
}

void NPushReportAltitude::read_points_from_xml(QXmlStreamReader& xml)
{
    graphPoints.clear();

    do
    {
        while(xml.readNextStartElement()) {
            if(xml.name() == "point") {
                graphPoints.push_back(xml.attributes().value("val").toString().toDouble());
            } else {
                xml.skipCurrentElement();
            }
        }
    } while(xml.name() != "graph");
}
