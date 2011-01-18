#include "runmanager.h"

RunManager::RunManager(PushDevicesHolder * aDevHolder, QObject *parent) :
    configuredDevices(aDevHolder), QObject(parent)
{
    logging = false;
    ubiqSaver = 0;
}

RunManager::~RunManager()
{
}

void RunManager::start_logging()
{
    if(logging)
        return;
    logging = true;
    uint startTime = QDateTime::currentDateTime().toUTC().toTime_t();

    ubiqSaver = new UbiqLogSaver(startTime);

    UpdateDataStreamConnections();

    for(int i=0;i<configuredDevices->count();i++)
    {
        PushBurtonGenericDevice * dev = configuredDevices->at(i);
        dev->start_readings();
        dev->start_run();
    }
}

void RunManager::stop_logging()
{
    if(!logging)
        return;
    logging = false;

    for(int i=0;i<configuredDevices->count();i++)
    {
        PushBurtonGenericDevice * dev = configuredDevices->at(i);
        dev->end_run();
        dev->stop_readings();
    }

    ubiqSaver->run_end();
    delete ubiqSaver;
    ubiqSaver = 0;

    UpdateDataStreamConnections();
}

void RunManager::UpdateDataStreamConnections()
{
    for(int i=0;i<configuredDevices->count();i++)
    {
        PushBurtonGenericDevice * dev = configuredDevices->at(i);
        if(ubiqSaver){
            disconnect(dev, SIGNAL(report_ready(NPushGenReport*)), ubiqSaver, SLOT(report_in(NPushGenReport*)));
            disconnect(dev, SIGNAL(reading_ready(NPushLogTick*)), ubiqSaver, SLOT(log_tick_in(NPushLogTick*)));
        }

        if(logging)
        {
            if(ubiqSaver) {
                connect(dev, SIGNAL(report_ready(NPushGenReport*)), ubiqSaver, SLOT(report_in(NPushGenReport*)));
                connect(dev, SIGNAL(reading_ready(NPushLogTick*)), ubiqSaver, SLOT(log_tick_in(NPushLogTick*)));
            }
        }
    }
}
