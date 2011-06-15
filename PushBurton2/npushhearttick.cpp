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

#include "npushhearttick.h"

NPushHeartTick::NPushHeartTick(int a_hr_3, int a_hr_2, int a_hr_1)
{

    hr_3 = a_hr_3;
    hr_2 = a_hr_2;
    hr_1 = a_hr_1;

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

NPushHeartTick::NPushHeartTick(int a_hr_3, int a_hr_2, int a_hr_1, quint64 a_msecsToEpoch)
{
    hr_3 = a_hr_3;
    hr_2 = a_hr_2;
    hr_1 = a_hr_1;

    msecsToEpoch = a_msecsToEpoch;
}

NPushHeartTick::~NPushHeartTick()
{
}

void NPushHeartTick::read_from_xml( QXmlStreamReader& xml)
{
    hr_3 = xml.attributes().value("last32").toString().toInt();
    hr_2= xml.attributes().value("last16").toString().toInt();
    hr_1 = xml.attributes().value("lastBeat").toString().toInt();

    msecsToEpoch = (quint64)((double)xml.attributes().value("tstamp").toString().toDouble()*1000.0);
}

void NPushHeartTick::dump_to_xml(QXmlStreamWriter& xml) const
{
    xml.writeStartElement("heart_data");
    xml.writeAttribute("tstamp", QString::number(((double)msecsToEpoch)/(double)1000.0, 'f', 3));

    xml.writeAttribute("last32", QString::number(hr_3));
    xml.writeAttribute("last16", QString::number(hr_2));
    xml.writeAttribute("lastBeat", QString::number(hr_1));

    xml.writeEndElement();//heart_data
}

QString NPushHeartTick::get_pretty_print() const
{
    return QString("Heart= %1")
            .arg(QString::number(hr_1));
}
