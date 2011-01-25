/*
 * QBtDeviceDiscoverer_symbian.cpp
 *
 *  
 *      Author: Ftylitakis Nikolaos, Luis Valente
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
 
#include "../QBtDeviceDiscoverer_symbian.h"
#include "../QBtDeviceDiscoverer.h"
#include <bt_sock.h> 
#include <bttypes.h>

_LIT(KBTLinkManagerTxt,"BTLinkManager");

QBtDeviceDiscovererPrivate* QBtDeviceDiscovererPrivate::NewL(QBtDeviceDiscoverer *publicClass)
{
	QBtDeviceDiscovererPrivate* self = QBtDeviceDiscovererPrivate::NewLC(publicClass);
	CleanupStack::Pop(self);
	return self;
}


QBtDeviceDiscovererPrivate* QBtDeviceDiscovererPrivate::NewLC(QBtDeviceDiscoverer *publicClass)
{
	QBtDeviceDiscovererPrivate* self = new (ELeave) QBtDeviceDiscovererPrivate(publicClass);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}


void QBtDeviceDiscovererPrivate::ConstructL()
{
	User::LeaveIfError(iSocketServ.Connect());
	TProtocolName protocolName(KBTLinkManagerTxt);
	User::LeaveIfError(iSocketServ.FindProtocol(protocolName,iProtocolInfo)); 
}


// ----------------------------------------------------------------------------
// CDeviceDiscoverer::CDeviceDiscoverer(RSocketServ *aSocketServ,
//                                      MDeviceDiscoObserver *aObserver)
//
// constructor
// ----------------------------------------------------------------------------
QBtDeviceDiscovererPrivate::QBtDeviceDiscovererPrivate (QBtDeviceDiscoverer *publicClass)
:	CActive (CActive::EPriorityStandard),
 	sizeOfListeningQueue(4),
 	p_ptr(publicClass),
 	iLIAC (false),
 	iIsBusy (false)
{
	CActiveScheduler::Add(this);
	iDevList = new QBtDevice::List();
}


// ----------------------------------------------------------------------------
// CDeviceDiscoverer::~CDeviceDiscoverer()
//
// destructor
// ----------------------------------------------------------------------------
QBtDeviceDiscovererPrivate::~QBtDeviceDiscovererPrivate()
{ 
	// cancel active object
	if(IsActive())
		Cancel();
	
	//iResolver.Close();
	delete iDevList;
}


// ----------------------------------------------------------------------------
// CDeviceDiscoverer::DiscoverDevicesL(TDeviceDataList *aDevDataList)
//
// discover remote devices.  RHostResolver will be used to do the discovery.
// any found devices will be placed in aDevDataList.
// ----------------------------------------------------------------------------
void QBtDeviceDiscovererPrivate::DiscoverDevices()
{
	if (!IsActive())
	{
		// initialize host resolver
		// load protocol for discovery
		iResolver.Close();
		TInt err = iResolver.Open(iSocketServ, iProtocolInfo.iAddrFamily, iProtocolInfo.iProtocol);
		if (err) 
		{
			EmitErrorSignal (QBtDeviceDiscoverer::BluetoothInUse);
			return;
		}
		
		// set as busy
		iIsBusy = true;

		// wipe existing device data list, start fresh
		iDevList->clear();

		// start device discovery by invoking remote address lookup
		TUint myIAC(iLIAC ? KLIAC : KGIAC);
		iAddr.SetIAC(myIAC);

		iAddr.SetAction(KHostResInquiry|KHostResName|KHostResIgnoreCache);
		iResolver.GetByAddress(iAddr, iEntry, iStatus);
		emit p_ptr->discoveryStarted();
		SetActive();
	}
	else
	{
		EmitErrorSignal (QBtDeviceDiscoverer::BluetoothInUse) ;		
		return;
	}
}


void QBtDeviceDiscovererPrivate::RunL()
{
	// RHostResolver.GetByAddress(..) has completed, process results
	if ( iStatus == KErrNone )
	{
		TBTSockAddr address = iEntry().iAddr;
		TBTDevAddr btAddress = address.BTAddr();
		
		//QByteArray btAddressArray((const char*)btAddress.Des().Ptr(), 6); 
		QBtAddress qtBtDeviceAddress(btAddress);
		
		THostName nameDevice = iEntry().iName;
		QString qtNameDevice = QString::fromUtf16(nameDevice.Ptr(),nameDevice.Length());        

		TInquirySockAddr& sa = TInquirySockAddr::Cast( iEntry().iAddr );
		//TUint8 minorClass = sa.MinorClassOfDevice();
		//quint8 qMinorClass = quint8(minorClass);  
		
		//Set major device class
		TUint16 majorClass = sa.MajorClassOfDevice();
		QBtDevice::DeviceMajor qtDeviceMajorClass;
		switch (majorClass) {
		case 0x00: //computer
			qtDeviceMajorClass = QBtDevice::Miscellaneous;
			break;	     
		case 0x01:
			qtDeviceMajorClass = QBtDevice::Computer;
			break;	
		case 0x02:
			qtDeviceMajorClass = QBtDevice::Phone;
			break;
		case 0x03:
			qtDeviceMajorClass = QBtDevice::LANAccess;
			break;   	
		case 0x04:
			qtDeviceMajorClass = QBtDevice::AudioVideo;
			break;         	
		case 0x05:
			qtDeviceMajorClass = QBtDevice::Peripheral;
			break;     
		case 0x06:
			qtDeviceMajorClass = QBtDevice::Imaging;
			break;   
		default:
			qtDeviceMajorClass = QBtDevice::Uncategorized;
			break;
		}
		
		//QBtDevice* remoteDevice = new QBtDevice(qtNameDevice, qtBtDeviceAddress, qtDeviceMajorClass);
		QBtDevice remoteDevice (qtNameDevice, qtBtDeviceAddress, qtDeviceMajorClass);		
		QT_TRYCATCH_LEAVING (EmitNewDeviceFoundSignal (remoteDevice));
		
		
		// store on list
		iDevList->append (remoteDevice);

		
		// next
		iResolver.Next(iEntry,iStatus);
		SetActive();
	}
	else if (iStatus == KErrHostResNoMoreResults) {
        // Note emit may throw. We translate to Leave here.
		QT_TRYCATCH_LEAVING (EmitDiscoveryStoppedSignal() );
	}
    else {
    	QT_TRYCATCH_LEAVING (EmitDiscoveryStoppedSignal() );
		//ErrorConvertToLocalL(iStatus.Int());
    }
}

void QBtDeviceDiscovererPrivate::DoCancel()
{
	// Note that must trap any errors here - Cancel is called in destructor and destructor must not throw.
	QT_TRY
	{
		EmitDiscoveryStoppedSignal();
	}
	QT_CATCH (std::exception& e)
	{}
	
	iResolver.Cancel();
}

// ----------------------------------------------------------------------------
// CDeviceDiscoverer::HasDevices()
//
// returns true if any devices were discovered
// ----------------------------------------------------------------------------
TBool QBtDeviceDiscovererPrivate::HasDevices()
{
	//TBool exists = EFalse;
	return iDevList->size() > 0;
}

// ----------------------------------------------------------------------------
// CBluetoothPMPExampleEngine::SetLIAC
// ----------------------------------------------------------------------------
void QBtDeviceDiscovererPrivate::SetLIAC( TBool aState )
{
	iLIAC = aState;
}

// ----------------------------------------------------------------------------
// CBluetoothPMPExampleEngine::StopDiscovery
// ----------------------------------------------------------------------------
void QBtDeviceDiscovererPrivate::StopDiscovery()
{
	if (IsActive()) 
		Cancel();
	else
	{
		// Note, may throw. That is OK, stopSearch() is called from Qt code only.
		EmitErrorSignal (QBtDeviceDiscoverer::BluetoothAlreadyStopped) ;
	}
}

// ----------------------------------------------------------------------------
// QBtDeviceDiscovererPrivate::GetInquiredDevices
// Gets the list of devices that where found during the last search
// ----------------------------------------------------------------------------
const QBtDevice::List& QBtDeviceDiscovererPrivate::GetInquiredDevices() const
{
	return *iDevList;	
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TBool QBtDeviceDiscovererPrivate::IsBusy() const
{
	return iIsBusy;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void QBtDeviceDiscovererPrivate::EmitErrorSignal (QBtDeviceDiscoverer::DeviceDiscoveryErrors error)
{
	try
	{
		emit p_ptr->error (error);
		iIsBusy = false;
	}
	catch (...)
	{
		iIsBusy = false;
		throw;
	}
	
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void QBtDeviceDiscovererPrivate::EmitDiscoveryStoppedSignal()
{
	try
	{
		emit p_ptr->discoveryStopped();
		iIsBusy = false;
	}
	catch (...)
	{
		iIsBusy = false;
		throw;
	}
		
	
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void QBtDeviceDiscovererPrivate::EmitNewDeviceFoundSignal (const QBtDevice & device)
{
	try
	{
		emit p_ptr->newDeviceFound (device);		
		iIsBusy = false;
	}
	catch (...)
	{
		iIsBusy = false;
		throw;
	}
	
}
