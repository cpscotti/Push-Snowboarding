#ifndef GRAPHICCONFIRMATIONREQUEST_H
#define GRAPHICCONFIRMATIONREQUEST_H

#include <QObject>
#include <QGraphicsObject>

#include <QString>
#include <QDebug>

#include "graphictextbt.h"

class GraphicConfirmationRequest : public QGraphicsObject
{
    Q_OBJECT
public:
    GraphicConfirmationRequest(const QString& question, QGraphicsItem* parent = 0);
    ~GraphicConfirmationRequest();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *);

signals:
    void accepted();
    void rejected();

public slots:
    void hide();

private:
    GraphicTextBt * questionPane;
    GraphicTextBt * yesPane;
    GraphicTextBt * noPane;

};

#endif // GRAPHICCONFIRMATIONREQUEST_H
