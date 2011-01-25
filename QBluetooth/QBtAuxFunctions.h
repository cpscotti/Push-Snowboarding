/*
 * auxFunctions_symbian.h
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

#ifndef AUXFUNCTIONS_SYMBIAN_H_
#define AUXFUNCTIONS_SYMBIAN_H_

#include <QtGlobal>

template <class T> inline void SafeDelete(T* &ptr)
{
	if(ptr)
	{
		delete ptr;
		ptr = 0; 
	}
}


#ifdef Q_OS_SYMBIAN

#include <QMessageBox>
#include <QDir>
#include <QDebug.h>

inline void _DEBUG_MSG (const QString & m, const QString & file, const QString& line)
{
	qDebug() << "file: " << file << " line: " << line << " " << m;
}

inline void _Assert_Msg (const QString msg, const QString & file, const QString & line)
{
	// strip directory info
	int index = file.lastIndexOf("/");
	QString f = file.mid (index+1);
	QString m = QString ("file: %1 line: %2\n%3").arg (f, line, msg);

	QMessageBox::information(0, "assert failed", m);		
	Q_ASSERT (false);
}

inline void _Break_Here (const QString & file, const QString & line)
{
	// strip directory info
	int index = file.lastIndexOf("/");
	QString f = file.mid (index+1);
	QString m = QString ("file: %1 line: %2").arg (f, line);

	QMessageBox::information(0, "code break", m);
	Q_ASSERT (false);
}


#ifdef QT_DEBUG
	#define DEBUG_MSG(msg) { _DEBUG_MSG((msg),__FILE__,QString::number(__LINE__)); }
	#define BREAK_HERE() { _Break_Here(__FILE__, QString::number(__LINE__)); }
	#define ASSERT_MSG(test, msg) { if (!(test)) _Assert_Msg ((msg), __FILE__, QString::number(__LINE__) ); }
	
	//#define ASSERT_MSG_X(test, where, msg) { if (!(test)) Assert_Msg ((msg), __FILE__, QString::number(__LINE__) ); }
#else

	#define DEBUG_MSG(msg) 0
	#define ASSERT_MSG_X(test, where, msg) 0
	#define ASSERT_MSG(test, msg) 0
	#define BREAK_HERE() 0

#endif
	
	
#endif



#ifdef Q_OS_WIN32

#include <windows.h>
#include <conio.h>

extern "C"{
#include "./BlueSoleil_SDK_2.0.5/include/Btsdk_ui.h"
}

using namespace std;


// used to have the number of the classes that use the Bluesoleil SDK.
// if the number reaches zero the sdk is deinitialized.
static size_t sdkInitializationCounter = 0;

static bool InitBthSdk()
{
	if ( sdkInitializationCounter == 0) /* not connected with BlueSoleil */
	{
		if (Btsdk_Init() != BTSDK_OK)
			return false;

		if (Btsdk_IsBluetoothReady() == BTSDK_FALSE)
			Btsdk_StartBluetooth();
			
		BTUINT16 discoveryFlags = BTSDK_CONNECTABLE | BTSDK_PAIRABLE | BTSDK_GENERAL_DISCOVERABLE;
		Btsdk_SetDiscoveryMode(discoveryFlags);
		Btsdk_SetLocalDeviceClass(BTSDK_COMPCLS_DESKTOP);
	}
	sdkInitializationCounter++;
	
	return true;
}

static void DeinitBthSdk()
{
	if(Btsdk_IsSDKInitialized() && sdkInitializationCounter > 0)
	{
		sdkInitializationCounter--;
		if(sdkInitializationCounter == 0)
			Btsdk_Done();
	}
}

static BTDEVHDL GetDeviceHandle(QBtAddress& address)
{
	//get device handle
	BTDEVHDL devHandle = BTSDK_INVALID_HANDLE;
	BTUINT8 btAddr [6]= {0};
	QByteArray btAddrQt = address.toReversedByteArray();
	memcpy(btAddr, btAddrQt.constData(), btAddrQt.size());
	devHandle = Btsdk_GetRemoteDeviceHandle(btAddr);
	return devHandle;
}
#endif // Q_OS_WIN32

#endif /* AUXFUNCTIONS_SYMBIAN_H_ */
