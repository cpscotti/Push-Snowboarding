/*
 * QBtDevice.cpp
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

#include "../QBtDevice.h"

QBtDevice::QBtDevice() :
	_name("Undefined"), _address(), _type(Uncategorized), _serviceListUpdated (false)
{
	_supportedServices.clear();
}

QBtDevice::QBtDevice(const QBtDevice& dev) 
	: _name(dev.getName()), _address(dev.getAddress()), _type(dev.getType()), _serviceListUpdated (dev._serviceListUpdated)
{	
	QBtService::List* servList = new QBtService::List();
	*servList = dev.getSupportedServices();
		
	_supportedServices.clear();
	_supportedServices = *servList;
}

QBtDevice::QBtDevice (const QString & devName, const QBtAddress & devAddress, DeviceMajor devType) 
	: _name(devName), _address(devAddress), _type(devType)
{
	_supportedServices.clear();
}

QBtDevice::~QBtDevice()
{

}

void QBtDevice::addNewService (const QBtService& newService)
{
	_supportedServices.append(newService);
}

void QBtDevice::setName (const QString & newName)
{
	_name = newName;
}

void QBtDevice::setAddress (const QBtAddress& newAddress)
{
	_address = newAddress;
}

void QBtDevice::setType(DeviceMajor newType)
{
	_type = newType;
}

void QBtDevice::setSupportedServices (const QBtService::List& newSupportedServices)
{
	_serviceListUpdated = true;
	
	_supportedServices.clear();
	_supportedServices = newSupportedServices;
}

bool QBtDevice::serviceListUpdated() const
{
	return _serviceListUpdated;
}

QString QBtDevice::getName() const
{
	return _name;
}

QBtAddress QBtDevice::getAddress() const
{
	return _address;
}

QBtDevice::DeviceMajor QBtDevice::getType() const
{
	return _type;
}

QString QBtDevice::getTypeAsString() const
{
	switch(_type)
	{
		case (QBtDevice::Miscellaneous):
			return "Miscellaneous";
		case (QBtDevice::Computer):
			return "Computer";
		case (QBtDevice::Phone):
			return "Phone";
		case (QBtDevice::LANAccess):
			return "LANAccess";
		case (QBtDevice::AudioVideo):
			return "AudioVideo";
		case (QBtDevice::Peripheral):
			return "Peripheral";
		case (QBtDevice::Imaging):
			return "Imaging";
		default:
			return "Uncategorized";
	}
}

QBtService::List QBtDevice::getSupportedServices() const
{
	return _supportedServices;
}

/*
QBtDevice& QBtDevice::operator= (const QBtDevice& dev)
{
	if ( this == &dev )  
		return *this;

	
	QBtAddress* addr = new QBtAddress(dev.getAddress());
	QBtService::List* servList = new QBtService::List();
	*servList = dev.getSupportedServices();
	
	setName(dev.getName());
	setAddress(*addr);
	setType(dev.getType());
	setSupportedServices(*servList);
	
	return *this;
}
*/
