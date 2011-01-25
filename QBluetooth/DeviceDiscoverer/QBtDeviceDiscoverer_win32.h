/*
 * QBtDeviceDiscoverer_stub.h
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

#ifndef QBTDEVICEDISCOVERER_STUB_H_
#define QBTDEVICEDISCOVERER_STUB_H_

#include <QBtTypes.h>
#include <QBtDeviceDiscoverer.h>
#include <QBtAuxFunctions.h>

class QBtDeviceDiscovererPrivate
{
public:
    QBtDeviceDiscovererPrivate(QBtDeviceDiscoverer *publicClass = 0);
    ~QBtDeviceDiscovererPrivate();

public:
    void DiscoverDevices();
    void StopDiscovery();
    QBtDevice::List GetInquiredDevices();
	bool IsBusy () const;

protected:
    void Construct();

private:
    static void ReportInquiryResult(BTDEVHDL dev_hdl);
    static void InquiryCompleteResult(void);
    static void RegisterFoundDevice(BTDEVHDL dev_hdl);

private:
    static QBtDevice::List deviceList;
    // handle of the win event used to indicate that discovery finished
    static HANDLE browseDevEventHandler;
    static QBtDeviceDiscoverer* sp_ptr;
    static bool isSearching;

    QBtDeviceDiscoverer *p_ptr;  //Pointer to public interface
};

#endif /* QBTDEVICEDISCOVERER_STUB_H_ */
