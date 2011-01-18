#include "fuzzydetector.h"

FuzzyDetector::FuzzyDetector()
{
    fp_onAir.push_back(Relation(0.0, 1.0));//No pressure, air
    fp_onAir.push_back(Relation(2000.0, 1.0));
    fp_onAir.push_back(Relation(2600.0, 0.0));
    fp_onAir.push_back(Relation(4024.0, 0.0));//full pressure, dunno

    fp_onGrnd.push_back(Relation(0.0, 0.0));//No pressure, dunno
    fp_onGrnd.push_back(Relation(2600.0, 0.0));
    fp_onGrnd.push_back(Relation(3000.0, 0.9));
    fp_onGrnd.push_back(Relation(4024.0, 1.0));//full pressure, ground

    pa_onAir.push_back(Relation(0.0, 1.0));//no gravity, on air!
    pa_onAir.push_back(Relation(3.0, 1.0));
    pa_onAir.push_back(Relation(6.6, 0.0));
    pa_onAir.push_back(Relation(50.0, 0.0));//hell no!

//    fp_onGrnd.push_back(Relation(0.0, 0.0));//No pressure, dunno
//    fp_onGrnd.push_back(Relation(2600.0, 0.0));
//    fp_onGrnd.push_back(Relation(3300.0, 1.0));
//    fp_onGrnd.push_back(Relation(4024.0, 1.0));//full pressure, ground

//    pa_onAir.push_back(Relation(0.0, 1.0));//no gravity, on air!
//    pa_onAir.push_back(Relation(3.0, 1.0));
//    pa_onAir.push_back(Relation(6.0, 0.0));
//    pa_onAir.push_back(Relation(50.0, 0.0));//hell no!

    pa_onGrnd.push_back(Relation(0.0, 0.0));//no gravity, hell no
    pa_onGrnd.push_back(Relation(6.0, 0.0));
    pa_onGrnd.push_back(Relation(9.0, 1.0));
    pa_onGrnd.push_back(Relation(50.0, 1.0));//definitely on ground

    ia_onAir.push_back(Relation(0.0, 0.8));
    ia_onAir.push_back(Relation(0.6, 0.0));
    ia_onAir.push_back(Relation(8.0, 0.0));

    ia_onGrnd.push_back(Relation(0.0, 0.0));
    ia_onGrnd.push_back(Relation(1.0, 0.0));
    ia_onGrnd.push_back(Relation(3.0, 1.0));
    ia_onGrnd.push_back(Relation(8.0, 1.0));

    LoadFromXml("E:/atdsettings.xml");//if file doesn't exist, do nothing + save it

//    LoadFromXml("E:/saved.xml");
//    SaveToXml("E:/saved2.xml");
}

FuzzyDetector::~FuzzyDetector()
{

}

Response FuzzyDetector::AskGodAboutAirTime(double fp, double pa, double ia)
{
    double oa_fp = fp_onAir.fuzificate(fp);
    double oa_pa = pa_onAir.fuzificate(pa);
    double oa_ia = ia_onAir.fuzificate(ia);
    double onAir = Max(oa_fp, Max(oa_pa, oa_ia));

    double og_fp = fp_onGrnd.fuzificate(fp);
    double og_pa = pa_onGrnd.fuzificate(pa);
    double og_ia = ia_onGrnd.fuzificate(ia);
    double onGround = Max(og_fp, Max(og_pa, og_ia));

    Response resp;
    if(onAir > onGround) {
        if(onAir > 0.8 && onGround < 0.5) {
            resp = FlyingLikeAnEagle;

//            qDebug() << "onAir " << oa_fp <<"(fp)/"
//                    << oa_pa <<"(pa)/"
//                    << oa_ia << "(ia), onGround "
//                    << og_fp <<"(fp)/"
//                    << og_pa <<"(pa)/"
//                    << og_ia << "ia";
        } else {
            resp = CantHelpYouOnThisOne;
        }
    } else if(onAir < onGround){
        if(onGround > 0.8 && onAir < 0.5) {
            resp = SlidingDownLikeAPenguin;
        } else {
            resp = CantHelpYouOnThisOne;
        }
    } else {
        resp = CantHelpYouOnThisOne;
    }


//    if(onAir > onGround) {
//        if(onAir > 0.8) {
//            resp = FlyingLikeAnEagle;

//            qDebug() << "onAir " << oa_fp <<"(fp)/"
//                    << oa_pa <<"(pa)/"
//                    << oa_ia << "(ia), onGround "
//                    << og_fp <<"(fp)/"
//                    << og_pa <<"(pa)/"
//                    << og_ia << "ia";
//        } else {
//            resp = CantHelpYouOnThisOne;
//        }
//    } else if(onAir < onGround){
//        if(onGround > 0.8) {
//            resp = SlidingDownLikeAPenguin;
//        } else {
//            resp = CantHelpYouOnThisOne;
//        }
//    } else {
//        resp = CantHelpYouOnThisOne;
//    }

    return resp;
}

