#ifndef NPUSHTICKDISPOSER_H
#define NPUSHTICKDISPOSER_H

#include <QObject>
#include <QQueue>
#include <QDebug>

#include "npushlogtick.h"

class NPushTickDisposer : public QObject
{
    Q_OBJECT
public:
    explicit NPushTickDisposer(QObject *parent = 0);
    ~NPushTickDisposer();

    void perform_maintenance();

    int get_queue_count();

signals:

public slots:
    void log_ticks_sink(NPushLogTick *);

private:
    QQueue<NPushLogTick *> ticksQueue;

    static const int maxQueueSize = 2000;

};

#endif // NPUSHTICKDISPOSER_H
