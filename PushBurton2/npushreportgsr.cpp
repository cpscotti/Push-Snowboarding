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

#include "npushreportgsr.h"

NPushReportGSR::NPushReportGSR() : graphPoints(100, 0.0)
{
    running = false;
}

NPushReportGSR::~NPushReportGSR()
{

}

bool NPushReportGSR::start_run()
{
    QMutexLocker locker(&dataAccessMutex);

    if(running)
        return false;

    graphPoints.clear();

    GSR_max = 0;
    GSR_avg = 0;

    GSR_acc = 0;
    GSR_cnt = 0;

    running = true;

    return true;
}

bool NPushReportGSR::end_run()
{
    QMutexLocker locker(&dataAccessMutex);

    if(!running)
        return false;

    if(GSR_cnt > 0)
        GSR_avg = (double)(GSR_acc/(long int)GSR_cnt);
    else
        GSR_avg = 0;

    running = false;
    return true;
}

void NPushReportGSR::append_new_reading(int reading)
{
    QMutexLocker locker(&dataAccessMutex);
    if(reading > GSR_max) {
        GSR_max = reading;
    }

    GSR_acc += reading;
    GSR_cnt++;

    graphPoints.push_back((double)reading);
}

bool NPushReportGSR::save_to_dir(QString &dirName)
{

    QString reportFilename = (dirName + FSC_FSYS_SLASH) + "Rush.xml";
    QFile reportFile(reportFilename);

    reportFile.open(QFile::WriteOnly | QFile::Text);

    qDebug() << "GSR -> Saving to dir";

    QXmlStreamWriter xml;

    xml.setDevice(&reportFile);
    xml.setAutoFormatting(true);

    xml.writeStartDocument();

    dataAccessMutex.lock();


    xml.writeStartElement("GSRReport");

    xml.writeStartElement("stats");

    xml.writeAttribute("max", QString::number(GSR_max));
    xml.writeAttribute("avg", QString::number(GSR_avg));

    xml.writeEndElement();//stats

    xml.writeStartElement("graph");

    SelfShrinkingList::iterator it;
    for(it = graphPoints.begin();it != graphPoints.end();++it) {
        xml.writeStartElement("point");

        xml.writeAttribute("val", QString::number(*it));

        xml.writeEndElement();
    }
    xml.writeEndElement();//graph

    xml.writeEndElement();//GSRReport
    dataAccessMutex.unlock();

    xml.writeEndDocument();

    reportFile.close();

    qDebug() << "GSR -> Ended";
}

bool NPushReportGSR::load_from_dir(QString &dirName)
{

    QString data_in_filename = (dirName + FSC_FSYS_SLASH) + "Rush.xml";

    QFile data_input(data_in_filename);
    data_input.open(QFile::ReadOnly | QFile::Text);

    QXmlStreamReader xml;
    xml.setDevice(&data_input);

    if(xml.readNextStartElement())
    {
        if(xml.name() != "GSRReport") {
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
                GSR_max = xml.attributes().value("max").toString().toDouble();
                GSR_avg = xml.attributes().value("avg").toString().toDouble();
            } else if(xml.name() == "graph") {
                read_points_from_xml(xml);
            } else {
                xml.skipCurrentElement();
            }
        }
    }   while(xml.name() != "GSRReport");
}

void NPushReportGSR::read_points_from_xml(QXmlStreamReader& xml)
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
