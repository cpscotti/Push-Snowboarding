/*
 * QBtServiceAdvertiser_stub.h
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

#ifndef QBTSERVICEADVERTISER_STUB_H_
#define QBTSERVICEADVERTISER_STUB_H_

class QBtServiceAdvertiserPrivate
{
public:
	QBtServiceAdvertiserPrivate(QBtServiceAdvertiser* publicClass);
	~QBtServiceAdvertiserPrivate();

	/*!
	 * StartAdvertiser(const QBtService& service)
	 *
	 * Starts the service advertiser.
	 *
	 * param service the service class that contains all the necessery
	 * information for transmitting the service
	 */
	void StartAdvertiser(const QBtService& service);

	/*!
	 * StopAdvertiser()
	 *
	 * Stops the service advertiser.  our entry from service 
	 * discovery database will be removed.
	 */
	void StopAdvertiser();

	/*!
	 * UpdateAvailability()
	 *
	 * Updates the availability of advertised service.  the service
	 * record in service discovery database will be updated accordingly.
	 *
	 * param aAvailable true if the service should be set as available, 
	 * false if unavailable.
	 */
	void UpdateAvailability(TBool aAvailable);


private:
	QBtServiceAdvertiser* p_ptr;
};

#endif /* QBTSERVICEADVERTISER_STUB_H_ */
