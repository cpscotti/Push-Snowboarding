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

#include "npushnormfeettick.h"

NPushNormFeetTick::NPushNormFeetTick(SnowboradPressureData a_pdata, quint64 a_msecsToEpoc)
    :msecsToEpoch(a_msecsToEpoc), pdata(a_pdata)

{
}


NPushNormFeetTick::~NPushNormFeetTick()
{

}

void NPushNormFeetTick::dump_to_xml(QXmlStreamWriter& xml) const
{
    xml.writeStartElement("feet_sum");

    xml.writeAttribute("tstamp", QString::number(((double)msecsToEpoch)/(double)1000.0, 'f', 3));
    xml.writeAttribute("sum", QString::number(pdata.totalRawSum));


//    xml.writeAttribute("norm_factor", QString::number(pdata.totalNormFactor));

//    xml.writeAttribute("on_air", QString((pdata.totalNormFactor < 0.5)?"true":"false"));

//    xml.writeAttribute("valid_cnt", QString::number(pdata.validCount));
//    xml.writeAttribute("minLToe", QString::number(pdata.LToe.minVal));
//    xml.writeAttribute("minLHeel", QString::number(pdata.LHeel.minVal));
//    xml.writeAttribute("minRToe", QString::number(pdata.RToe.minVal));
//    xml.writeAttribute("minRHeel", QString::number(pdata.RHeel.minVal));

    xml.writeEndElement();//feet_sum
}

QString NPushNormFeetTick::get_pretty_print() const
{
    return QString("Feet PSum [%1]")
            .arg(QString::number(pdata.totalRawSum));
}
