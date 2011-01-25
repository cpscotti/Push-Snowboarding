/*
 * QBtServiceDiscoverer_symbian.cpp
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

#include "../QBtServiceDiscoverer_symbian.h"
#include <btsdp.h>
#include <e32des8.h>

#include <QMessageBox>



#define IF_SET(a) if(iLastUUID == TUUID(a)) \
						currentService->addProtocol(a)
#define ELSEIF_SET(a) else if(iLastUUID == TUUID(a)) \
						currentService->addProtocol(a)

QBtServiceDiscovererPrivate* QBtServiceDiscovererPrivate::NewL(QBtServiceDiscoverer* publicClass)
{
	QBtServiceDiscovererPrivate* self = QBtServiceDiscovererPrivate::NewLC(publicClass);
	CleanupStack::Pop(self);
	return self;
}


QBtServiceDiscovererPrivate* QBtServiceDiscovererPrivate::NewLC(QBtServiceDiscoverer* publicClass)
{
	QBtServiceDiscovererPrivate* self = new (ELeave) QBtServiceDiscovererPrivate(publicClass);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}


void QBtServiceDiscovererPrivate::ConstructL()
{
}

QBtServiceDiscovererPrivate::QBtServiceDiscovererPrivate(QBtServiceDiscoverer* publicClass)
	: iAgent(NULL),
	  iSpat(NULL),
	  device(NULL),	  
	  uuidIndex (0),	  
	  attrIndex(0),
	  currentServiceProcessing(0),	  
	  p_ptr(publicClass),
	  discoveryInProgress (false),
	  multipleSearching(false)
{
	uuidList.append(QBtConstants::SerialPort);
	uuidList.append(QBtConstants::OBEXObjectPush);
	uuidList.append(QBtConstants::OBEXFileTransfer);
	//uuidList.append((QBtConstants::ServiceClass)0x0100); //L2CAP
	
	attrList.append(KSdpAttrIdProtocolDescriptorList);
	attrList.append(KSdpAttrIdBasePrimaryLanguage + KSdpAttrIdOffsetServiceName);
	attrList.append(KSdpAttrIdBasePrimaryLanguage + KSdpAttrIdOffsetServiceDescription);
}

QBtServiceDiscovererPrivate::~QBtServiceDiscovererPrivate()
{
	// must not throw/leave
	TRAPD (error, StopDiscovery() );
}


// ----------------------------------------------------------------------------
// QBtServiceDiscovererPrivate::DiscoverServicesOnDevice(QBtDevice* targetDevice)
//
// discover services on given device.  a service discovery agent will be
// started to do the discovery of services on given remote device.
// Every service contained in UUIDList will be searched at the remote
// device's SDP database.
// ----------------------------------------------------------------------------
void QBtServiceDiscovererPrivate::DiscoverServicesOnDevice(QBtDevice* targetDevice)
{	
	_StopDiscovery();	
	discoveryInProgress = true;

	uuidIndex = 0;
	multipleSearching = ETrue;
	device = targetDevice;
	QBtService::List* newServiceList = new QBtService::List();
	device->setSupportedServices(*newServiceList); // clear list
	
	if(uuidList.size() > 0)
		DiscoverSpecificClass(targetDevice, uuidList[uuidIndex]);
}

// ----------------------------------------------------------------------------
// QBtServiceDiscovererPrivate::DiscoverSpecificClass(
//											QBtDevice* targetDevice, 
//											QBtConstants::ServiceClass uuid)
//
// discover services on given device.  a service discovery agent will be
// started to do the discovery of services on given remote device which 
// match to the given uuid
// ----------------------------------------------------------------------------
void QBtServiceDiscovererPrivate::DiscoverSpecificClass(
								QBtDevice* targetDevice, 
								QBtConstants::ServiceClass uuid)
{	
	_StopDiscovery();	
	discoveryInProgress = true;
	
	if(!multipleSearching)
	{
		device = targetDevice;
		QBtService::List* newServiceList = new QBtService::List();
		device->setSupportedServices(*newServiceList); // clear list
	}
	
	/*const TUint8* memPtr = (const TUint8*)device->getAddress().toByteArray().constData();
	
	TPtrC8 devAddrPtr8;
	devAddrPtr8.Set(memPtr, 6);*/

	const TBTDevAddr devAddress = device->getAddress().convertToSymbianBtDevAddr();

	// init new service discovery agent
	iAgent = CSdpAgent::NewL( *this, devAddress);

	// set search properties for agent
	iSpat = CSdpSearchPattern::NewL();
	// use our service id to filter the services discovered
	// -> will return only the services with matching service id(s)
	iSpat->AddL(TUUID((TUint32)uuid));
	iAgent->SetRecordFilterL(*iSpat);


	// initiate search
	// this will result in call to NextRecordRequestComplete()
	iAgent->NextRecordRequestL();
}

