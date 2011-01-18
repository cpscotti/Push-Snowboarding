#ifndef PUSHN8SIMULATIONDEVICE_H
#define PUSHN8SIMULATIONDEVICE_H

#include <QtXml>

#include <QDebug>
#include <QFile>
#include <QFileDialog>

#include <QAccelerometerReading>
#include <QMagnetometerReading>
#include <QGeoPositionInfo>

#include "pushburtongenericdevice.h"

#include "npushacctick.h"
#include "npushfoottick.h"
#include "npushgpstick.h"
#include "npushgsrtick.h"
#include "npushhearttick.h"
#include "npushimutick.h"
#include "npushmagtick.h"
#include "npushlogtick.h"

class PushN8SimulationDevice : public PushBurtonGenericDevice
{
    Q_OBJECT
public:
    PushN8SimulationDevice();
    ~PushN8SimulationDevice();

    QString get_description();

    bool is_online();

private:
    QFile * data_input;
    QXmlStreamReader xml;

    NPushAccTick * readAccTick();
    NPushFootTick * readFootTick();
    NPushGpsTick * readGpsTick();
    NPushGSRTick * readGsrTick();
    NPushHeartTick * readHeartTick();
    NPushIMUTick * readImuTick();
    NPushMagTick * readMagTick();

protected:
    void timerEvent(QTimerEvent *);
};

#endif // PUSHN8SIMULATIONDEVICE_H
