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

#ifndef NPUSHIMUTICK_H
#define NPUSHIMUTICK_H

#include "npushlogtick.h"

#include <QtXml>
#include <QDateTime>

class NPushIMUTick : public NPushLogTick
{
public:

    NPushIMUTick() {}

    NPushIMUTick(QStringList& values);
    NPushIMUTick(QStringList &values, quint64 a_msecsToEpoch);

    virtual ~NPushIMUTick();

    virtual void read_from_xml( QXmlStreamReader& xml);
    virtual void dump_to_xml(QXmlStreamWriter& xml) const;

    virtual QString get_pretty_print() const;

    void ParseValues(QStringList& values);

    int gyro[3];
    int accel[3];
    int mag[3];

    quint64 msecsToEpoch;

    /*
     *  black magic!!
     *
     *  So far we still have to cope with old, analog, accelerometers.
     *  Using this static flag, we make sure that after the sensor type
     *  is determined, app will handle it correctly throughout runtime
     *
     *  (Same happens in simulation)
     */
    static bool digitalAcc; //initialized to false
};

#endif // NPUSHIMUTICK_H
