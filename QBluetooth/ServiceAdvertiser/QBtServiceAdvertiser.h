/*
 * QBtServiceAdvertiser.h
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

#ifndef QBTSERVICEADVERTISER_H_
#define QBTSERVICEADVERTISER_H_

#include <QBtGlobal.h>
#include <QObject>
#include <QBtTypes.h>

//forward declaration
class QBtServiceAdvertiserPrivate;

/**
 * Class responsible to advertise a given bluetooth service.
 *
 * After the instantiation of the class, startAdvertising(const QBtService&) can be callled
 * to start advertising the given bluetooth service. If successfull then
 * advertisingStarted(const QBtService&) signal is emitted.
 *
 * At any time stopAdvertising() can be called to stop the advertising and remove the
 * registered service from the SDP database. If successfull then advertisingStopped()
 * is emitted.
 *
 * If user wants to temporarily deactive the service advertisement then he can call
 * updateAvailability(bool) (instead of the stopAdvertising() that stops the advertising
 * permanently).
 *
 * In case of error, error(ErrorCodes) signal is emitted.
 *
 * For integrity reasons, at destruction stops the advertising
 * causing the service to be deleted from the SDP database.
 *
 * NOTE: Currently there is no implementation on the Windows platform so
 * after the intatiation of this object, any call to any function will
 * emit an error(QBtServiceAdvertiser::FeatureNotSupported) signal.
 */
class DLL_EXPORT QBtServiceAdvertiser : public QObject
{
    Q_OBJECT

public:
    enum ErrorCodes
    {
        FeatureNotSupported
    };

public:
    QBtServiceAdvertiser(QObject* parent);
    ~QBtServiceAdvertiser();

    /**
     * startAdvertising(const QBtService& service)
     *
     * Starts the service advertiser.
     *
     * @param service the service class that contains all the necessery
     * information for transmitting the service
     */
    void startAdvertising(const QBtService& service);

    /**
     * stopAdvertising()
     *
     * Stops the transmittion of the service running (if any)
     */
    void stopAdvertising();

    /**
     * updateAvailability()
     *
     * Change the availability of the service.
     * @param aAvailable If true, the service is transmitted.
     */
    void updateAvailability(bool aAvailable);

signals:
    /**
     * advertisingStarted(const QBtService& service)
     *
     * The signal is emitted if the transmittion of the service is started
     * @param service A reference to an object containing all the info of the service being transmitted.
     */
    void advertisingStarted(const QBtService& service);

    /**
     * advertisingStopped()
     * The signal is emitted when the transmittion of the service ends.
     */
    void advertisingStopped();

    /**
     * error(ErrorCodes code);
     *
     * Emitted in case of error.
     */
    void error(ErrorCodes code);

private:
    // The service to advertise
    QBtService* _localService;

    //pointer to implementation
    QBtServiceAdvertiserPrivate *_implPtr;  
    
    
    friend class QBtServiceAdvertiserPrivate;
    
};


#endif /* QBTSERVICEADVERTISER_H_ */
