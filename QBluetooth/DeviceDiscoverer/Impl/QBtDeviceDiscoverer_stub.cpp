/*
 * QBtDeviceDiscoverer_stub.cpp
 *
 *  
 *      Author: Ftylitakis Nikolaos
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "../QBtDeviceDiscoverer_stub.h"
#include "../QBtDeviceDiscoverer.h"

// ================= MEMBER FUNCTIONS =======================
//
// ---------------------------------------------------------
// QBtDeviceDiscovererPrivate::QBtDeviceDiscovererPrivate()
// Constructor
// ---------------------------------------------------------
//
QBtDeviceDiscovererPrivate::QBtDeviceDiscovererPrivate(QBtDeviceDiscoverer *wrapper) 
 : d_ptr(wrapper)
{
}


// ---------------------------------------------------------
// QBtDeviceDiscovererPrivate::~QBtDeviceDiscovererPrivate()
// Destructor
// ---------------------------------------------------------
//
QBtDeviceDiscovererPrivate::~QBtDeviceDiscovererPrivate()
{
}

// ---------------------------------------------------------
// QBtDeviceDiscovererPrivate::startSearch()
// Called to start searching for new devices
// ---------------------------------------------------------
//
void QBtDeviceDiscovererPrivate::DiscoverDevices()
{
    emit d_ptr->error(QBtDeviceDiscoverer::BluetoothNotSupported);
}

// ---------------------------------------------------------
// QBtDeviceDiscovererPrivate::startSearch()
// Called to stop searching
// ---------------------------------------------------------
//
void QBtDeviceDiscovererPrivate::StopDiscovery()
{
    emit d_ptr->error(QBtDeviceDiscoverer::BluetoothNotSupported);
}

QBtDevice::List QBtDeviceDiscovererPrivate::GetInquiredDevices()
{
	return QBtDevice::List();
}
