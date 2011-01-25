/*
 * QBtLocalDevice_symbian.cpp
 *
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

#include "../QBtLocalDevice_symbian.h"
#include <QBtAuxFunctions.h>
#include <QtGlobal>


// bluetooth engine api plugin
#ifdef __SERIES60_31__
	#include <bteng.h>  
#else
	#include <btengsettings.h>
#endif



///////////////////////////////////////
///	 Bluetooth local properties		///
///////////////////////////////////////

TBool QBtLocalDevicePrivate::DefineProperty(TUint propertyId, RProperty::TType typeEnum)
{
    TInt attErr = RProperty::Define(KPropertyUidBluetoothControlCategory,propertyId,typeEnum);
    if (attErr != KErrNone && attErr != KErrAlreadyExists)
        return EFalse;

    return ETrue;
}

QBtAddress QBtLocalDevicePrivate::GetLocalDeviceAddress()
{
    TUint8 buffer[512];
    TPtr8 value(buffer, 512);
    RProperty::Get( KPropertyUidBluetoothControlCategory,
            KPropertyKeyBluetoothGetLocalDeviceAddress, value );

    QByteArray bArray((const char*)buffer, 6);
    return QBtAddress(bArray);
}

TBool QBtLocalDevicePrivate::IsVisible()
{
    /*
    TInt value;
    RProperty::Get( KPropertyUidBluetoothControlCategory,
            KPropertyKeyBluetoothGetLimitedDiscoverableStatus, value );

    return value;
    */

  TBool value = EFalse;


  QT_TRAP_THROWING(
      {
          #ifdef __SERIES60_31__
            TBTDiscoverabilityMode mode;
            CBTMCMSettings* b = CBTMCMSettings::NewLC ();
            User::LeaveIfError (b->GetDiscoverabilityMode (mode) );

            if (EBTDiscModeHidden == mode)
              value = EFalse;
            else
              value = ETrue;

          #else
              TBTVisibilityMode mode;
              CBTEngSettings* b = CBTEngSettings::NewLC();
              User::LeaveIfError(b->GetVisibilityMode (mode) );

              if (EBTVisibilityModeHidden  == mode)
                value = EFalse;
              else
                value = ETrue;

          #endif

          CleanupStack::PopAndDestroy();
      }
  );


  return value;

}


void QBtLocalDevicePrivate::SetVisible (TBool value)
{
    /*
    DefineProperty(KPropertyKeyBluetoothGetLimitedDiscoverableStatus, RProperty::EInt);

    RProperty::Set( KPropertyUidBluetoothControlCategory,
            KPropertyKeyBluetoothSetLimitedDiscoverableStatus, limited);
            */

  QT_TRAP_THROWING(
      {
          #ifdef __SERIES60_31__
            TBTDiscoverabilityMode mode;

            if (value)
              mode = EBTDiscModeGeneral;
            else
              mode = EBTDiscModeHidden;


            CBTMCMSettings* b = CBTMCMSettings::NewLC ();
            User::LeaveIfError (b->SetDiscoverabilityModeL (mode) );

          #else
              TBTVisibilityMode mode;

              if (value)
                mode = EBTVisibilityModeGeneral;
              else
                mode = EBTVisibilityModeHidden;

              CBTEngSettings* b = CBTEngSettings::NewLC();
              User::LeaveIfError(b->SetVisibilityMode (mode) );

          #endif

          CleanupStack::PopAndDestroy();
      }
  );


}

QBtDevice::DeviceMajor QBtLocalDevicePrivate::GetDeviceClass()
{
    TInt value;
    RProperty::Get( KPropertyUidBluetoothControlCategory,
            KPropertyKeyBluetoothGetDeviceClass, value );

    return (QBtDevice::DeviceMajor)value;
}

void QBtLocalDevicePrivate::SetDeviceClass(QBtDevice::DeviceMajor classId)
{
    DefineProperty(KPropertyKeyBluetoothGetDeviceClass, RProperty::EInt);

    RProperty::Set( KPropertyUidBluetoothControlCategory,
            KPropertyKeyBluetoothGetDeviceClass, (TInt)classId );
}

QString QBtLocalDevicePrivate::GetLocalDeviceName()
{
    TUint16 buffer[512] = {0};
    TPtr16 value(buffer, 512);
    RProperty::Get( KPropertyUidBluetoothControlCategory,
            KPropertyKeyBluetoothGetDeviceName, value );

    return QString::fromUtf16((unsigned short*)value.Ptr());
}

