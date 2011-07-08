#ifndef QTMPUSHDEVICE_H
#define QTMPUSHDEVICE_H

#include <QDebug>

#include <QByteArray>
#include <QDateTime>
#include <QTimer>

#include "pushburtongenericdevice.h"

#include "npushimutick.h"
#include <QStringList>

#include <QBluetoothSocket>
#include <QBluetoothServiceInfo>

using namespace QtMobility;

class QtmPushDevice : public PushBurtonGenericDevice
{
    Q_OBJECT
public:
    explicit QtmPushDevice(QBluetoothServiceInfo info, QObject *parent = 0);
    ~QtmPushDevice();

    virtual QString getName();

    virtual bool is_online();

    virtual void disconnect_from_backend();

    virtual bool subscribesToAny();
    virtual bool subscribesTo(PushBurtonGenericDevice* deviceType);

public slots:
    void socketConnected();
    void socketDisconnected();
    void socketError(QBluetoothSocket::SocketError);

    void readSocket();

protected:
    void timerEvent(QTimerEvent *);

private:
    QBluetoothSocket * socket;

};

#endif // QTMPUSHDEVICE_H