void FuzzyDetector::SaveToXml(QString settingsFileName)
{

    QFile reportFile(settingsFileName);

    reportFile.open(QFile::WriteOnly | QFile::Text);


    QXmlStreamWriter xml;

    xml.setDevice(&reportFile);
    xml.setAutoFormatting(true);

    xml.writeStartDocument();

    xml.writeStartElement("FuzzyDetectorSettings");

    xml.writeStartElement("FP_OnAir");
    for(int i=0;i<fp_onAir.count();i++) {
        xml.writeStartElement("relation");

        xml.writeAttribute("inVal", QString::number(fp_onAir.at(i).inVal));
        xml.writeAttribute("outVal", QString::number(fp_onAir.at(i).outVal));

        xml.writeEndElement();
    }
    xml.writeEndElement();

    xml.writeStartElement("FP_OnGrnd");
    for(int i=0;i<fp_onGrnd.count();i++) {
        xml.writeStartElement("relation");

        xml.writeAttribute("inVal", QString::number(fp_onGrnd.at(i).inVal));
        xml.writeAttribute("outVal", QString::number(fp_onGrnd.at(i).outVal));

        xml.writeEndElement();
    }
    xml.writeEndElement();

    xml.writeStartElement("PA_OnAir");
    for(int i=0;i<pa_onAir.count();i++) {
        xml.writeStartElement("relation");

        xml.writeAttribute("inVal", QString::number(pa_onAir.at(i).inVal));
        xml.writeAttribute("outVal", QString::number(pa_onAir.at(i).outVal));

        xml.writeEndElement();
    }
    xml.writeEndElement();

    xml.writeStartElement("PA_OnGrnd");
    for(int i=0;i<pa_onGrnd.count();i++) {
        xml.writeStartElement("relation");

        xml.writeAttribute("inVal", QString::number(pa_onGrnd.at(i).inVal));
        xml.writeAttribute("outVal", QString::number(pa_onGrnd.at(i).outVal));

        xml.writeEndElement();
    }
    xml.writeEndElement();

    xml.writeStartElement("IA_OnAir");
    for(int i=0;i<ia_onAir.count();i++) {
        xml.writeStartElement("relation");

        xml.writeAttribute("inVal", QString::number(ia_onAir.at(i).inVal));
        xml.writeAttribute("outVal", QString::number(ia_onAir.at(i).outVal));

        xml.writeEndElement();
    }
    xml.writeEndElement();

    xml.writeStartElement("IA_OnGrnd");
    for(int i=0;i<ia_onGrnd.count();i++) {
        xml.writeStartElement("relation");

        xml.writeAttribute("inVal", QString::number(ia_onGrnd.at(i).inVal));
        xml.writeAttribute("outVal", QString::number(ia_onGrnd.at(i).outVal));

        xml.writeEndElement();
    }
    xml.writeEndElement();

    xml.writeEndElement();//FuzzyDetectorSettings

    xml.writeEndDocument();

    reportFile.close();
}

