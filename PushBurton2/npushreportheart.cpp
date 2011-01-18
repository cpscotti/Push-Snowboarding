#include "npushreportheart.h"

NPushReportHeart::NPushReportHeart() : graphPoints(100, 0.0)
{
    running = false;
    Heart_max = 0;
    Heart_avg = 0;

    Heart_acc = 0;
    Heart_cnt = 0;
}

NPushReportHeart::~NPushReportHeart()
{

}

bool NPushReportHeart::start_run()
{
    QMutexLocker locker(&dataAccessMutex);

    if(running)
        return false;

    graphPoints.clear();

    Heart_max = 0;
    Heart_avg = 0;

    Heart_acc = 0;
    Heart_cnt = 0;

    running = true;
    return true;
}


bool NPushReportHeart::end_run()
{
    QMutexLocker locker(&dataAccessMutex);

    if(!running)
        return false;

    if(Heart_cnt > 0)
        Heart_avg = (int)(Heart_acc/(long int)Heart_cnt);
    else
        Heart_avg = 0;

    running = false;
    return true;
}


void NPushReportHeart::append_new_reading(int reading)
{
    QMutexLocker locker(&dataAccessMutex);

    if(reading > Heart_max) {
        Heart_max = reading;
    }

    Heart_acc += reading;
    Heart_cnt++;

    graphPoints.push_back((double)reading);

}

bool NPushReportHeart::save_to_dir(QString &dirName)
{

    QString reportFilename = dirName + "/Heart.xml";
    QFile reportFile(reportFilename);

    reportFile.open(QFile::WriteOnly | QFile::Text);

    qDebug() << "Heart -> Saving to dir";

    QXmlStreamWriter xml;

    xml.setDevice(&reportFile);
    xml.setAutoFormatting(true);

    xml.writeStartDocument();

    dataAccessMutex.lock();


    xml.writeStartElement("HeartReport");

    xml.writeStartElement("stats");

    xml.writeAttribute("max", QString::number(Heart_max));
    xml.writeAttribute("avg", QString::number(Heart_avg));

    xml.writeEndElement();//stats

    xml.writeStartElement("graph");

    SelfShrinkingList::iterator it;
    for(it = graphPoints.begin();it != graphPoints.end();++it) {
        xml.writeStartElement("point");

        xml.writeAttribute("val", QString::number(*it));

        xml.writeEndElement();
    }
    xml.writeEndElement();//graph

    xml.writeEndElement();//HeartReport
    dataAccessMutex.unlock();

    xml.writeEndDocument();

    reportFile.close();

    qDebug() << "Heart -> Ended";
}

bool NPushReportHeart::load_from_dir(QString &dirName)
{
    QString data_in_filename = dirName + "/Heart.xml";
    QFile data_input(data_in_filename);
    data_input.open(QFile::ReadOnly | QFile::Text);

    QXmlStreamReader xml;
    xml.setDevice(&data_input);

    if(xml.readNextStartElement())
    {
        if(xml.name() != "HeartReport") {
            qDebug() << "Invalid file format";
            return false;
        }
    } else {
        qDebug() << "Could not read first start Element";
        return false;
    }

    do
    {
        while(xml.readNextStartElement())
        {
            if(xml.name() == "stats")
            {
                Heart_max = xml.attributes().value("max").toString().toInt();
                Heart_avg = xml.attributes().value("avg").toString().toInt();
            } else if(xml.name() == "graph") {
                read_points_from_xml(xml);
            } else {
                xml.skipCurrentElement();
            }
        }
    }   while(xml.name() != "HeartReport");

//    qDebug() << "Done; " << Heart_max << "/" << Heart_avg << "; " << graphPoints.count() << "points";
}

void NPushReportHeart::read_points_from_xml(QXmlStreamReader& xml)
{
    graphPoints.clear();

    do
    {
        while(xml.readNextStartElement()) {
            if(xml.name() == "point") {
                graphPoints.push_back(xml.attributes().value("val").toString().toDouble());
            } else {
                xml.skipCurrentElement();
            }
        }
    } while(xml.name() != "graph");
}

