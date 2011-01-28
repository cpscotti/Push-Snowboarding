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

#include "npushacctick.h"

NPushAccTick::NPushAccTick(QAccelerometerReading * reading)
{
    tick.copyValuesFrom(reading);

    uint secs_to_epoch;
    uint msecs;

    secs_to_epoch = QDateTime::currentDateTime().toTime_t();
    msecs = QTime::currentTime().msec();
    uint aft = QDateTime::currentDateTime().toTime_t();

    //ugly work around, will be fized with msecsToEpoc from Qt 4.7
    if(aft != secs_to_epoch)//on a second slip
    {
        msecs = 0;
        secs_to_epoch = aft;
    }

    msecsToEpoch = (double)secs_to_epoch*1000.0+(double)msecs;

    pAccAbsMag = qSqrt(tick.x()*tick.x()+
                        tick.y()*tick.y()+
                        tick.z()*tick.z());
}

//Used to generate simulated ticks (tstamp comes directly from the xml file)
NPushAccTick::NPushAccTick(QAccelerometerReading * reading, quint64 a_msecsToEpoch)
{
    tick.copyValuesFrom(reading);
    msecsToEpoch = a_msecsToEpoch;

    pAccAbsMag = qSqrt(tick.x()*tick.x()+
                        tick.y()*tick.y()+
                        tick.z()*tick.z());
}

NPushAccTick::~NPushAccTick()
{
}

void NPushAccTick::append_to_xml(QDomDocument& doc, QDomElement& root) const
{
    QDomElement tickDom = doc.createElement("acc_data");
    tickDom.setAttribute("x", QString::number(tick.x()));
    tickDom.setAttribute("y", QString::number(tick.y()));
    tickDom.setAttribute("z", QString::number(tick.z()));
    tickDom.setAttribute("r", QString::number(pAccAbsMag));

    tickDom.setAttribute("tstamp", (double)(msecsToEpoch*0.001));

    if(pAccAbsMag < freefall_threshold)
    {
        tickDom.setAttribute("free_fall", "true");
    }

    root.appendChild(tickDom);
}

void NPushAccTick::dump_to_xml(QXmlStreamWriter& xml) const
{
    xml.writeStartElement("acc_data");

    xml.writeAttribute("tstamp", QString::number(((double)msecsToEpoch)/(double)1000.0, 'f', 3));

    xml.writeAttribute("x", QString::number(tick.x()));
    xml.writeAttribute("y", QString::number(tick.y()));
    xml.writeAttribute("z", QString::number(tick.z()));
    xml.writeAttribute("r", QString::number(pAccAbsMag));
    xml.writeEndElement();//acc_data
}

QString NPushAccTick::get_pretty_print() const
{
    return QString("P.Acc= %1;%2;%3")
            .arg(QString::number(tick.x()))
            .arg(QString::number(tick.y()))
            .arg(QString::number(tick.z()));
}

