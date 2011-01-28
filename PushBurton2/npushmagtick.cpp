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

#include "npushmagtick.h"

NPushMagTick::NPushMagTick(QMagnetometerReading * reading)
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
}

NPushMagTick::NPushMagTick(QMagnetometerReading *reading, quint64 a_msecsToEpoch)
{
    tick.copyValuesFrom(reading);

    msecsToEpoch = a_msecsToEpoch;
}

NPushMagTick::~NPushMagTick()
{
}

void NPushMagTick::append_to_xml(QDomDocument& doc, QDomElement& root) const
{
    QDomElement tickDom = doc.createElement("mag_data");
    tickDom.setAttribute("x", QString::number(tick.x()*1000.0,'f',5));
    tickDom.setAttribute("y", QString::number(tick.y()*1000.0,'f',5));
    tickDom.setAttribute("z", QString::number(tick.z()*1000.0,'f',5));
    tickDom.setAttribute("r", QString::number(qSqrt(tick.x()*tick.x()+
                                                    tick.y()*tick.y()+
                                                    tick.z()*tick.z())*1000.0,'f',5));
    tickDom.setAttribute("tstamp", (double)(msecsToEpoch*0.001));

    root.appendChild(tickDom);
}

void NPushMagTick::dump_to_xml(QXmlStreamWriter& xml) const
{
    xml.writeStartElement("mag_data");

    xml.writeAttribute("tstamp", QString::number(((double)msecsToEpoch)/(double)1000.0, 'f', 3));

    xml.writeAttribute("x", QString::number(tick.x()*1000.0,'f',5));
    xml.writeAttribute("y", QString::number(tick.y()*1000.0,'f',5));
    xml.writeAttribute("z", QString::number(tick.z()*1000.0,'f',5));
    xml.writeAttribute("r", QString::number(qSqrt(tick.x()*tick.x()+
                                                    tick.y()*tick.y()+
                                                    tick.z()*tick.z())*1000.0,'f',5));
    xml.writeEndElement();//mag_data
}

QString NPushMagTick::get_pretty_print() const
{
    return QString("Magnetometer Update: [x;y;z]mPhi = [%1;%2;%3]")
            .arg(QString::number(tick.x()*1000.0,'f',4))
            .arg(QString::number(tick.y()*1000.0,'f',4))
            .arg(QString::number(tick.z()*1000.0,'f',4));
}

