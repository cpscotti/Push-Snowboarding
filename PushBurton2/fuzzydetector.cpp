/*
 * Copyright (c) 2011 Nokia Corporation
 *
 * This file is part of the Push Snowboarding Project, More info at:
 * www.pushsnowboading.com
 *
 * Author: Clovis Scotti <scotti@ieee.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */

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

    LoadFromXml(QString(FSC_RUNS_FOLDERS_ROOT)+QString("atdsettings.xml"));//if file doesn't exist, do nothing + save it

}

FuzzyDetector::~FuzzyDetector()
{

}

Response FuzzyDetector::AskGodAboutAirTime(double fp, double pa, double ia)
{
    double oa_fp = fp_onAir.fuzificate(fp);
    double oa_pa = pa_onAir.fuzificate(pa);
    double oa_ia = ia_onAir.fuzificate(ia);
    double onAir = std::max(oa_fp, std::max(oa_pa, oa_ia));

    double og_fp = fp_onGrnd.fuzificate(fp);
    double og_pa = pa_onGrnd.fuzificate(pa);
    double og_ia = ia_onGrnd.fuzificate(ia);
    double onGround = std::max(og_fp, std::max(og_pa, og_ia));

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
        SaveToXml(QString(FSC_RUNS_FOLDERS_ROOT)+QString("atdsettings.xml"));
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

    do
    {
        while( xml.readNextStartElement()) {
            if(xml.name() == "FP_OnAir") {
                do {
                while(xml.readNextStartElement()) {
                    if(xml.name() == "relation") {
                        fp_onAir.push_back(Relation(
                                xml.attributes().value("inVal").toString().toDouble(),
                                xml.attributes().value("outVal").toString().toDouble()
                                ));
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