// ----------------------------------------------------------------------------
// QBtServiceDiscovererPrivate::DiscoverSpecificProtocol(
//											QBtDevice* targetDevice, 
//											QBtConstants::ServiceProtocol uuid)
//
// discover services on given device.  a service discovery agent will be
// started to do the discovery of services on given remote device which 
// match to the given uuid
// ----------------------------------------------------------------------------
void QBtServiceDiscovererPrivate::DiscoverSpecificProtocol(
		QBtDevice* targetDevice, 
		QBtConstants::ServiceProtocol uuid)
{	
	DiscoverSpecificClass(targetDevice, (QBtConstants::ServiceClass)uuid);
}

// ----------------------------------------------------------------------------
// QBtServiceDiscovererPrivate::StopDiscovery()
//
// stop discovering services, stop service discovery agent.
// ----------------------------------------------------------------------------
void QBtServiceDiscovererPrivate::StopDiscovery()
{
	if (!discoveryInProgress)
		return;
	
	
	if(iAgent && iSpat)
	//{ emit p_ptr->discoveryStopped("StopDiscovery"); }
		{ discoveryInProgress = false; QT_TRYCATCH_LEAVING (emit p_ptr->discoveryStopped() ) ; }
	
	_StopDiscovery();
}


// ----------------------------------------------------------------------------
// QBtServiceDiscovererPrivate::StopDiscovery()
//
// stop discovering services, stop service discovery agent.
// ----------------------------------------------------------------------------
void QBtServiceDiscovererPrivate::_StopDiscovery()
{
	if(iAgent)
	{
		iAgent->Cancel();
		delete iAgent;
		iAgent=NULL;
	}
	if(iSpat)
	{
		iSpat->Reset();
		delete iSpat;
		iSpat=NULL;
	}
}



// ----------------------------------------------------------------------------
// QBtServiceDiscovererPrivate::NextRecordRequestComplete(
//      TInt aError,
//      TSdpServRecordHandle aHandle,
//      TInt aTotalRecordsCount)
//
// called when the service discovery agent has completed discovering services
// on device.  now the attributes of the found service records (if any) must
// be evaluated.  if no service records were discovered, proceed doing
// service discovery on next device.
// ----------------------------------------------------------------------------
void QBtServiceDiscovererPrivate::NextRecordRequestComplete(
		TInt aError,
		TSdpServRecordHandle aHandle,
		TInt aTotalRecordsCount)
{	
	if ( aError == KErrEof )
	{
		SearchNextUUIDorReportCompletion();
		return;
	}

	if ( aError != KErrNone )
	{
		//discovering new service on target device
		QT_TRYCATCH_LEAVING (emit p_ptr->error(QBtServiceDiscoverer::ServiceDiscoveryNotAbleToComplete) );
		
		//emit p_ptr->discoveryStopped("Error-ServiceDiscoveryNotAbleToComplete");		
		QT_TRYCATCH_LEAVING(emit p_ptr->discoveryStopped() );
		discoveryInProgress = false;
		return;
	}

	if ( aTotalRecordsCount == 0 )
	{
		SearchNextUUIDorReportCompletion();
		return;
	}

	//a new service is found
	QBtService* newService = new QBtService();
	newService->setHandle(aHandle);
	newService->setClass(uuidList[uuidIndex]);

	handleMap.insert(aHandle, newService);
	
	attrIndex = 0;

	if(iAgent)
		iAgent->AttributeRequestL(aHandle, attrList[attrIndex]);
}


