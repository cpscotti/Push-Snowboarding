/*
 * QBtServiceDiscoverer_symbian.h
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

#ifndef QBTSERVICEDISCOVERER_SYMBIAN_H_
#define QBTSERVICEDISCOVERER_SYMBIAN_H_

#include <e32base.h>

#include <coecntrl.h>
#include <es_sock.h>
#include <btdevice.h>
#include <bt_sock.h>
#include <btsdp.h>
#include <QMap>

#include <QBtDevice.h>
#include <QBtServiceDiscoverer.h>

class QBtServiceDiscovererPrivate : public CBase,
public MSdpAgentNotifier,
public MSdpAttributeValueVisitor
{
public:

    /*!
     * NewL()
     *
     *   Create new QBtServiceDiscovererPrivate object
     * return a pointer to the created instance of QBtServiceDiscovererPrivate
     */
    static QBtServiceDiscovererPrivate* NewL(QBtServiceDiscoverer* publicClass);

    /*!
     * NewLC()
     *
     */
    static QBtServiceDiscovererPrivate* NewLC(QBtServiceDiscoverer* publicClass);

    /*!
     * ~QBtServiceDiscovererPrivate()
     *
     *   Destroy the object and release all memory objects
     */
    ~QBtServiceDiscovererPrivate();

    /*!
     * DiscoverServiceOnDeviceL()
     *
     *   Discovers services on a given device.  service discovery
     * agent will be started to do the service discovery.  search pattern
     * will be used to limit the discovery for our services (matching service
     * id).
     *
     * param aDevData device data record
     */
    void DiscoverServicesOnDevice(QBtDevice* targetDevice);

    /*
     * DiscoverSpecificProtocol(QBtDevice* targetDevice, QBtConstants::ServiceProtocol uuid)
     *
     *	Start service discovery for specific protocol
     */
    void DiscoverSpecificProtocol(QBtDevice* targetDevice, QBtConstants::ServiceProtocol uuid);

    /*
     * DiscoverSpecificProtocol(QBtDevice* targetDevice, QBtConstants::ServiceClass uuid)
     *
     *	Start service discovery for specific service
     */
    void DiscoverSpecificClass(QBtDevice* targetDevice, QBtConstants::ServiceClass uuid);

    /*!
     * FinishDiscovery()
     *
     *   Stops the service discovery, stops the discovery agent.
     *
     * param aDevData device data record
     */
    void StopDiscovery();

    
    /**
     * 
     */
    bool IsBusy() const;

private:

    /*!
     * QBtServiceDiscovererPrivate()
     *
     *   Perform the first phase of two phase construction
     */
    QBtServiceDiscovererPrivate(QBtServiceDiscoverer* publicClass);

    /*!
     * ConstructL()
     *
     */
    void ConstructL();

    /*!
     * HandleServiceDiscoveryCompleteL()
     *
     *   Handles the service discovery completed event.  the handling
     * request will be passed onto handling observer.
     */
    void HandleServiceDiscoveryCompleteL();

    /*
     * NextRecordRequestComplete()
     *
     *   called when an service record
     *            (CSdpAgent::NextRecordRequestL()) operation completes.
     * param aError KErrNone, or an SDP error
     * param aHandle service record for which the query was made
     * param aTotalRecordsCount total number of matching records
     */
    void NextRecordRequestComplete(
            TInt aError,
            TSdpServRecordHandle aHandle,
            TInt aTotalRecordsCount);

    /*
     * AttributeRequestResult()
     *
     *   Called by the attribute request
     *            (CSdpAgent::AttributeRequestL()) to pass the results of
     *            a successful attribute request.
     *
     * param aHandle service record for which the query was made
     * param aAttrID ID of the attribute obtained
     * param aAttrValue attribute value obtained
     */
    void AttributeRequestResult(
            TSdpServRecordHandle aHandle,
            TSdpAttributeID aAttrID,
            CSdpAttrValue* aAttrValue);

    /*!
     * AttributeRequestComplete()
     *
     *   Called when the attribute request
     *            (CSdpAgent::AttributeRequestL()) completes.
     *
     * param aHandle service record for which the query was made
     * param aError KErrNone, or an SDP error
     */
    void AttributeRequestComplete(
            TSdpServRecordHandle aHandle,
            TInt aError);

    /*!
     * VisitAttributeValueL()
     *
     *   Called to pass the attribute request values
     *
     * param aValue attribute value
     * param aType attribute type
     */
    void VisitAttributeValueL(CSdpAttrValue& aValue, TSdpElementType aType);
    // not needed -> not implemented
    void StartListL(CSdpAttrValueList &aList);
    // not needed -> not implemented
    void EndListL();


    inline void SearchNextUUIDorReportCompletion();
    
    
    void _StopDiscovery();

private:    // data

    // service discovery agent
    CSdpAgent* iAgent;
    // service discovery search pattern
    CSdpSearchPattern* iSpat;
    // last discovered uuid in the service attributes
    TUUID iLastUUID;
    // port (comm channel) found in the service attributes
    TUint iPort;
    // device data record reference
    QBtDevice* device;

    // used when multiple search is on
    QList<QBtConstants::ServiceClass> uuidList;
    // used to get multiple information on one service
    QList<TSdpAttributeID> attrList;
    // index of the service UUID currently searching for
    TInt uuidIndex;
    // index of the attribute currently searching for
    TInt attrIndex;


    TSdpServRecordHandle currentServiceProcessing;


    //aux variables
    QMap<TSdpServRecordHandle, QBtService*> handleMap;

    //pointer to parent object (from constructor). Not owned by this class
    QBtServiceDiscoverer *p_ptr;
    
    // if the process has been started
    bool discoveryInProgress;
    
    // indicates if DiscoverServicesOnDeviceL is called (true if it is)
    // Searches device for services matching to one of the uuidList UUIDs
    TBool multipleSearching;    
};

#endif /* QBTSERVICEDISCOVERER_SYMBIAN_H_ */
