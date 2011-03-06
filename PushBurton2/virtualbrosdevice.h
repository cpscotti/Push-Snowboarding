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

#ifndef VIRTUALBROSDEVICE_H
#define VIRTUALBROSDEVICE_H

#include <Phonon/Phonon>
#include <QString>
#include <typeinfo> //needed by the subscribe mechanism

#include <QApplication>

#include "pushburtongenericdevice.h"

#include "npushairtimetick.h" //data structure that will "bring" the air time data
#include "pushn8airtimedetector.h" //device who we'll subscribe to

class VirtualBrosDevice : public PushBurtonGenericDevice
{
    Q_OBJECT
public:
    explicit VirtualBrosDevice(QObject *parent = 0); //Was created automatically

    //We'll need the good'ol destructor to keep things clean
    ~VirtualBrosDevice();

    //The following two functions are Pure Virtual functions on PushBurtonGenericDevice
    //you have to implement them - there's no implementation on the baseclass
    //(Watch the =0 at the end of their declarations there!!)
    QString get_description();
    bool is_online();

    //Our device subscribes to other devices so we need to reimplement these 2 functions
    //accordingly. One can argue that subscribesToAny wouldn't be necessary but it SURE
    //prevents doing a huge amount of unnecessary checks.
    bool subscribesToAny();//returns true..

    //This function will return true when the given device is a AirTimeDetector
    bool subscribesTo(PushBurtonGenericDevice* deviceType);


signals:

public slots:
    //last but not least, we need to reimplement the slot that will receive all the incoming data,
    //"judge" it and stream sounds accordingly
    void incoming_reading(NPushLogTick *);

private:
    //pointer that will hold the sound ready to be "played"
    Phonon::MediaObject *applause;

};

#endif // VIRTUALBROSDEVICE_H
