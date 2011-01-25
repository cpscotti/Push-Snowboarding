/*
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

#ifndef QBTDEVICE_H
#define QBTDEVICE_H

#include <QList>
#include <QString>
#include <QBtGlobal.h>
#include <QBtService.h>
#include <QBtAddress.h>

/**
 * The class that contains all the information needed about any bluetooth device.
 * Its main feilds are 4.
 *
 * a) QBtAddress address: It is the device's bluetooth address.
 *                  Any communication between remote devices is made using this feild
 *                  so it is an essential information. No communication can be done
 *                  without this.
 *
 * b) QString name: The name of the device. By default this feild is equal to QBtAddress.toString().
 *                  The actual name is retrieved from the device discovery
 *                  and is set by the remote device as an identification string ID.
 *
 * c) DeviceMajor type: an enumeration feild characterizing the device type.
 *
 * d) QBtService::List supportedServices: this field is updated after calling
 *                  QBtServiceDiscoverer::startDiscovery(QBtDevice*)
 *                  (if any services are found through SDP)
 */
class DLL_EXPORT QBtDevice
{
public:
    typedef QList<QBtDevice> List;
    
    enum DeviceMajor
    {
        Uncategorized, 	//Uncategorized device
        Computer,		//Computer device
        Phone,			//Phone device
        LANAccess,		//Some form of a local area network router
        AudioVideo,		//Audio / Video device
        Peripheral,		//Mouse, Joystick, Keyboard
        Imaging,		//Camera or Scanner device
        Wearable,		//Wearable Device
        Toy,			//Toy device
        Miscellaneous	//Miscellaneous device
    };

public:
    QBtDevice();
    QBtDevice(const QBtDevice& dev);
    QBtDevice (const QString & devName, const QBtAddress & devAddress, DeviceMajor devType);
    ~QBtDevice();

    void addNewService (const QBtService& newService);
    void setName (const QString & newName);
    void setAddress (const QBtAddress& newAddress);
    void setType(DeviceMajor newType);
    void setSupportedServices (const QBtService::List& newSupportedServices);
    
    bool serviceListUpdated() const;

    QString getName() const;
    QBtAddress getAddress() const;
    DeviceMajor getType() const;
    QString getTypeAsString() const;
    QBtService::List getSupportedServices() const;

    
    // really needed? does not require special processing to copy the members
    //QBtDevice& operator= (const QBtDevice& dev);

private:
    QString _name;
    QBtAddress _address;
    DeviceMajor _type;
    QBtService::List _supportedServices;
    
    bool _serviceListUpdated;
};

#endif // QBTDEVICE_H

