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

#ifndef NPUSHHEARTTICK_H
#define NPUSHHEARTTICK_H

#include "npushlogtick.h"

#include <QtXml>
#include <QDateTime>

class NPushHeartTick : public NPushLogTick
{
public:

    NPushHeartTick() {}

    NPushHeartTick(int a_hr_3, int a_hr_2, int a_hr_1);

    NPushHeartTick(int a_hr_3, int a_hr_2, int a_hr_1, quint64 a_msecsToEpoch);

    virtual ~NPushHeartTick();

    virtual void read_from_xml( QXmlStreamReader& xml);
    virtual void dump_to_xml(QXmlStreamWriter& xml) const;

    virtual QString get_pretty_print() const;

    int hr_3;
    int hr_2;
    int hr_1;

    quint64 msecsToEpoch;
};

#endif // NPUSHHEARTTICK_H
