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

#ifndef QBTSERVICE_H
#define QBTSERVICE_H

#include <QString>
#include <QList>
#include <QBtGlobal.h>
#include <QBtConstants.h>

/**
 * Class containing all the necessary information about a bluetooth service
 * of a remote device.
 */
class DLL_EXPORT QBtService
{
public:
    typedef QList<QBtService> List;

    /*
     * This list will be populates either by the Service Discovery or
     * by the user.
     *
     * The index of every protocol contained corresponds to the level of
     * hierarchy that it belongs.
     *
     * example: to create a service using RFCOMM then the list will be:
     * [0]: L2CAP
     * [1]: RFCOMM
     *
     * So ProtocolList::last() returns the highest protocol.
     */
    typedef QList<QBtConstants::ServiceProtocol> ProtocolList;

    QBtService();
    
    // really needed?
    //QBtService(const QBtService& service);

    ProtocolList getProtocols() const;
    QBtConstants::ServiceClass getClass() const;
    QString getName() const;
    QString getDescription() const;
    unsigned int getPort() const;
    unsigned int getHandle() const;

    void addProtocol(QBtConstants::ServiceProtocol uuid);
    bool removeProtocol(QBtConstants::ServiceProtocol uuid);

    void setProtocols(const ProtocolList & newUUIDs);
    void setClass(QBtConstants::ServiceClass newClass);
    void setName(const QString& newName);
    void setDescription(const QString& newDescription);
    void setPort(unsigned int newPort);
    void setHandle(unsigned int newHandle);

    // really needed?
    //QBtService& operator= (const QBtService& service);

private:
    // protocol list is mainly used on symbian
    // on windows tha connections are done using classUUID
    QBtConstants::ServiceClass classUUID;
    ProtocolList protocolUUID;
    QString name;
    QString description;
    unsigned int handle;

    // used on RFCOMM service as channel number to be used for connection
    // to remote device
    unsigned int port;
};

#endif // QBTSERVICE_H
