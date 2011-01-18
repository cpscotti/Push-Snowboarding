#ifndef PUSHN8BTDEVICE_H
#define PUSHN8BTDEVICE_H

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

#include <QRegExp>

#include "pushburtongenericdevice.h"
#include "npushlogtick.h"

class PushN8BtDevice : public PushBurtonGenericDevice
{
    Q_OBJECT
public:
    explicit PushN8BtDevice(QBtDevice a_device, QObject *parent = 0);
    ~PushN8BtDevice();

    QString getDeviceName();

    QString getDetails();

    QString get_description();

    bool is_online();

    QString dataBuffer;

    virtual void disconnect_from_backend();

protected:
    QBtDevice device;
    QBtSerialPortClient * SPPClient;

    QString packetStart;
    QString packetEnd;
    int packetLen;

    QStringList getPackage(QString data);

    bool getPackage(QString data, QStringList& result);

private:
    QBtServiceDiscoverer * serviceDisc;
    QBtService service;


protected:
    virtual void timerEvent(QTimerEvent *);

signals:

public slots:

    void serviceFound(QBtDevice,QBtService);
    void gotDataReceived(QString);
    void gotError(QBtSerialPortClient::ErrorCode);

    void gotDisconnected();
    void connectionResetByPeer();

};

#endif // PUSHN8BTDEVICE_H