void QBtLocalDevicePrivate::SetLocalDeviceName (const QString & deviceName)
{
    DefineProperty(KPropertyKeyBluetoothSetDeviceName, RProperty::EText);

    TPtr16 value ((TUint16*)deviceName.utf16(), deviceName.size());
    RProperty::Set (KPropertyUidBluetoothControlCategory, KPropertyKeyBluetoothSetDeviceName, value );
}



///////////////////////////////
///	 Bluetooth Hardware		///
///////////////////////////////

TBool QBtLocalDevicePrivate::IsBluetoothSupported()
{
    return CFeatureDiscovery::IsFeatureSupportedL(KFeatureIdBt);
}

void QBtLocalDevicePrivate::AskUserTurnOnBtPower()
{
    const TUid KPowerModeSettingNotifierUid = {0x100059E2};

    RNotifier notifier;
    User::LeaveIfError( notifier.Connect() );
    TPckgBuf<TBool> dummy(ETrue);
    TPckgBuf<TBool> reply(EFalse);
    TRequestStatus stat;
    notifier.StartNotifierAndGetResponse(stat, KPowerModeSettingNotifierUid, dummy, reply);

    User::WaitForRequest(stat);
    notifier.CancelNotifier(KPowerModeSettingNotifierUid);
    notifier.Close();
}

TBool QBtLocalDevicePrivate::GetBluetoothPowerState()
{
    TInt value=0;


    QT_TRAP_THROWING(
         {
              CRepository* crep = CRepository::NewLC(KCRUidBluetoothPowerState);
              User::LeaveIfError( crep->Get(KBTPowerState, value) );
              CleanupStack::PopAndDestroy();
         }
    );



    return value;
}


TBool QBtLocalDevicePrivate::SetBluetoothPowerState (TBool value)
{		  
  QT_TRAP_THROWING(
      {
          #ifdef __SERIES60_31__
              CBTMCMSettings* b = CBTMCMSettings::NewLC ();
              User::LeaveIfError(b->SetPowerState (value) );

          #else
              TBTPowerStateValue v = value? EBTPowerOn: EBTPowerOff;
              CBTEngSettings* b = CBTEngSettings::NewLC();
              User::LeaveIfError(b->SetPowerState (v) );
          #endif

          CleanupStack::PopAndDestroy();
    }
    );

	
  return true;

}


///////////////////////////////
///	 Bluetooth Registry		///
///////////////////////////////

TBool QBtLocalDevicePrivate::AddNewDevice(const QBtDevice& device)
{
    try
    {
        CBTDevice* newDev;
        RBTRegServ regServ;
        RBTRegistry view;
        TRequestStatus stat;

        QT_TRAP_THROWING(
                newDev = CBTDevice::NewL(device.getAddress().convertToSymbianBtDevAddr()));

        newDev->SetDeviceClass(TBTDeviceClass(device.getType()));

        TPtrC8 name8((const TUint8 *)device.getName().toUtf8().data(),
                device.getName().toUtf8().size());
        TPtrC16 name16(device.getName().utf16());

        QT_TRAP_THROWING(
        {
            newDev->SetDeviceNameL(name8);
            newDev->SetFriendlyNameL(name16);
            //newDev->set
            regServ.Connect();
            view.Open(regServ);
            view.AddDeviceL(*newDev,stat);
        });

        User::WaitForRequest(stat);
        stat.Int();

        view.Close();
        regServ.Close();

        SafeDelete(newDev);
    }
    catch(char* str)
    {
        return false;
    }

    return true;
}

TBool QBtLocalDevicePrivate::DeleteDevice(const QBtAddress& address)
{
    return DeleteOrUnpairDevice(address, false);
}

TBool QBtLocalDevicePrivate::UnpairDevice(const QBtAddress& address)
{
    return DeleteOrUnpairDevice(address, true);
}

TBool QBtLocalDevicePrivate::DeleteOrUnpairDevice(const QBtAddress& address, TBool unpair)
{
    RBTRegServ regServ;
    RBTRegistry view;

    if( regServ.Connect() != KErrNone)
        return false;

    if( view.Open(regServ) != KErrNone)
    {
        regServ.Close();
        return false;
    }

    TBTRegistrySearch search;
    //the bd_addr of the device to be deleted
    QBtAddress nonConstAddress = address;
    search.FindAddress(nonConstAddress.convertToSymbianBtDevAddr());
    TRequestStatus stat;
    view.CreateView(search,stat);
    User::WaitForRequest(stat);

    if(stat.Int() != KErrNone)
    {
        regServ.Close();
        return false;
    }
    if(unpair)
        view.UnpairAllInView(stat);
    else
        view.DeleteAllInView(stat);
    User::WaitForRequest(stat);

    if(stat.Int() != KErrNone)
    {
        regServ.Close();
        return false;
    }

    return true;
}
