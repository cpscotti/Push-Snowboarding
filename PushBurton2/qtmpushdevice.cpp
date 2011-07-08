#include "qtmpushdevice.h"

using namespace QtMobility;

QtmPushDevice::QtmPushDevice(QBluetoothServiceInfo info, QObject *parent) :
    PushBurtonGenericDevice(parent)
{
    timerId = 0;
    timerPeriod = 25;
    socket = 0;

    socket = new QtMobility::QBluetoothSocket(QtMobility::QBluetoothSocket::RfcommSocket, this);

    if(socket)
    {
        qDebug() << "Created socket";

        connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
        connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));


        connect(socket, SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(socketError(QBluetoothSocket::SocketError)));

        socket->connectToService(info);
        qDebug() << "ConnectToService done";
    }

    this->start_readings();

}

QtmPushDevice::~QtmPushDevice()
{

}


void QtmPushDevice::readSocket()
{
    if(socket)
    {
        while (socket->canReadLine()) {
            QByteArray line = socket->readLine();
            qDebug() << "Received: " << line;

            QString strLine(line);
            QStringList heap = strLine.split(QRegExp("(,|;|\\n|\\r)+"), QString::SkipEmptyParts);

            NPushIMUTick * newIMUTick = new NPushIMUTick(heap);

            if(receivers(SIGNAL(reading_ready(NPushLogTick*))) > 0)
            {
                //anyone listening to us
                emit reading_ready(newIMUTick);
            } else {
                delete newIMUTick;
            }
        }
    }
}

QString QtmPushDevice::getName()
{
    return QString("push.qtm.bt.unknown");
}

bool QtmPushDevice::is_online()
{
    if(socket)
        return(socket->isOpen());
    else
        return false;
}

void QtmPushDevice::disconnect_from_backend()
{
    if(socket)
    {
        socket->deleteLater();
        socket = 0;
    }
}

bool QtmPushDevice::subscribesToAny()
{
    return false;
}

bool QtmPushDevice::subscribesTo(PushBurtonGenericDevice *)
{
    return false;
}

void QtmPushDevice::socketConnected()
{
    qDebug() << "QBluetoothSocket connected";
    this->start_readings();
    emit connected();
}

void QtmPushDevice::socketDisconnected()
{
    qDebug() << "QBluetoothSocket disconnected";
    this->stop_readings();
    emit disconnected();
}

void QtmPushDevice::socketError(QBluetoothSocket::SocketError error)
{
    qDebug() << "QBluetoothSocket Error:" << error ;
}

void QtmPushDevice::timerEvent(QTimerEvent *)
{
    if(socket)
    {
        if(socket->isOpen() && socket->isWritable())
        {
//            qDebug() << "Sent Request";
            QByteArray text = "A\n";
            socket->write(text);
        }
    }
}

