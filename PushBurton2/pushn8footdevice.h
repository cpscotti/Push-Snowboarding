#ifndef PUSHN8FOOTDEVICE_H
#define PUSHN8FOOTDEVICE_H

#include <QString>
#include <QObject>
#include "pushn8btdevice.h"

#include "npushfoottick.h"

class PushN8FootDevice : public PushN8BtDevice
{
    Q_OBJECT
public:
    PushN8FootDevice(QBtDevice a_device, QObject *parent = 0);

    ~PushN8FootDevice();

    char get_side();

private:
    char side;

public slots:
    void gotDataReceived(QString);
};

#endif // PUSHN8FOOTDEVICE_H
