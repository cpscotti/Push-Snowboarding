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

#include "runmanager.h"

RunManager::RunManager(PushDevicesHolder * aDevHolder, QObject *parent) :
    QObject(parent), configuredDevices(aDevHolder)
{
    logging = false;
    ubiqSaver = 0;
}

RunManager::~RunManager()
{
}

void RunManager::start_logging()
{
    if(logging)
        return;
    logging = true;
    uint startTime = QDateTime::currentDateTime().toTime_t();

    ubiqSaver = new UbiqLogSaver(startTime);

    UpdateDataStreamConnections();

    for(int i=0;i<configuredDevices->count();i++)
    {
        PushBurtonGenericDevice * dev = configuredDevices->at(i);
        dev->start_readings();
        dev->start_run();
    }
}

void RunManager::stop_logging()
{
    if(!logging)
        return;
    logging = false;

    for(int i=0;i<configuredDevices->count();i++)
    {
        PushBurtonGenericDevice * dev = configuredDevices->at(i);
        dev->end_run();
        dev->stop_readings();
    }

    ubiqSaver->run_end();
    delete ubiqSaver;
    ubiqSaver = 0;

    UpdateDataStreamConnections();

    emit dataSaved();
}

void RunManager::UpdateDataStreamConnections()
{
    for(int i=0;i<configuredDevices->count();i++)
    {
        PushBurtonGenericDevice * dev = configuredDevices->at(i);
        if(ubiqSaver){
            disconnect(dev, SIGNAL(report_ready(NPushGenReport*)), ubiqSaver, SLOT(report_in(NPushGenReport*)));
            disconnect(dev, SIGNAL(reading_ready(NPushLogTick*)), ubiqSaver, SLOT(log_tick_in(NPushLogTick*)));
        }

        if(logging)
        {
            if(ubiqSaver) {
                connect(dev, SIGNAL(report_ready(NPushGenReport*)), ubiqSaver, SLOT(report_in(NPushGenReport*)));
                connect(dev, SIGNAL(reading_ready(NPushLogTick*)), ubiqSaver, SLOT(log_tick_in(NPushLogTick*)));
            }
        }
    }
}
