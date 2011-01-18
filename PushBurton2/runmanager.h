#ifndef RUNMANAGER_H
#define RUNMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QDebug>

#include "pushdevicesholder.h"
#include "ubiqlogsaver.h"
#include "npushgenreport.h"
#include "npushlogtick.h"

class RunManager : public QObject
{
    Q_OBJECT
public:
    explicit RunManager(PushDevicesHolder * aDevHolder, QObject *parent = 0);
    ~RunManager();

signals:

public slots:
    void start_logging();
    void stop_logging();

private:
    PushDevicesHolder * configuredDevices;
    UbiqLogSaver * ubiqSaver;
    bool logging;

    void UpdateDataStreamConnections();

};

#endif // RUNMANAGER_H
