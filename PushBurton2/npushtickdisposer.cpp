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

#include "npushtickdisposer.h"

NPushTickDisposer::NPushTickDisposer(QObject *parent) :
    QObject(parent)
{
    qDebug() << "Tick Disposer initialized";
}

NPushTickDisposer::~NPushTickDisposer()
{
    qDebug() << "Tick Disposer Destructor, gotta get rid of " << get_queue_count() << " logTicks";
    while(ticksQueue.count() > 0){
        NPushLogTick * dyingTick = ticksQueue.dequeue();
        delete dyingTick;
    }
    qDebug() << "Cleanup finished";
}

void NPushTickDisposer::log_ticks_sink(NPushLogTick * tick)
{
    ticksQueue.enqueue(tick);
    perform_maintenance();
}


void NPushTickDisposer::perform_maintenance()
{
    static int msgCnt = 0;
    while(ticksQueue.count() > maxQueueSize) {
        NPushLogTick * dyingTick = ticksQueue.dequeue();
        delete dyingTick;

        if(msgCnt == 0) {
            qDebug() << "Cleaning up";
            msgCnt++;
        }
    }
}


int NPushTickDisposer::get_queue_count()
{
    return ticksQueue.count();
}
