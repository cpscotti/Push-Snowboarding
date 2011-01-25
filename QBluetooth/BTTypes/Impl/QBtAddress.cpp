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

#include "../QBtAddress.h"

#define BT_INVALID_ADDRESS "00:00:00:00:00:00"

#include <QRegExp>

QBtAddress::QBtAddress()
: _address(BT_INVALID_ADDRESS),
  _valid(false)
{
}

/*
QBtAddress::QBtAddress (const QBtAddress& other)
{
	_valid = other.isValid();
	_address = other.toString();
}
*/

QBtAddress::QBtAddress (const QString& addr)
 : _address(BT_INVALID_ADDRESS),
   _valid(false) 
{
	QRegExp rx("^[0-9a-fA-F]{2,2}:[0-9a-fA-F]{2,2}:[0-9a-fA-F]{2,2}:[0-9a-fA-F]{2,2}:[0-9a-fA-F]{2,2}:[0-9a-fA-F]{2,2}$");
	_valid = addr.contains( rx );
	if (_valid) 
		_address = addr.toLower();
}

QBtAddress::QBtAddress (const QByteArray& byteArray)
: _address(BT_INVALID_ADDRESS),
  _valid(false)
{
	if (byteArray.size() != 6)
		return;

	QString btAddr = "";
	char tmpChar[32];
	
	for (int i = 0; i < 5; i++){
		sprintf(tmpChar,"%02X:", ((unsigned char)byteArray[i]));
		btAddr += tmpChar;
	}
	sprintf(tmpChar,"%02X", ((unsigned char)byteArray[5]));
	btAddr += tmpChar;

	QRegExp rx("^[0-9a-fA-F]{2,2}:[0-9a-fA-F]{2,2}:[0-9a-fA-F]{2,2}:[0-9a-fA-F]{2,2}:[0-9a-fA-F]{2,2}:[0-9a-fA-F]{2,2}$");
	_valid = btAddr.contains( rx );
	if (_valid) 
		_address = btAddr.toLower();
}

#ifdef Q_OS_SYMBIAN
QBtAddress::QBtAddress (const TBTDevAddr& addr)
{
	QBtAddress tmpAddr (QByteArray((const char*)addr.Des().Ptr(), 6));
	*this = tmpAddr;	
}
#endif

QBtAddress QBtAddress::getAddressFromReversedArray (const QByteArray & reversedArray)
{
	unsigned char tmpByte;
	int size = reversedArray.size();
	
	QByteArray array (reversedArray);
	
	for(int i=0; i<size/2; i++)
	{
		tmpByte = array [i];
		array [i] = array [size-i-1];
		array [size-i-1] = tmpByte;
	}
	
	return QBtAddress (array);
}

QBtAddress::~QBtAddress ()
{
}

bool QBtAddress::isValid () const
{
	return _valid;
}

QString QBtAddress::toString() const
{
	return _address;
}

// Converts the _address string into a byte array of size 6.
// Every 2 characters from the string represent an 8bit number,
// the characters (excluding ':') are 12, so 6 8bit numbers are 
// created and returned through QByteArray
//
QByteArray QBtAddress::toByteArray() const
{
	if(!_valid)
		return QByteArray();
	
	QString btAddressCopy = _address;
	btAddressCopy.remove(QChar(':'));
	
	QByteArray resultArray;	
	resultArray.append(btAddressCopy.toAscii());
	resultArray = QByteArray::fromHex(resultArray);	
	
	return resultArray;
}

// Converts the _address string into a byte array as toByteArray
// function but reversed. This function is supported for
// convenience on windows support (and specialy Bluesoleil SDK
// where _address byte array is expected in this reversed way)
//
QByteArray QBtAddress::toReversedByteArray() const
{
	if(!_valid)
			return NULL;
		
	QByteArray revArray = this->toByteArray();
	
	int arraySize = revArray.size();
	char tmpChar;
	for(int i=0; i<arraySize/2; i++)
	{
		tmpChar = revArray[i];
		revArray[i] = revArray[arraySize-i-1];
		revArray[arraySize-i-1] = tmpChar;
	}
	
	return revArray;
}

bool QBtAddress::operator!= (const QBtAddress & other )
{
  int result = _address.compare(other.toString(),Qt::CaseInsensitive);
  
  return ((result == 0) ? false : true);
}

/*
QBtAddress& QBtAddress::operator= ( QBtAddress & other )
{
	if(this != &other)
	{
		_valid = other.isValid();
		_address = other.toString();
	}
	return *this;
}
*/

bool QBtAddress::operator== (const QBtAddress & other )
{
	return !(*this!=other);
}

bool QBtAddress::operator< (const QBtAddress & other )
{
	return (this->toString() < other.toString());
}

#ifdef Q_OS_SYMBIAN
TBTDevAddr QBtAddress::convertToSymbianBtDevAddr()
{
	const TUint8* memPtr = (const TUint8*)toByteArray().constData();
		
	TPtrC8 devAddrPtr8;
	devAddrPtr8.Set(memPtr, 6);

	const TBTDevAddr devAddress = TBTDevAddr(devAddrPtr8);
	
	return devAddress;
}
#endif
