/*
 * QBtServiceDiscoverer.h
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

#ifndef QBTSERVICEDISCOVERER_H_
#define QBTSERVICEDISCOVERER_H_

#include <QBtGlobal.h>
#include <QObject>
#include <QBtTypes.h>

//forward declaration
class QBtServiceDiscovererPrivate;

/**
 * This class provides the mechanism to inquire a given device for the bluetooth
 * services it supports.
 *
 * If startDiscovery(QBtDevice*) is called, then the device is inquired for all
 * the bluetooth services it supports.
 *
 * If user is searching for a specific service then
 * startDiscovery(QBtDevice* ,QBtConstants::ServiceClass) or
 * startDiscovery(QBtDevice* , QBtConstants::ServiceProtocol)
 * can be called, according to the information the user already has
 * about the inquired service.
 * After the successfull call of these functions, discoveryStarted()
 * signal si emitted.
 *
 * At any of the above cases, a found service is reported through
 * newServiceFound(const QBtDevice&, QBtService) signal.
 *
 * If service inquiring stops (either by the user calling stopDiscovery() or by normal termination
 * of the inquiring) then discoveryStopped() signal is emitted.
 *
 * At any time the user can access the found services through
 * getInquiredServices().
 */
class DLL_EXPORT QBtServiceDiscoverer : public QObject
{
    Q_OBJECT

public:
    enum ServiceDiscoveryError
    {
        ServiceDiscoveryNotAbleToComplete,
        UnableToRetrieveServiceAttributes,
        ProblemRequestingNextServiceRecord,
        ProblemAcquiringNextServiceAttributes,
        NotSupported,
        NoDeviceSelected,
        UnknownError
    };

public:
    QBtServiceDiscoverer(QObject* parent = 0);
    virtual ~QBtServiceDiscoverer();

    /**
     *  getInquiredServices()
     *
     *  Returns a service list containing the inquired services (if any)
     */
    QBtService::List getInquiredServices();

    /**
     *  getTargetDevice()
     *
     *  Returns the remote device selected for service discovery.
     *  If "startDiscovery" function (both overloads) was already called
     *  then returns "targetDevice" which was passed as argument
     *  else returns NULL
     */
    const QBtDevice & getTargetDevice() const;
    
    /**
     * Returns true if a discovery operation is in progress.
     */
    bool isBusy () const;

public slots:

    /**
     * Start service discovery using predefined service UUID's. (OBEX, RFCOMM, OBEXFtp)
     * New services found are reported through signal "newServiceFound" or can
     * be aquired through "getInquiredServices" function
     *
     * @param targetDevice
     */
    void startDiscovery(const QBtDevice& targetDevice);

    /**
     * startDiscovery(QBtDevice* targetDevice, QBtConstants::ServiceClass uuid);
     *
     * Start service discovery using a specific service UUID.
     * New services found are reported through signal "newServiceFound" or can
     * be aquired through "getInquiredServices" function
     *
     * @param targetDevice
     * @param uuid
     */
    void startDiscovery (const QBtDevice& targetDevice, QBtConstants::ServiceClass uuid);

    /**
     * startDiscovery(QBtDevice* targetDevice, QBtConstants::ServiceClass uuid);
     *
     * Start service discovery using a specific service UUID.
     * New services found are reported through signal "newServiceFound" or can
     * be aquired through "getInquiredServices" function
     *
     * @param targetDevice
     * @param uuid
     */
    void startDiscovery (const QBtDevice & targetDevice, QBtConstants::ServiceProtocol uuid);

    /**
     * stopDiscovery();
     *
     * public slot. Stop service discovery.
     * Services found so far can be acquired using "getInquiredServices" function.
     */
    void stopDiscovery();

signals:

    /**
     * Signal to report that discovery has started. Alternative is error.
     */
    void discoveryStarted();
    /**
     * Signal to report that discovery has stopped (either cancelled or no new devices to detect)
     */
    void discoveryStopped ();

    /**
     * Signaled for every service detected
     * The targetDevice's field "supportedServices" is updated every time
     * this signal is emitted.
     *
     * @param targetDevice The remote device where the service was found
     * @param service	The remote service found
     */
    void newServiceFound (const QBtDevice& targetDevice, const QBtService & service);

    /**
     * Signal to be emitted when error occurs.
     * @param error error code.
     */
    void error(QBtServiceDiscoverer::ServiceDiscoveryError error);

private:
    QBtDevice 					_remoteDevice;    
    QBtServiceDiscovererPrivate *_implPtr;  
    
    friend class QBtServiceDiscovererPrivate;
};

#endif /* QBTSERVICEDISCOVERER_H_ */