// ----------------------------------------------------------------------------
// QBtServiceDiscovererPrivate::AttributeRequestResult(
//      TSdpServRecordHandle aHandle,
//      TSdpAttributeID aAttrID,
//      CSdpAttrValue* aAttrValue)
//
// called when the service attributes for the service record have been
// retrieved.
// ----------------------------------------------------------------------------
void QBtServiceDiscovererPrivate::AttributeRequestResult(
		TSdpServRecordHandle aHandle,
		TSdpAttributeID aAttrID,
		CSdpAttrValue* aAttrValue)
{
	// parse attributes, will result in call to VisitAttributeValue()
	currentServiceProcessing = aHandle;
	TRAPD(err,aAttrValue->AcceptVisitorL(*this) );
	if( err != KErrNone)
	{
		// error retrieving service attributes
		QT_TRYCATCH_LEAVING(emit p_ptr->error(QBtServiceDiscoverer::UnableToRetrieveServiceAttributes) ); 
	}

	delete aAttrValue;
	aAttrValue = NULL;
}


// ----------------------------------------------------------------------------
// QBtServiceDiscovererPrivate::AttributeRequestComplete(
//      TSdpServRecordHandle aHandle,
//      TInt aError)
//
// called when the request to resolve the service attributes for the service
// record completes.  if there are more service records, proceed resolving
// the next service record.
// ----------------------------------------------------------------------------
void QBtServiceDiscovererPrivate::AttributeRequestComplete (TSdpServRecordHandle aHandle, TInt aError)
{
	if ( aError==KErrNone )
	{
		if(++attrIndex < attrList.size())
		{
			try
			{
				if(iAgent)
					iAgent->AttributeRequestL(aHandle, attrList[attrIndex]);				
				else
					QT_TRYCATCH_LEAVING (emit p_ptr->error(QBtServiceDiscoverer::UnknownError) );
			}
			catch(char* str)
			{
				QT_TRYCATCH_LEAVING (emit p_ptr->error(QBtServiceDiscoverer::UnableToRetrieveServiceAttributes) );
			}
		}
		else
		{
			// done with attributes for this record, request next
			// service record
			device->addNewService(*(handleMap[aHandle]));
			QT_TRYCATCH_LEAVING (emit p_ptr->newServiceFound(*device, *(handleMap[aHandle])) );
	
			try
			{
				if(iAgent)
				 iAgent->NextRecordRequestL();				
				else
					QT_TRYCATCH_LEAVING (emit p_ptr->error(QBtServiceDiscoverer::UnknownError) );
			}
			catch(char* str)
			{
				//error requesting next service
				QT_TRYCATCH_LEAVING (emit p_ptr->error(QBtServiceDiscoverer::ProblemRequestingNextServiceRecord) ); 
			}
		}
	}
	else
	{
		// error acquiring service's next attribute
		QT_TRYCATCH_LEAVING (emit p_ptr->error(QBtServiceDiscoverer::ProblemAcquiringNextServiceAttributes) ); 
	}
}


