#ifndef PUSHN8HEARTDEVICE_H
#define PUSHN8HEARTDEVICE_H

#include <QString>
#include <QObject>
#include "pushn8btdevice.h"

#include "npushhearttick.h"


class PushN8HeartDevice : public PushN8BtDevice
{
    Q_OBJECT
public:
    PushN8HeartDevice(QBtDevice a_device, QObject *parent = 0);

    ~PushN8HeartDevice();

public slots:
    void gotDataReceived(QString);
};

#endif // PUSHN8HEARTDEVICE_H
