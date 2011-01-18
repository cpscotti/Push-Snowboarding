#ifndef PUSHN8IMUDEVICE_H
#define PUSHN8IMUDEVICE_H

#include <QString>
#include <QObject>
#include "pushn8btdevice.h"

#include "npushimutick.h"

class PushN8IMUDevice : public PushN8BtDevice
{
    Q_OBJECT
public:
    PushN8IMUDevice(QBtDevice a_device, QObject *parent = 0);

    ~PushN8IMUDevice();

public slots:
    void gotDataReceived(QString);


};

#endif // PUSHN8IMUDEVICE_H
