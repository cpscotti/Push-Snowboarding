#include "ubiqlogsaver.h"

UbiqLogSaver::UbiqLogSaver(uint a_start_time)
{
    start_time = a_start_time;
    CreateRunDir();

    QString rawLogFilename = dirName + "/RawLog.xml";
    dataFile = new QFile(rawLogFilename);


    dataFile->open(QFile::WriteOnly | QFile::Text);
    xml.setDevice(dataFile);
    xml.setAutoFormatting(true);

    xml.writeStartDocument();
    xml.writeStartElement("N8SensorsLog");

    runEnd = false;
    this->start();
}

UbiqLogSaver::~UbiqLogSaver()
{
    this->run_end();//will return immediately if everything went ok since runEnd will be true
}

void UbiqLogSaver::run()
{
    while(true)
    {
        bool leaving = false;
        runControlLock.lock();
        if(!runEnd)
            runControlSignal.wait(&runControlLock);
        else
            leaving = true;
        runControlLock.unlock();


        ticksLock.lock();
        while(ticksBuffer.count() > 0){
            NPushLogTick * tick = ticksBuffer.dequeue();
            ticksLock.unlock();
            tick->dump_to_xml(xml);
            dataFile->flush();
            ticksLock.lock();
        }
        ticksLock.unlock();

        reportsLock.lock();
        while(reportBuffer.count() > 0){
            qDebug() << "report being dumped";
            NPushGenReport * repr = reportBuffer.dequeue();

            reportsLock.unlock();
            repr->save_to_dir(dirName);
            reportsLock.lock();
        }
        reportsLock.unlock();


        if(leaving){
            break;
        }
    }
    exit(0);
}

void UbiqLogSaver::run_end()
{
    if(runEnd)
        return;
    qDebug() << "before signaling exit";
    runControlLock.lock();
    runEnd = true;
    runControlLock.unlock();
    qDebug() << "waking up saver";
    runControlSignal.wakeAll();
    this->wait();//Adding a timeout here? not!?
    qDebug() << "saver exited";
    xml.writeEndElement();//N8SensorsLog
    xml.writeEndDocument();
    dataFile->close();
    qDebug() << "File closed";
}

void UbiqLogSaver::CreateRunDir()
{
    dirName = "E:/NPBRun";
    dirName += QString::number(start_time);
    QFSFileEngine fse;
    fse.mkdir(dirName, false);
}


void UbiqLogSaver::log_tick_in(NPushLogTick* tick)
{
    ticksLock.lock();
    ticksBuffer.enqueue(tick);
    ticksLock.unlock();
    runControlSignal.wakeAll();
}

void UbiqLogSaver::report_in(NPushGenReport* rep)
{
    reportsLock.lock();
    reportBuffer.enqueue(rep);
    reportsLock.unlock();
    runControlSignal.wakeAll();
}
