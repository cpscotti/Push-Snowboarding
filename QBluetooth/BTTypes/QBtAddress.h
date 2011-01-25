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

#ifndef QBTADDRESS_H
#define QBTADDRESS_H

#include <QString>
#include <QByteArray>
#include <QBtGlobal.h>

#ifdef Q_OS_SYMBIAN
#include <bttypes.h>
#endif

/**
 * Class that represents the device's bluetooth _address.
 * It is one of the main fields of QBtDevice.
 */
class DLL_EXPORT QBtAddress
{
public:

    /**
     * QBtAddress()
     * Default constructor.
     */
    QBtAddress();

    /**
     * QBtAddress()
     * Copy constructor
     */
    // really needed?
    //QBtAddress (const QBtAddress& other);

    QBtAddress (const QByteArray& byteArray);
    QBtAddress (const QString& addr);
    
#ifdef Q_OS_SYMBIAN
    QBtAddress (const TBTDevAddr& addr);
#endif
    /**
     * Mainly created because on windows bluesoleil sdk, bluetooth _address
     * byte array is reversed so it is usefull to provide this functionality
     * here
     *
     * @param reversedArray
     */
    static QBtAddress getAddressFromReversedArray (const QByteArray & reversedArray);

    ~QBtAddress ();

    /**
     * @return true if the _address contained in this instance is
     * a valid bluetooth _address
     */
    bool isValid () const;

    /**
     * Returns a string representation of this bluetooth _address
     */
    QString toString() const;

    /**     
     * Returns a 6-byte array representation of the bluetooth _address
     */
    QByteArray toByteArray() const; // Symbian representation

    /**
     * Returns a 6-byte array representation of the bluetooth _address
     * in reversed order. Mainly created for convenience when using
     * windows bluesoleil SDK.
     */
    QByteArray toReversedByteArray() const; // windows representation

    
    // really needed?
    //QBtAddress& operator= (const QBtAddress & other );
    
    bool operator!= (const QBtAddress & other );
    bool operator== (const QBtAddress & other );
    bool operator< (const QBtAddress & other );

#ifdef Q_OS_SYMBIAN
    TBTDevAddr convertToSymbianBtDevAddr();
#endif

private:
    QString _address;
    bool _valid;
};

#endif // QBTADDRESS_H