// ----------------------------------------------------------------------------
// QBtServiceDiscovererPrivate::VisitAttributeValueL(
//      CSdpAttrValue &aValue,
//      TSdpElementType aType)
//
// called for processing of each service attribute.  here we must look for
// attributes of UUID type.  if the UUID is RFCOMM UUID, resolve the value
// for this attribute, which will be channel number to be used for connection
// to remote device.
// ----------------------------------------------------------------------------
void QBtServiceDiscovererPrivate::VisitAttributeValueL(
		CSdpAttrValue &aValue,
		TSdpElementType aType)
{
	//QString* str;
	QBtService* currentService = handleMap[currentServiceProcessing];
	switch (aType)
	{
		case ETypeUUID:
		{
			TPtrC8 uuid(aValue.UUID().ShortestForm());//.FixedLengthL(2));
			iLastUUID.SetL(uuid);

			IF_SET(QBtConstants::SDP);
			ELSEIF_SET(QBtConstants::UDP);
			ELSEIF_SET(QBtConstants::RFCOMM);
			ELSEIF_SET(QBtConstants::TCP);
			ELSEIF_SET(QBtConstants::TCS_BIN);
			ELSEIF_SET(QBtConstants::TCS_AT);
			ELSEIF_SET(QBtConstants::OBEX);
			ELSEIF_SET(QBtConstants::IP);
			ELSEIF_SET(QBtConstants::FTP);
			ELSEIF_SET(QBtConstants::HTTP);
			ELSEIF_SET(QBtConstants::WSP);
			ELSEIF_SET(QBtConstants::BNEP);
			ELSEIF_SET(QBtConstants::UPNP);
			ELSEIF_SET(QBtConstants::HIDP);
			ELSEIF_SET(QBtConstants::HardcopyControlChannel);
			ELSEIF_SET(QBtConstants::HardcopyDataChannel);
			ELSEIF_SET(QBtConstants::HardcopyNotification);
			ELSEIF_SET(QBtConstants::AVCTP);
			ELSEIF_SET(QBtConstants::AVDTP);
			ELSEIF_SET(QBtConstants::CMPT);
			ELSEIF_SET(QBtConstants::UDI_C_Plane);
			ELSEIF_SET(QBtConstants::L2CAP);
			
			break;
		}

		case ETypeUint:
		{
			if ( iLastUUID== TUUID(QBtConstants::RFCOMM) )
			{
				// previous call to this method with rfcomm UUID, therefore
				// this one will be the value, rfcomm service channel (port)
                currentService->setPort(aValue.Uint());
			}
			break;
		}
		case ETypeString:
		{
			//str = new QString((const char*)aValue.Des().Ptr());
			//CSdpAttrValueString* attrStr = (CSdpAttrValueString*) (&aValue);			
			//str = QString::fromUtf8(attrStr->Des().Ptr() , attrStr->Des().Length() );
			
			QString str = QString::fromUtf8( (const char*)aValue.Des().Ptr(), aValue.Des().Length() );			
			
			
			if(attrList[attrIndex] == (KSdpAttrIdBasePrimaryLanguage + KSdpAttrIdOffsetServiceName))
				currentService->setName(str);
			else
				currentService->setDescription(str);
			
			break;			
		}
		
		default:
		{
			break;
		}
	}
}


void QBtServiceDiscovererPrivate::StartListL(CSdpAttrValueList& /*aList*/)
{
	// not needed
}

void QBtServiceDiscovererPrivate::EndListL()
{
	// not needed
}

// inline
void QBtServiceDiscovererPrivate::SearchNextUUIDorReportCompletion()
{
	if(multipleSearching)
	{
		if((++uuidIndex < uuidList.size()))
			DiscoverSpecificClass(device, uuidList[uuidIndex]);
		else
			//{  emit p_ptr->discoveryStopped("[qdll] search next uiid I"); }
			{  discoveryInProgress = false; QT_TRYCATCH_LEAVING (emit p_ptr->discoveryStopped() );  }
	}
	else
		//{ emit p_ptr->discoveryStopped("[qdll] multiple search=false"); }
		{ discoveryInProgress = false; QT_TRYCATCH_LEAVING (emit p_ptr->discoveryStopped() ); }
}


bool QBtServiceDiscovererPrivate::IsBusy() const
{
	return discoveryInProgress;
}
