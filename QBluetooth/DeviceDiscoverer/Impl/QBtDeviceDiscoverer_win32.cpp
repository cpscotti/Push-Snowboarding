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

#include "../QBtDeviceDiscoverer_win32.h"
#include "../QBtDeviceDiscoverer.h"

//////////////////////////
//	Static variables	//
//////////////////////////
QBtDevice::List QBtDeviceDiscovererPrivate::deviceList;
HANDLE QBtDeviceDiscovererPrivate::browseDevEventHandler;
QBtDeviceDiscoverer* QBtDeviceDiscovererPrivate::sp_ptr;
bool QBtDeviceDiscovererPrivate::isSearching = false;

// ================= MEMBER FUNCTIONS =======================
//
// ---------------------------------------------------------
// QBtDeviceDiscovererPrivate::QBtDeviceDiscovererPrivate()
// Constructor
// ---------------------------------------------------------
//
QBtDeviceDiscovererPrivate::QBtDeviceDiscovererPrivate(QBtDeviceDiscoverer *publicClass) 
 : p_ptr(publicClass)
{
	Construct();
}

void QBtDeviceDiscovererPrivate::Construct()
{
	if(!InitBthSdk())
		return;

	BtSdkCallbackStru cb = {0};

	/* inquiry complete result ind*/
	cb.type = BTSDK_INQUIRY_COMPLETE_IND;
	cb.func = &QBtDeviceDiscovererPrivate::InquiryCompleteResult;
	Btsdk_RegisterCallback4ThirdParty(&cb);

	/* inquiry result ind*/
	cb.type = BTSDK_INQUIRY_RESULT_IND;
	cb.func = &QBtDeviceDiscovererPrivate::ReportInquiryResult;
	Btsdk_RegisterCallback4ThirdParty(&cb);
}


// ---------------------------------------------------------
// QBtDeviceDiscovererPrivate::~QBtDeviceDiscovererPrivate()
// Destructor
// ---------------------------------------------------------
//
QBtDeviceDiscovererPrivate::~QBtDeviceDiscovererPrivate()
{
	StopDiscovery();

	BtSdkCallbackStru cb;
	cb.type = BTSDK_INQUIRY_COMPLETE_IND;
	cb.func = NULL;	
	Btsdk_RegisterCallback4ThirdParty(&cb);

	cb.type = BTSDK_INQUIRY_RESULT_IND;
	cb.func = NULL;
	Btsdk_RegisterCallback4ThirdParty(&cb);

	Btsdk_RegisterGetStatusInfoCB4ThirdParty(NULL);

	DeinitBthSdk();
}

// ---------------------------------------------------------
// QBtDeviceDiscovererPrivate::startSearch()
// Called to start searching for new devices
// ---------------------------------------------------------
//
void QBtDeviceDiscovererPrivate::DiscoverDevices()
{
	/*if(isSearching)
	{
		emit p_ptr->error(QBtDeviceDiscoverer::BluetoothInUse);
		return;
	}*/
	if( isSearching )
		StopDiscovery();

	deviceList.clear();

	sp_ptr = p_ptr;

//	browseDevEventHandler = CreateEvent(NULL, FALSE, FALSE, L"completeBrowseDevice");

	// the first zero means that all device classes are searched
	// and the second that the discovery can find unlimited number
	// of devices
	if (BTSDK_OK == Btsdk_StartDeviceDiscovery(0, 100, 6))
	{
		isSearching = true;
		emit p_ptr->discoveryStarted();
//		WaitForSingleObject(browseDevEventHandler, INFINITE);
	}
	else
		emit p_ptr->error(QBtDeviceDiscoverer::UnknownError);
}

// ---------------------------------------------------------
// QBtDeviceDiscovererPrivate::startSearch()
// Called to stop searching
// ---------------------------------------------------------
//
void QBtDeviceDiscovererPrivate::StopDiscovery()
{
	Btsdk_StopDeviceDiscovery();
}

QBtDevice::List QBtDeviceDiscovererPrivate::GetInquiredDevices()
{
	return deviceList;
}


void QBtDeviceDiscovererPrivate::ReportInquiryResult(BTDEVHDL dev_hdl)
{
	RegisterFoundDevice(dev_hdl);
}

void QBtDeviceDiscovererPrivate::InquiryCompleteResult(void)
{
//	browseDevEventHandler = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"completeBrowseDevice");
//	SetEvent(browseDevEventHandler);

	if(sp_ptr)
		emit sp_ptr->discoveryStopped();

	isSearching = false;
}

void QBtDeviceDiscovererPrivate::RegisterFoundDevice(BTDEVHDL dev_hdl)
{
	BTUINT8 szDevName[BTSDK_DEVNAME_LEN] = { 0 };
	BTUINT16 usLen = 0;
	BTUINT32 ulDevClass = 0;
	BTUINT8 szBdAddr[6] = {0};
	QBtDevice devInfo;

	usLen = BTSDK_DEVNAME_LEN;

	BTINT32 result = BTSDK_FALSE;
	if ((result = Btsdk_GetRemoteDeviceName(dev_hdl, szDevName, &usLen)) != BTSDK_OK)
	{
		usLen = BTSDK_DEVNAME_LEN;
		ZeroMemory(szDevName, BTSDK_DEVNAME_LEN);
		if ((result = Btsdk_UpdateRemoteDeviceName(dev_hdl, szDevName, &usLen)) != BTSDK_OK)
		{
			strcpy((char*)szDevName, "Unknown");
		}
	}

	devInfo.setName(QString::fromUtf8((char*)szDevName));

	Btsdk_GetRemoteDeviceAddress(dev_hdl, szBdAddr);

	QByteArray btAddrr((char*)szBdAddr, 6);
	devInfo.setAddress(QBtAddress::getAddressFromReversedArray(btAddrr));

	Btsdk_GetRemoteDeviceClass(dev_hdl, &ulDevClass);
	
	if(ulDevClass & BTSDK_DEVCLS_COMPUTER)
		devInfo.setType(QBtDevice::Computer);
	else if(ulDevClass & BTSDK_DEVCLS_PHONE)
		devInfo.setType(QBtDevice::Phone);
	else if(ulDevClass & BTSDK_DEVCLS_LAP)
		devInfo.setType(QBtDevice::LANAccess);
	else if(ulDevClass & BTSDK_DEVCLS_AUDIO)
		devInfo.setType(QBtDevice::AudioVideo);
	else if(ulDevClass & BTSDK_DEVCLS_PERIPHERAL)
		devInfo.setType(QBtDevice::Peripheral);
	else if(ulDevClass & BTSDK_DEVCLS_IMAGE)
		devInfo.setType(QBtDevice::Imaging);
	else if(ulDevClass & BTSDK_DEVCLS_IMAGE)
		devInfo.setType(QBtDevice::Imaging);
	else if(ulDevClass & BTSDK_DEVCLS_WEARABLE)
		devInfo.setType(QBtDevice::Wearable);
	else
		devInfo.setType(QBtDevice::Uncategorized);

	deviceList.append(devInfo);

	if(sp_ptr)
		emit sp_ptr->newDeviceFound(devInfo);
}


bool QBtDeviceDiscovererPrivate::IsBusy () const
{
	return isSearching;
}