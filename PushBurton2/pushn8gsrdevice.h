#ifndef PUSHN8GSRDEVICE_H
#define PUSHN8GSRDEVICE_H

#include <QString>
#include <QObject>
#include "pushn8btdevice.h"

#include "npushgsrtick.h"

class PushN8GSRDevice : public PushN8BtDevice
{
    Q_OBJECT
public:
    PushN8GSRDevice(QBtDevice a_device, QObject *parent = 0);

    ~PushN8GSRDevice();

public slots:
    void gotDataReceived(QString);
};

#endif // PUSHN8GSRDEVICE_H
