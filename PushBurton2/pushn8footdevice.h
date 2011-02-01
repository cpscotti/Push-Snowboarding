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

#ifndef PUSHN8FOOTDEVICE_H
#define PUSHN8FOOTDEVICE_H

#include <QString>
#include <QObject>
#include "pushn8btdevice.h"

#include "npushfoottick.h"

class PushN8FootDevice : public PushN8BtDevice
{
#ifdef Q_OS_SYMBIAN
    Q_OBJECT
public:
    PushN8FootDevice(QBtDevice a_device, QObject *parent = 0);

    ~PushN8FootDevice();

    char get_side();

private:
    char side;

public slots:
    void gotDataReceived(QString);
#endif
};

#endif // PUSHN8FOOTDEVICE_H
