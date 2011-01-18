#ifndef PUSHDEVICESHOLDER_H
#define PUSHDEVICESHOLDER_H

#include <QVector>
#include <QString>
#include <QDebug>
#include <QObject>

#include "npushlogtick.h"
#include "pushburtongenericdevice.h"
#include "pushn8simulationdevice.h"
#include "npushtickdisposer.h"

class PushDevicesHolder : public QVector<PushBurtonGenericDevice *>
{
public:
    PushDevicesHolder();
    ~PushDevicesHolder();

    int deleteAllAndClear();

    bool CheckForDeviceType(QString&);

    void push_back(PushBurtonGenericDevice* ptr);


    NPushTickDisposer * tickDisposer;

//    bool CheckForDeviceName(QString&);
};

#endif // PUSHDEVICESHOLDER_H
