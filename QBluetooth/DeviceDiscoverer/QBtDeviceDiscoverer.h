/*
 * QBtDeviceDiscoverer.h
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

#ifndef QBTDEVICEDISCOVERER_H_
#define QBTDEVICEDISCOVERER_H_

#include <QBtGlobal.h>
#include <QObject>
#include <QBtTypes.h>

//Forward declaration
class QBtDeviceDiscovererPrivate;

/**
 * Class that contains the mechanism of the device discovery.
 * After instantiation, user can call startDiscovery() to begin the discovery.
 * If calling startDiscovery() is successfull then discoveryStarted() signal is emitted.
 * Any found device is reported through newDeviceFound(QBtDevice) signal or can be retrieved
 * calling GetInquiredDevices() which returns a list of all the found devices till now.
 * When discovery stops, discoveryStopped() signal is emitted.
 *
 * Incase of any errors, the are reported through
 * error(QBtDeviceDiscoverer::DeviceDiscoveryErrors) signal.
 */
class DLL_EXPORT QBtDeviceDiscoverer : public QObject
{
Q_OBJECT

public: //enums
    enum DeviceDiscoveryErrors
        { BluetoothNotSupported, BluetoothInUse, BluetoothAlreadyStopped, BluetoothNotReady,
        DiscoveryCancelled, UnknownError };

public:
    QBtDeviceDiscoverer(QObject* parent = 0);
    virtual ~QBtDeviceDiscoverer();

    /**
     * GetInquiredDevices()
     *
     * returns a reference to the list of devices found (if any)
     */
    const QBtDevice::List& getInquiredDevices() const;
    
    
    /**
     * Returns true if the class if performing a device discovery.
     */
    bool isBusy () const;

public slots:
    /**
     * startDiscovery()
     *
     * Start a device discovery.
     * The start and completion of device discovery are reported through
     * discoveryStarted() and discoveryStopped() signals respectively.
     */
    void startDiscovery();

    /**
     * stopDiscovery()
     *
     * Stops device discovery.
     * Signal discoveryStopped() is emitted.
     */
    void stopDiscovery();

signals:
    /**
     * Signaled for every device detected
     * @param remoteDevice The remote device
     */
    void newDeviceFound (const QBtDevice & remoteDevice);

    /**
     * Signal to report that discovery has stopped (either cancelled or no new devices to detect)
     */
    void discoveryStopped();

    /**
     * Signal to report that discovery has started. Alternative is error.
     */
    void discoveryStarted();

    /**
    * error
    * Signal to be emitted when error occurs.
    * @param error error code.
    */
    void error(QBtDeviceDiscoverer::DeviceDiscoveryErrors error);

private:        
    
    QBtDeviceDiscovererPrivate *_implPtr;    
    friend class QBtDeviceDiscovererPrivate;
    
};

#endif /* QBTDEVICEDISCOVERER_H_ */