void FuzzyDetector::LoadFromXml(QString settingsFileName)
{

    QFile data_input(settingsFileName);
    if(! data_input.open(QFile::ReadOnly | QFile::Text))
    {
        SaveToXml("E:/atdsettings.xml");
        return;
    }

    QXmlStreamReader xml;
    xml.setDevice(&data_input);

    fp_onAir.clear();
    fp_onGrnd.clear();

    pa_onAir.clear();
    pa_onGrnd.clear();

    ia_onAir.clear();
    ia_onGrnd.clear();

    if(xml.readNextStartElement())
    {
        if(xml.name() != "FuzzyDetectorSettings") {
            qDebug() << "not a fuzzy detector log file";
            return;
        }
    }

    qDebug() << "before running everything up";

    do
    {
        while( xml.readNextStartElement()) {
            if(xml.name() == "FP_OnAir") {
                do {
                while(xml.readNextStartElement()) {
                    qDebug() << "running fp_onAir";
                    if(xml.name() == "relation") {
                        fp_onAir.push_back(Relation(
                                xml.attributes().value("inVal").toString().toDouble(),
                                xml.attributes().value("outVal").toString().toDouble()
                                ));
                        qDebug() << "Added one fp_onAir";
                    } else {
                        xml.skipCurrentElement();
                    }
                } } while(xml.name() != "FP_OnAir");
            } else if(xml.name() == "FP_OnGrnd") {
                do {
                while(xml.readNextStartElement()) {
                    if(xml.name() == "relation") {
                        fp_onGrnd.push_back(Relation(
                                xml.attributes().value("inVal").toString().toDouble(),
                                xml.attributes().value("outVal").toString().toDouble()
                                ));
                    } else {
                        xml.skipCurrentElement();
                    }
                } } while(xml.name() != "FP_OnGrnd");
            } else if(xml.name() == "PA_OnAir") {
                do {
                while(xml.readNextStartElement()) {
                    if(xml.name() == "relation") {
                        pa_onAir.push_back(Relation(
                                xml.attributes().value("inVal").toString().toDouble(),
                                xml.attributes().value("outVal").toString().toDouble()
                                ));
                    } else {
                        xml.skipCurrentElement();
                    }
                } } while(xml.name() != "PA_OnAir");
            } else if(xml.name() == "PA_OnGrnd") {
                do {
                while(xml.readNextStartElement()) {
                    if(xml.name() == "relation") {
                        pa_onGrnd.push_back(Relation(
                                xml.attributes().value("inVal").toString().toDouble(),
                                xml.attributes().value("outVal").toString().toDouble()
                                ));
                    } else {
                        xml.skipCurrentElement();
                    }
                } } while(xml.name() != "PA_OnGrnd");
            } else if(xml.name() == "IA_OnAir") {
                do {
                while(xml.readNextStartElement()) {
                    if(xml.name() == "relation") {
                        ia_onAir.push_back(Relation(
                                xml.attributes().value("inVal").toString().toDouble(),
                                xml.attributes().value("outVal").toString().toDouble()
                                ));
                    } else {
                        xml.skipCurrentElement();
                    }
                } } while(xml.name() != "IA_OnAir");
            } else if(xml.name() == "IA_OnGrnd") {
                do {
                while(xml.readNextStartElement()) {
                    if(xml.name() == "relation") {
                        ia_onGrnd.push_back(Relation(
                                xml.attributes().value("inVal").toString().toDouble(),
                                xml.attributes().value("outVal").toString().toDouble()
                                ));
                    } else {
                        xml.skipCurrentElement();
                    }
                } } while(xml.name() != "IA_OnGrnd");
            } else {
                xml.skipCurrentElement();
            }
        }
    }while(xml.name() != "FuzzyDetectorSettings");
}
