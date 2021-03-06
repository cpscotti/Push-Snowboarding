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

#ifndef NPUSHACCTICK_H
#define NPUSHACCTICK_H

#include "npushlogtick.h"
#include <qmath.h>
#include <QAccelerometer>
#include <QtXml>
#include <QTime>

QTM_USE_NAMESPACE
class NPushAccTick : public NPushLogTick
{
public:
    NPushAccTick() {}
    NPushAccTick(QAccelerometerReading * reading);
    NPushAccTick(QAccelerometerReading * reading, quint64 a_msecsToEpoch);

    virtual ~NPushAccTick();

    virtual void read_from_xml( QXmlStreamReader& xml);
    virtual void dump_to_xml(QXmlStreamWriter& xml) const;

    virtual QString get_pretty_print() const;

    QAccelerometerReading tick;
    qreal pAccAbsMag;

    quint64 msecsToEpoch;

    static const qreal freefall_threshold = 7.0;
};

#endif // NPUSHACCTICK_H
