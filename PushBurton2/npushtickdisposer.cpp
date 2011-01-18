#include "npushtickdisposer.h"

NPushTickDisposer::NPushTickDisposer(QObject *parent) :
    QObject(parent)
{
    qDebug() << "Tick Disposer initialized";
}

NPushTickDisposer::~NPushTickDisposer()
{
    qDebug() << "Tick Disposer Destructor, gotta get rid of " << get_queue_count() << " logTicks";
    while(ticksQueue.count() > 0){
        NPushLogTick * dyingTick = ticksQueue.dequeue();
        delete dyingTick;
    }
    qDebug() << "Cleanup finished";
}

void NPushTickDisposer::log_ticks_sink(NPushLogTick * tick)
{
    ticksQueue.enqueue(tick);
    perform_maintenance();
}


void NPushTickDisposer::perform_maintenance()
{
    static int msgCnt = 0;
    while(ticksQueue.count() > maxQueueSize) {
        NPushLogTick * dyingTick = ticksQueue.dequeue();
        delete dyingTick;

        if(msgCnt == 0) {
            qDebug() << "Cleaning up";
            msgCnt++;
        }
    }
}


int NPushTickDisposer::get_queue_count()
{
    return ticksQueue.count();
}
