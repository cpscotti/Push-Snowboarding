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

#ifndef NPUSHNORMFEETTICK_H
#define NPUSHNORMFEETTICK_H

#include "npushlogtick.h"

#include "AbsNormFeetTypes.h"

class NPushNormFeetTick : public NPushLogTick
{
public:
    NPushNormFeetTick(SnowboradPressureData, quint64);
    ~NPushNormFeetTick();


    void dump_to_xml(QXmlStreamWriter& xml) const;

    QString get_pretty_print() const;

public:

    quint64 msecsToEpoch;
    SnowboradPressureData pdata;
};

#endif // NPUSHNORMFEETTICK_H
