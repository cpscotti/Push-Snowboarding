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

#include "npushgpstick.h"

NPushGpsTick::NPushGpsTick(const QGeoPositionInfo &info)
{
    tick = info;
//    QDateTime correctedTstamp;
//    correctedTstamp = tick.timestamp().addSecs(3600);
//    tick.setTimestamp(correctedTstamp);
}

NPushGpsTick::~NPushGpsTick()
{
}

void NPushGpsTick::read_from_xml( QXmlStreamReader& xml)
{
    QDateTime tstamp;
    QGeoCoordinate fakeCoord;

    fakeCoord.setAltitude(xml.attributes().value("altitude").toString().toDouble());
    fakeCoord.setLatitude(xml.attributes().value("latitude").toString().toDouble());
    fakeCoord.setLongitude(xml.attributes().value("longitude").toString().toDouble());

    tstamp = QDateTime::fromTime_t(floor(xml.attributes().value("tstamp").toString().toDouble()));//+3600.0));

    tick.setTimestamp(tstamp);
    tick.setCoordinate(fakeCoord);

    tick.setAttribute(QGeoPositionInfo::GroundSpeed, xml.attributes().value("ground_speed").toString().toFloat());

}

void NPushGpsTick::dump_to_xml(QXmlStreamWriter& xml) const
{
    xml.writeStartElement("gps_data");
    xml.writeAttribute("tstamp", QString::number(tick.timestamp().toTime_t()));
    xml.writeAttribute("longitude", QString::number(tick.coordinate().longitude()));
    xml.writeAttribute("latitude", QString::number(tick.coordinate().latitude()));
    xml.writeAttribute("altitude", QString::number(tick.coordinate().altitude()));
    xml.writeAttribute("ground_speed", QString::number(tick.attribute(QGeoPositionInfo::GroundSpeed)));

    xml.writeEndElement();//gps_data
}

QString NPushGpsTick::get_pretty_print() const
{
    return QString("Speed = %1, Alt = %2").arg(QString::number(tick.attribute(QGeoPositionInfo::GroundSpeed)*3.6)).arg(QString::number(tick.coordinate().altitude()));
}
