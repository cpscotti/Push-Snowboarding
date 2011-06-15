#include "broadcasterdevice.h"

BroadcasterDevice::BroadcasterDevice(QObject *parent) :
    PushBurtonGenericDevice(parent)
{

    running = false;


    xmlWriter = 0;

    currConnection = 0;



    serverAddress = QHostAddress("192.168.0.2");

    serverPort = 45454;

}


BroadcasterDevice::~BroadcasterDevice()
{
    if(currConnection)
        currConnection->disconnectFromHost();
}


QString BroadcasterDevice::getName()
{

    return QString("push.network.broadcaster");
}


bool BroadcasterDevice::start_run()
{
    if(!running) {
        running = true;
        return true;
    }
    return false;
}

bool BroadcasterDevice::end_run()
{
    if(running) {
        running = false;

        return true;
    }
    return false;
}

void BroadcasterDevice::incoming_reading(NPushLogTick * tick)
{
    if(running) {
        if(xmlWriter && currConnection->isWritable()) {
            tick->dump_to_xml(*xmlWriter);
            currConnection->flush();
        }
    }
}

void BroadcasterDevice::connected()
{
    qDebug() << "Connected";
    xmlWriter = new QXmlStreamWriter(currConnection);
    qDebug() << "coupled with xml writer";

    emit connectionUpdate(true);
}

void BroadcasterDevice::disconnected()
{
    if(xmlWriter) {
        delete xmlWriter;
        xmlWriter = 0;
    }
    if(currConnection) {
        currConnection->close();
        currConnection->deleteLater();
        currConnection = 0;
    }
    emit connectionUpdate(false);
}

void BroadcasterDevice::tryToConnect()
{
    if(currConnection) {
        qDebug() << "Cleaning up old connection";
        currConnection->close();
        currConnection->deleteLater();
        currConnection = 0;
    }

    qDebug() << "Trying to establish new connection";

    currConnection = new QTcpSocket(this);

    currConnection->connectToHost(serverAddress, serverPort, QIODevice::WriteOnly);

    connect(currConnection, SIGNAL(connected()),
            this, SLOT(connected()));

    connect(currConnection, SIGNAL(disconnected()),
            this, SLOT(disconnected()));
}

void BroadcasterDevice::setServerAddress(const QString &addr)
{
    serverAddress.setAddress(addr);
    qDebug() << "Address set to " << serverAddress.toString();
}

void BroadcasterDevice::setServerPort(quint16 port)
{
    serverPort = port;
    qDebug() << "Port set to " << serverPort;
}

void BroadcasterDevice::tryToDisconnect()
{
    if(xmlWriter) {
        delete xmlWriter;
        xmlWriter = 0;
    }

    if(currConnection) {
        currConnection->close();
        currConnection->deleteLater();
        currConnection = 0;
    }

    emit connectionUpdate(false);
}
