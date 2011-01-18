#ifndef UBIQLOGSAVER_H
#define UBIQLOGSAVER_H

#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QQueue>
#include <QFile>
#include <QFSFileEngine>
#include <QXmlStreamWriter>

#include "npushlogtick.h"
#include "npushgenreport.h"

class UbiqLogSaver : public QThread
{
    Q_OBJECT
public:
    UbiqLogSaver(uint a_start_time);
    ~UbiqLogSaver();
    void run();

    void run_end();

public slots:
    void log_tick_in(NPushLogTick*);
    void report_in(NPushGenReport*);

private:
    void CreateRunDir();
    QQueue<NPushLogTick*> ticksBuffer;
    QQueue<NPushGenReport*> reportBuffer;

    uint start_time;
    bool saving_ticks;

    QMutex ticksLock;
    QMutex reportsLock;

    QWaitCondition runControlSignal;
    QMutex runControlLock;

    bool runEnd;
    QString dirName;

    QFile* dataFile;
    QXmlStreamWriter xml;
};

#endif // UBIQLOGSAVER_H
