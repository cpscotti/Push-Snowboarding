/*
 * QBtDeviceDiscoverer_symbian.h
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

#ifndef QBTDEVICEDISCOVERER_SYMBIAN_H_
#define QBTDEVICEDISCOVERER_SYMBIAN_H_

#include <e32base.h>
#include <ES_SOCK.H>
#include <bt_sock.h>

#include <QBtDevice.h>
#include <QBtDeviceDiscoverer.h>

class QBtDeviceDiscovererPrivate : public CActive
{
public:

/*!
 * NewL()
 * Create new CDeviceDiscoverer object
 * return a pointer to the created instance of CDeviceDiscoverer
 */
    static QBtDeviceDiscovererPrivate* NewL(QBtDeviceDiscoverer *publicClass);

/*!
 * NewLC()
 */
    static QBtDeviceDiscovererPrivate* NewLC(QBtDeviceDiscoverer *publicClass);

/*!
 * ~CDeviceDiscoverer()
 *	the object and release all memory objects
 */
    ~QBtDeviceDiscovererPrivate();

/*!
 * DiscoverDevicesL()
 * Discovers remote bluetooth devices within range.
 * param aDevDataList device data list where the found devices will be placed.
 */
    void DiscoverDevices();

/*!
 * StopDiscovery()
 * Stops ongoing device discovery
 *
 */
    void StopDiscovery();

/*!
 * HasDevices()
 * any devices discovered?
 * return true if any devices were discovered.
 */
    TBool HasDevices();

/*!
 * SetLIAC()
 * Set LimitedDiscoverableStatus status
 *
 */
    void SetLIAC( TBool aState );

    /*
     * Gets the list of devices that where found during the last search
     */
    const QBtDevice::List& GetInquiredDevices() const;
    
    
    /**
     * Returns true if the class is busy discovering devices.
     */
    TBool IsBusy() const;

protected:

/*!
 * RunL()
 * Handle the active objects request completion.
 */
    void RunL();

/*!
 * DoCancel()
 * Cancels the outstanding request on active object.
 */
    void DoCancel();


private:

/*!
 * CDeviceDiscoverer()
 * Perform the first phase of two phase construction
 * param aSocketServ reference to socket server
 */
    QBtDeviceDiscovererPrivate(QBtDeviceDiscoverer *publicClass);

/*!
 * ConstructL()
 */
    void ConstructL();
    
    
private:
    
    void EmitErrorSignal (QBtDeviceDiscoverer::DeviceDiscoveryErrors error);
    void EmitDiscoveryStoppedSignal();
    void EmitNewDeviceFoundSignal (const QBtDevice & device);

private: //data

    // socket server handle
    RSocketServ iSocketServ;
    // device data list, to be filled by resolver results
    QBtDevice::List *iDevList;
    // host resolver
    RHostResolver iResolver;
    TInquirySockAddr iAddr;
    TProtocolDesc iProtocolInfo;
    TNameEntry iEntry;
    TInt sizeOfListeningQueue;

    //pointer to parent object (from constructor). Not owned by this class
    QBtDeviceDiscoverer *p_ptr;
    
    TBool iLIAC;
    TBool iIsBusy;

};

#endif /* QBTDEVICEDISCOVERER_SYMBIAN_H_ */
