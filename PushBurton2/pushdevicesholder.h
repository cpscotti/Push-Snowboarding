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

#ifndef PUSHDEVICESHOLDER_H
#define PUSHDEVICESHOLDER_H

#include <QVector>
#include <QString>
#include <QDebug>
#include <QObject>

#include "npushlogtick.h"
#include "pushburtongenericdevice.h"
#include "pushn8simulationdevice.h"
#include "npushtickdisposer.h"

class PushDevicesHolder : public QVector<PushBurtonGenericDevice *>
{
public:
    PushDevicesHolder();
    ~PushDevicesHolder();

    int deleteAllAndClear();

    bool CheckForDeviceType(QString&);

    void push_back(PushBurtonGenericDevice* ptr);


    NPushTickDisposer * tickDisposer;

//    bool CheckForDeviceName(QString&);
};

#endif // PUSHDEVICESHOLDER_H
