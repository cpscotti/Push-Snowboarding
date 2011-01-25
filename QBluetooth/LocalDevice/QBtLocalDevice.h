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

#ifndef QBTLOCALDEVICE_H
#define QBTLOCALDEVICE_H

#include <QBtGlobal.h>
#include <QBtTypes.h>
#include <QObject>


class DLL_EXPORT QBtLocalDevice : public QObject
{
    Q_OBJECT
public:

    ///////////////////////////////////////
    ///	 Bluetooth local properties		///
    ///////////////////////////////////////
    /**
     * Returns if possible the local device address
     */
    static QBtAddress getLocalDeviceAddress();

    /**
     * Returns true if local device is visible to other devices.
     */
    static bool isVisible ();

    /**
     * Sets the device visibility to other devices.
     * @param value true -> visible, false -> invisible
     */
    static void setVisible (bool value);
    
    
    static QBtDevice::DeviceMajor getDeviceClass();

    /**
     * Sets the UUID of the device that characterizes the device's type.
     * The possible enumeration values passed as argument are defined in
     * QBtDevice::DeviceMajor.
     */
    static void setDeviceClass(QBtDevice::DeviceMajor classId);

    /**
     * setLocalDeviceName()
     *
     * Sets the name of the device throught which it will be identified by other
     * devices during SDP discovery.
     */
    static void setLocalDeviceName (const QString & deviceName);

    /**
     * GetDeviceName()
     *
     * Get local device name.
     * @return device name
     */
    static QString getLocalDeviceName();

    /**
     * Returns true if bluetooth hardware exists and is plugged in.
     */
    static bool isBluetoothSupported();

    /**
     * Symbian specific function.
     * Used if bluetooth is powered off, showing a dialog that asks permission
     * from the user to power on bluetooth.
     */
    static void askUserTurnOnBtPower();
    
    
    /**
     * Symbian specific function.
     * Turns on/off bluetooth. Returns true is operation was sucessful.     
     */
    static bool setBluetoothPowerState (bool value);    

    /**
     * Returns true/false if bluetooth hardware is powered on/off respectively.
     */
    static bool getBluetoothPowerState();

    /**
     * addNewDevice()
     *
     * Add a new device in the bluetooth registry.
     * @param device the device to be added
     * @return true upon success, else false
     */
    static bool addNewDevice(const QBtDevice& device);

    /**
     * deleteDevice()
     *
     * Delete a device from the bluetooth registry.
     * @param device the device to be deleted
     * @return true upon success, else false. If device doesn't exists in the registry the also false
     */
    static bool deleteDevice(const QBtDevice& device);

    /**
     * deleteDevice()
     *
     * Delete a device from the bluetooth registry using the bluetooth address.
     * @param address the device address of the device to be delete
     * @return true upon success, else false. If device doesn't exists in the registry the also false
     */
    static bool deleteDevice(const QBtAddress& address);

    /**
     * Breaks the pairing between the local device and the device passed as argument.
     *
     * @param device the device class containing the information of the device to be unpaired.
     */
    static bool unpairDevice(const QBtDevice& device);

    /**
     * Breaks the pairing between the local device and the remote device identified by
     * the bluetooth address passed as argument.
     *
     * @param address the bluetooth address of the remote device.
     */
    static bool unpairDevice(const QBtAddress& address);
};

#endif // QBTLOCALDEVICE_H
