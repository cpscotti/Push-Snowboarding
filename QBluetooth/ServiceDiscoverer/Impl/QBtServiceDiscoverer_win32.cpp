/*
 * QBtServiceDiscoverer_stub.cpp
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

#include <QtGlobal>
#include "../QBtServiceDiscoverer_win32.h"

QBtServiceDiscovererPrivate::QBtServiceDiscovererPrivate(QBtServiceDiscoverer* publicClass):
	QObject(NULL), p_ptr(publicClass), isBusy(false)
{
	Construct();
}

void QBtServiceDiscovererPrivate::Construct()
{
	InitBthSdk();

	connect(p_ptr, SIGNAL(discoveryStarted()), 
			this, SLOT(SetBusy()));
	connect(p_ptr, SIGNAL(discoveryStopped()),
			this, SLOT(SetNotBusy()));
}

QBtServiceDiscovererPrivate::~QBtServiceDiscovererPrivate()
{	
	DeinitBthSdk();
}

void QBtServiceDiscovererPrivate::DiscoverServicesOnDevice(QBtDevice* targetDevice)
{
	BTSVCHDL remoteServiceHandleArray[128] = {0};
	BTUINT32 remoteServiceNumber = 128;

	BTUINT32 result = BTSDK_FALSE;
	
	BTDEVHDL devHandle = GetDeviceHandle(targetDevice->getAddress());

	if(devHandle == BTSDK_INVALID_HANDLE)
	{
		emit p_ptr->error(QBtServiceDiscoverer::UnknownError);
		return;
	}

	emit p_ptr->discoveryStarted();
	int cnt = 0;
	do
	{
		result = Btsdk_BrowseRemoteServices(devHandle, remoteServiceHandleArray, &remoteServiceNumber);
		cnt++;
	}while(cnt < 5 && result != BTSDK_OK);
	
	if(cnt == 5 && result != BTSDK_OK)
	{
		emit p_ptr->error(QBtServiceDiscoverer::UnknownError);
		return;
	}
	
	// process results
	ProcessFoundServices(remoteServiceHandleArray, remoteServiceNumber);
	
	emit p_ptr->discoveryStopped();

	return;
}

void QBtServiceDiscovererPrivate::DiscoverSpecificClass(
			QBtDevice* targetDevice, QBtConstants::ServiceClass uuid)
{
	BtSdkSDPSearchPatternStru sdpStru;
	sdpStru.mask = BTSDK_SSPM_UUID16;
	sdpStru.uuid.Data1 = uuid;

	BTSVCHDL servicesFound[128] = {0};
	BTUINT32 servicesNumFound = 128;
	
	BTDEVHDL devHandle = GetDeviceHandle(targetDevice->getAddress());
	
	if(devHandle == BTSDK_INVALID_HANDLE)
	{
		emit p_ptr->error(QBtServiceDiscoverer::UnknownError);
		return;
	}

	emit p_ptr->discoveryStarted();
	
	//start searching on a specific device and service
	int tmpCnt = 0;
	BTINT32 result = BTSDK_FALSE;
	do
	{
		result = Btsdk_BrowseRemoteServicesEx(devHandle, &sdpStru, 1, servicesFound, &servicesNumFound);
		tmpCnt++;
	}while(tmpCnt < 5 && result != BTSDK_OK);
	
	if(tmpCnt == 5 && result != BTSDK_OK)
	{
		emit p_ptr->error(QBtServiceDiscoverer::ServiceDiscoveryNotAbleToComplete);
		return;
	}

	// process results
	ProcessFoundServices(servicesFound, servicesNumFound);
	
	emit p_ptr->discoveryStopped();
	
	return;
}

void QBtServiceDiscovererPrivate::DiscoverSpecificProtocol(
			QBtDevice* /*targetDevice*/, QBtConstants::ServiceProtocol /*uuid*/)
{
	emit p_ptr->error(QBtServiceDiscoverer::NotSupported);
	return;
}

void QBtServiceDiscovererPrivate::StopDiscovery()
{
	return;
}

// private methods

void QBtServiceDiscovererPrivate::ProcessFoundServices(BTSVCHDL* foundServices, int numOfServices)
{
	BTUINT32 result = BTSDK_FALSE;
	BtSdkRemoteServiceAttrStru serviceInfo;
	serviceInfo.mask = BTSDK_RSAM_SERVICENAME;

	for(int i=0; i<numOfServices; i++)
	{
		result = Btsdk_GetRemoteServiceAttributes(foundServices[i], &serviceInfo);
		if(result != BTSDK_OK)
			return;

		QBtService newService;
		newService.setHandle(foundServices[i]);
		newService.setName(QString::fromUtf8((const char*)serviceInfo.svc_name));
		newService.setClass((QBtConstants::ServiceClass)serviceInfo.service_class);
		
		if(serviceInfo.service_class == BTSDK_CLS_SERIAL_PORT)
			RetrieveSPPAttributes(&newService, foundServices[i]);
			
		p_ptr->_remoteDevice.addNewService(newService);
		emit p_ptr->newServiceFound(p_ptr->_remoteDevice, newService);
	}
}

void QBtServiceDiscovererPrivate::RetrieveSPPAttributes(QBtService* currService, BTSVCHDL servHandle)
{
	BTUINT32 result = BTSDK_FALSE;
	BtSdkRmtSPPSvcExtAttrStru infoStructure;
	BtSdkRemoteServiceAttrStru serviceInfo;
	serviceInfo.mask = BTSDK_RSAM_EXTATTRIBUTES;
	serviceInfo.ext_attributes = &infoStructure;

	result = Btsdk_GetRemoteServiceAttributes(servHandle, &serviceInfo);
	if(result != BTSDK_OK)
		return;
	
	currService->setPort((unsigned int)infoStructure.server_channel);
}

void QBtServiceDiscovererPrivate::SetBusy()
{
	isBusy = true;
}

void QBtServiceDiscovererPrivate::SetNotBusy()
{
	isBusy = false;
}

bool QBtServiceDiscovererPrivate::IsBusy() const
{
	return isBusy;
}
