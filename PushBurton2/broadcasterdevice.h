#ifndef BROADCASTERDEVICE_H
#define BROADCASTERDEVICE_H

#include "npushlogtick.h"
#include "pushburtongenericdevice.h"

#include <QtNetwork>
#include <QHostAddress>
#include <QXmlStreamWriter>
#include <QIODevice>

#include <QDebug>

class BroadcasterDevice : public PushBurtonGenericDevice
{
    Q_OBJECT
public:
    explicit BroadcasterDevice(QObject *parent = 0);

    ~BroadcasterDevice();


    QString getName();
    bool is_online() {
        return true;
    }

    //Our device subscribes to other devices so we need to reimplement these 2 functions
    //accordingly. One can argue that subscribesToAny wouldn't be necessary but it SURE
    //prevents doing a huge amount of unnecessary checks.
    bool subscribesToAny() { return true; }

    //This function will return true when the given device is a AirTimeDetector
    bool subscribesTo(PushBurtonGenericDevice*) { return true; }

    bool start_run();
    bool end_run();

signals:
    void connectionUpdate(bool connState);

public slots:

    void tryToConnect();
    void tryToDisconnect();

    void setServerAddress(const QString& addr);
    void setServerPort(quint16 port);

    void connected();
    void disconnected();
    void incoming_reading(NPushLogTick *);

private:
    bool running;

    QHostAddress serverAddress;
    quint16 serverPort;


    QTcpSocket *currConnection;

    QXmlStreamWriter *xmlWriter;


};

#endif // BROADCASTERDEVICE_H
