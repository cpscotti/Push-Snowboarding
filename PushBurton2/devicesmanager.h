#ifndef DEVICESMANAGER_H
#define DEVICESMANAGER_H

#include <QObject>
#include <QDebug>

#include <QBluetooth/QBtDevice.h>
#include <QBluetooth/QBtService.h>
#include <QBluetooth/QBtConstants.h>
#include <QBluetooth/QBluetooth.h>
#include <QBluetooth/QBtDeviceDiscoverer.h>
#include <QBluetooth/QBtServiceDiscoverer.h>
#include <QBluetooth/QBtSerialPortClient.h>
#include <QBluetooth/QBtLocalDevice.h>

#include "pushdevicesholder.h"
#include "pushn8btdevice.h"
#include "pushn8footdevice.h"
#include "pushn8gsrdevice.h"
#include "pushn8imudevice.h"
#include "pushn8heartdevice.h"

#include "pushn8phoneacc.h"
#include "pushn8phonegps.h"
#include "pushn8phonemag.h"

#include "pushn8absnormfeetdevice.h"
#include "pushn8airtimedetector.h"
#include "pushn8simplereportsgenerator.h"

//#include "pushn8unprogdev.h"
#include "pushn8simulationdevice.h"

class DevicesManager : public QObject
{
    Q_OBJECT
public:
    DevicesManager(PushDevicesHolder * aDevHolder, QObject *parent = 0);
    ~DevicesManager();

signals:
    void deviceConnected(QString description);

    //This may seem abusive at first look
    //  but makes complete sense since they are just routed from
    //  signals comming from different objects and well.. at some point you need to break into!
    void phone_gps_connecting();
    void phone_gps_connected();

    void motion_box_connecting();
    void motion_box_connected();

    void heart_connecting();
    void heart_connected();

    void gsr_connecting();
    void gsr_connected();

    void foot_l_connecting();
    void foot_l_connected();

    void foot_r_connecting();
    void foot_r_connected();


public slots:

    //inner mechanisms
    void deviceFound(QBtDevice);

    //comm with user app
    void start_bt_search();
    void stop_bt_search();
    void kit_selected(int);
    void switch_to_simulation_device();


private:
    PushDevicesHolder * configuredDevices;

    bool searching;
    bool auto_connect;
    int auto_connect_to_kit;

    bool simulating;


    QVector<QBtDevice> unknownFoundDevices;
    QBtDeviceDiscoverer * deviceDiscoverer;

    bool SetupDevice(QBtDevice);
    bool IsDeviceNovell(QBtDevice);

    void SetupPhoneDevices();
    void SetupAbstractDevices();

};

#endif // DEVICESMANAGER_H
