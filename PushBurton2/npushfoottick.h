#ifndef NPUSHFOOTTICK_H
#define NPUSHFOOTTICK_H

#include "npushlogtick.h"

#include <QtXml>
#include <QTime>
#include <QString>

class NPushFootTick : public NPushLogTick
{
public:
    NPushFootTick(char a_side, int a_toes, int a_heel);
    NPushFootTick(char a_side, int a_toes, int a_heel, quint64 a_msecsToEpoch);
    ~NPushFootTick();

    void append_to_xml(QDomDocument& doc, QDomElement& root) const;
    void dump_to_xml(QXmlStreamWriter& xml) const;
    QString get_pretty_print() const;

public:

    char side;
    int toes;
    int heel;

    quint64 msecsToEpoch;

};

#endif // NPUSHFOOTTICK_H
