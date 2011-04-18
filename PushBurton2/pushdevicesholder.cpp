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

#include "pushdevicesholder.h"

PushDevicesHolder::PushDevicesHolder()
{
    tickDisposer = new NPushTickDisposer();
}

PushDevicesHolder::~PushDevicesHolder()
{
    for(int i=0;i<this->count();i++)
    {
        this->at(i)->disconnect_from_backend();
        this->at(i)->deleteLater();
    }

    if(tickDisposer) {
        qDebug() << "Calling delete later for tick disposer";
        tickDisposer->deleteLater();
    }
}

int PushDevicesHolder::deleteAllAndClear()
{
    int i;
    for(i=0; i < this->count(); i++)
    {
        PushBurtonGenericDevice * iDev = this->at(i);
        iDev->deleteLater();
    }
    this->clear();
    return i;
}

bool PushDevicesHolder::CheckForDeviceType(QString& device_type)
{
//    qDebug() << "checking for: " << device_type;
    for(int i=0; i < this->count(); i++)
    {
        QString thisDevTypeName(typeid(*(this->at(i))).name());
//        qDebug() << "against: " << thisDevTypeName;
        if(thisDevTypeName == device_type)
        {
            return true;
        }
    }
    return false;
}

void PushDevicesHolder::push_back(PushBurtonGenericDevice* ptr)
{
    qDebug() << "Adding device to PushDevicesHolder internal list";
    QVector<PushBurtonGenericDevice *>::push_back(ptr);

    //Essential connection to logTicksDisposer! Without this the program is a dangling huge memory leaker
    ptr->connect(ptr, SIGNAL(reading_ready(NPushLogTick*)), tickDisposer, SLOT(log_ticks_sink(NPushLogTick*)), Qt::UniqueConnection);

    if(typeid(*ptr) == typeid(PushBurtonGenericDevice)){
        qDebug() << "Initial type check O.K.";
    }

    //checking if new device subscribes to any of the availables devices
    if(ptr->subscribesToAny()) {
        for(int i=0;i<this->count();i++) {
            PushBurtonGenericDevice* iptr = this->at(i);
            //If new device subscribes to it //PS: This enables recursion!!
            if(ptr->subscribesTo(iptr) || typeid(*iptr) == typeid(PushN8SimulationDevice)) {
                qDebug() << "Connected " << iptr->get_description() << "\'s output to " << ptr->get_description();
                ptr->connect(iptr, SIGNAL(reading_ready(NPushLogTick*)), ptr, SLOT(incoming_reading(NPushLogTick*)), Qt::UniqueConnection);
            }
        }
    }

    //Checking if previously available device subscribes to newly added device
    for(int i=0;i<this->count()-1;i++) {
        PushBurtonGenericDevice* iptr = this->at(i);
        if(iptr->subscribesToAny()) {
            if(iptr->subscribesTo(ptr)) {
                qDebug() << "Connected " << ptr->get_description() << "\'s output to " << iptr->get_description();
                iptr->connect(ptr, SIGNAL(reading_ready(NPushLogTick*)), iptr, SLOT(incoming_reading(NPushLogTick*)), Qt::UniqueConnection);
            }
        }
    }
}
