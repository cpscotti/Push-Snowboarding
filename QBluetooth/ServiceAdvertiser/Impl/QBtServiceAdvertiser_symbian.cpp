/*
 * QBtServiceAdvertiser_symbian.cpp
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

#include <btmanclient.h>
#include "../QBtServiceAdvertiser_symbian.h"
#include <QBtAuxFunctions.h>


QBtServiceAdvertiserPrivate* QBtServiceAdvertiserPrivate::NewL(QBtServiceAdvertiser* publicClass)
{
    QBtServiceAdvertiserPrivate* self = QBtServiceAdvertiserPrivate::NewLC(publicClass);
    CleanupStack::Pop(self);
    return self;
}


QBtServiceAdvertiserPrivate* QBtServiceAdvertiserPrivate::NewLC(QBtServiceAdvertiser* publicClass)
{
    QBtServiceAdvertiserPrivate* self = new (ELeave) QBtServiceAdvertiserPrivate(publicClass);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}


void QBtServiceAdvertiserPrivate::ConstructL()
{
}


// ----------------------------------------------------------------------------
// QBtServiceAdvertiserPrivate::QBtServiceAdvertiserPrivate(QBtServiceAdvertiser* publicClass)
//
// constructor
// ----------------------------------------------------------------------------
QBtServiceAdvertiserPrivate::QBtServiceAdvertiserPrivate(QBtServiceAdvertiser* publicClass)
	: localService(NULL), p_ptr(publicClass)
{
}


// ----------------------------------------------------------------------------
// QBtServiceAdvertiserPrivate::~QBtServiceAdvertiserPrivate()
//
// destructor
// ----------------------------------------------------------------------------
QBtServiceAdvertiserPrivate::~QBtServiceAdvertiserPrivate()
{
    TRAPD(err, StopAdvertiser());

    SafeDelete(localService);
}



// ----------------------------------------------------------------------------
// QBtServiceAdvertiserPrivate::StartAdvertiser(const QBtService& service)
//
// start service advertiser on given channel.  an entry to service discovery
// database will be entered describing our advertised service.
// ----------------------------------------------------------------------------
void QBtServiceAdvertiserPrivate::StartAdvertiser(const QBtService& service)
{
    localService = new QBtService(service);

    // open sdp session
    User::LeaveIfError(iSdp.Connect());
    // open sdp database session
    User::LeaveIfError(iSdpDB.Open(iSdp));
    // create a record of the correct service class
    TUUID serviceUUID(localService->getClass());
    iSdpDB.CreateServiceRecordL(serviceUUID, iRecord);

    // add a protocol to the record
    CSdpAttrValueDES* protocolDescriptorList = CSdpAttrValueDES::NewDESL(NULL);
    CleanupStack::PushL(protocolDescriptorList);

    TBuf8<1> channel;
    channel.Append((TChar)localService->getPort());



    // create protocol list for our service
    protocolDescriptorList->StartListL()->BuildDESL();

    QBtService::ProtocolList protocolList = service.getProtocols();
    for(int i=0; i<protocolList.size(); i++)
    {
        protocolDescriptorList->StartListL();
            protocolDescriptorList->BuildUUIDL(protocolList[i]);
            if(protocolList[i] == QBtConstants::RFCOMM)
                protocolDescriptorList->BuildUintL(channel);
        protocolDescriptorList->EndListL();
    }
    protocolDescriptorList->EndListL();

    // set protocol list to the record
    iSdpDB.UpdateAttributeL(iRecord, KSdpAttrIdProtocolDescriptorList,
            *protocolDescriptorList);
    CleanupStack::PopAndDestroy(protocolDescriptorList);

    TPtrC16 servName(localService->getName().utf16());

    // add a name to the record
    iSdpDB.UpdateAttributeL(iRecord,
            KSdpAttrIdBasePrimaryLanguage +
            KSdpAttrIdOffsetServiceName,
            servName);

    TPtrC16 servDescr(localService->getDescription().utf16());
    // add a description to the record
    iSdpDB.UpdateAttributeL(iRecord,
            KSdpAttrIdBasePrimaryLanguage +
            KSdpAttrIdOffsetServiceDescription,
            servDescr);

    // set service available
    UpdateAvailability(ETrue);
}


// ----------------------------------------------------------------------------
// QBtServiceAdvertiserPrivate::StopAdvertiser()
//
// stop service advertiser.  delete service record from service discovery
// database to stop advertising.
// ----------------------------------------------------------------------------
void QBtServiceAdvertiserPrivate::StopAdvertiser()
{
    if ( iRecord!=0 )
    {
        // delete out record from service discovery database
        iSdpDB.DeleteRecordL(iRecord);
        // close sdp and sdp db sessions
        iSdpDB.Close();
        iSdp.Close();
        iRecord=0;
    }
}


// ----------------------------------------------------------------------------
// QBtServiceAdvertiserPrivate::UpdateAvailability(TBool aAvailable)
//
// set availability of our advertised service.  the service record on the
// service discovery database will be updated accordingly.
// ----------------------------------------------------------------------------
void QBtServiceAdvertiserPrivate::UpdateAvailability(TBool aAvailable)
{
    TInt state = aAvailable ? 0xFF : 0x00;
    // set availability
    iSdpDB.UpdateAttributeL(iRecord, KSdpAttrIdServiceAvailability, state);
    // mark record changed
    iSdpDB.UpdateAttributeL(iRecord, KSdpAttrIdServiceRecordState,
            ++iRecordState);
}



