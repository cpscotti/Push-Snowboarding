/*
 * QBtServiceDiscoverer_stub.h
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

#ifndef QBTSERVICEDISCOVERER_STUB_H_
#define QBTSERVICEDISCOVERER_STUB_H_

#include <QBtDevice.h>
#include <QBtServiceDiscoverer.h>

class QBtServiceDiscovererPrivate
{
public:
    QBtServiceDiscovererPrivate(QBtServiceDiscoverer* publicClass);
    ~QBtServiceDiscovererPrivate();

    void DiscoverServicesOnDevice(QBtDevice* targetDevice);
    void DiscoverSpecificProtocol(QBtDevice* targetDevice, QBtConstants::ServiceClass uuid);
    void StopDiscovery();
    QBtService::List GetSupportedServices();
    
    bool IsBusy() const;
    
    

private:
    QBtServiceDiscoverer* p_ptr;  //Pointer to public interface
};

#endif /* QBTSERVICEDISCOVERER_STUB_H_ */
