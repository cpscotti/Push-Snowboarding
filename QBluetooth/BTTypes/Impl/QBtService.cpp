/*
 * QBtService.cpp
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

#include "../QBtService.h"

QBtService::QBtService()
	: classUUID(QBtConstants::UndefinedClass),
	  name("Undefined"),
	  description("Undefined"),
	  handle(0),
	  port(0) 
{	
	protocolUUID.clear();
}

/*
QBtService::QBtService(const QBtService& service) :
	name(service.getName()), classUUID(service.getClass()), 
	port(service.getPort()), handle(service.getHandle()), 
	description(service.getDescription())
{
	protocolUUID = service.getProtocols();
}
*/

//////////////////////////////////////////////
//					Accessors				//
//////////////////////////////////////////////
QBtService::ProtocolList QBtService::getProtocols() const
{
	return protocolUUID;
}

QBtConstants::ServiceClass QBtService::getClass() const
{
	return classUUID;
}

QString QBtService::getName() const
{
	return name;
}

QString QBtService::getDescription() const
{
	return description;
}

unsigned int QBtService::getPort() const
{
	return port;
}

unsigned int QBtService::getHandle() const
{
	return handle;
}


//////////////////////////////////////////////
//				Mutators					//
//////////////////////////////////////////////

void QBtService::setClass(QBtConstants::ServiceClass newClass)
{
	classUUID = newClass;
}

void QBtService::setName(const QString& newName)
{
	name = newName;
}

void QBtService::setDescription(const QString& newDescription)
{
	description = newDescription;
}

void QBtService::setPort(unsigned int newPort)
{
	port = newPort;
}

void QBtService::setHandle(unsigned int newHandle)
{
	handle = newHandle;
}

void QBtService::setProtocols(const QBtService::ProtocolList & newUUIDs)
{
	protocolUUID = newUUIDs;
}

void QBtService::addProtocol(QBtConstants::ServiceProtocol uuid)
{
	if(protocolUUID.indexOf(uuid) == -1) // if not on list
		protocolUUID.append(uuid);
}

/**
 * Returns false if uuid not found in the protocol list
 */
bool QBtService::removeProtocol(QBtConstants::ServiceProtocol uuid)
{
	return protocolUUID.removeOne(uuid);
}

//////////////////////////////////////////
//				Operators				//	
//////////////////////////////////////////
/*
QBtService& QBtService::operator= (const QBtService& service)
{
	if ( this == &service )  
			return *this;
	
	name 			= service.getName();
	port 			= service.getPort();
	classUUID 		= service.getClass();
	protocolUUID 	= service.getProtocols();
	handle 			= service.getHandle();
	description 	= service.getDescription();
	
	
	return *this;
}
*/
