#ifndef NPUSHLOGTICK_H
#define NPUSHLOGTICK_H


#include <QtXml>
#include <QString>
#include <QObject>

class NPushLogTick : QObject
{
    Q_OBJECT
public:

    virtual void append_to_xml(QDomDocument& doc, QDomElement& root) const = 0;
    virtual void dump_to_xml(QXmlStreamWriter& xml) const = 0;

    virtual QString get_pretty_print() const = 0;

    virtual ~NPushLogTick() = 0;

};

#endif // NPUSHLOGTICK_H
