/*
 * QBtServiceAdvertiser.cpp
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

#include "../QBtServiceAdvertiser.h"
#include <QBtAuxFunctions.h>

#if defined(Q_OS_SYMBIAN)
#include "../QBtServiceAdvertiser_symbian.h"
#elif defined(Q_OS_WIN32)
#include "../QBtServiceAdvertiser_win32.h"
#elif !defined(Q_OS_WIN32) && !defined(Q_OS_SYMBIAN)
#include "../QBtServiceAdvertiser_stub.h"
#endif

//////////////////////////////////////////////////
// QBtServiceAdvertiser::QBtServiceAdvertiser()	//
// Defaul constructor							//
//////////////////////////////////////////////////
QBtServiceAdvertiser::QBtServiceAdvertiser(QObject *parent) :
    QObject(parent), _localService(NULL)
{
#ifdef Q_OS_SYMBIAN
    QT_TRAP_THROWING(_implPtr = QBtServiceAdvertiserPrivate::NewL(this));
#else
    _implPtr = new QBtServiceAdvertiserPrivate(this);
#endif
}

QBtServiceAdvertiser::~QBtServiceAdvertiser()
{
    stopAdvertising();

    SafeDelete(_localService);
    SafeDelete(_implPtr);
}

/**
 * startAdvertising(const QBtService& service)
 */
void QBtServiceAdvertiser::startAdvertising(const QBtService& service)
{
    _localService = new QBtService(service);
    _implPtr->StartAdvertiser(service);
}

/**
 * stopAdvertising()
 */
void QBtServiceAdvertiser::stopAdvertising()
{
    _implPtr->StopAdvertiser();
}

/**
 *
 */
void QBtServiceAdvertiser::updateAvailability(bool aAvailable)
{
    _implPtr->UpdateAvailability(aAvailable);
}
