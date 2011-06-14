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

#include "npushairtimetick.h"


NPushAirTimeTick::NPushAirTimeTick(quint64 a_msecsInAir, bool a_landed,
                                   quint64 a_msecsToEpoch, int nDataPts, bool a_hint)
{
    landed = a_landed;
    msecsToEpoch = a_msecsToEpoch;
    msecsOnAir = a_msecsInAir;
    isOnlyHint = a_hint;
    nOfDataPoints = nDataPts;
}


NPushAirTimeTick::~NPushAirTimeTick()
{
}


void NPushAirTimeTick::append_to_xml(QDomDocument& doc, QDomElement& root) const
{
    if(!isOnlyHint) {
        QDomElement tickDom = doc.createElement("air_time");
        tickDom.setAttribute("tstamp", (double)(msecsToEpoch*0.001));
        tickDom.setAttribute("in_air", (double)(msecsOnAir*0.001));
        tickDom.setAttribute("data_points", nOfDataPoints);
        tickDom.setAttribute("landed", ((landed)?"true":"false") );

        root.appendChild(tickDom);
    }
}

void NPushAirTimeTick::dump_to_xml(QXmlStreamWriter& xml) const
{
    if(!isOnlyHint) {
        xml.writeStartElement("air_time");

        xml.writeAttribute("tstamp", QString::number(((double)msecsToEpoch)/(double)1000.0, 'f', 3));
        xml.writeAttribute("in_air", QString::number(((double)msecsOnAir)/(double)1000.0, 'f', 3));
        xml.writeAttribute("data_points", QString::number(nOfDataPoints));
        xml.writeAttribute("landed", ((landed)?"true":"false") );
        xml.writeEndElement();//air_time
    }
}

QString NPushAirTimeTick::get_pretty_print() const
{
    return QString(((landed)?"Landed [%1]":"AIR [%1]"))
            .arg(QString::number(msecsOnAir));
}
