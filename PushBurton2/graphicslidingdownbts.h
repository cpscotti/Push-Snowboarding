#ifndef GRAPHICSLIDINGDOWNBTS_H
#define GRAPHICSLIDINGDOWNBTS_H

#include <QDebug>
#include <QGraphicsObject>
#include <QVector>
#include <QSignalMapper>
#include <QStateMachine>
#include <QPropertyAnimation>

#include "graphictextbt.h"


class GraphicSlidingDownBts : public QGraphicsObject
{
    Q_OBJECT
public:
    GraphicSlidingDownBts(QGraphicsItem* parent = 0);
    ~GraphicSlidingDownBts();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *);

    void addBt(QString text, QString value);

    void construction_finished();

signals:
    void bt_selected(const QString& val);

private slots:
    void inn_selected(const QString& val);

private:

    void push_back(GraphicTextBt*, QString);

    QVector<GraphicTextBt*> graphicBts;
    QVector<QState *> selectedStates;
    QState * rootState;
    QState * chooserState;
    QStateMachine machine;
    QSignalMapper * selectedMapper;
};

#endif // GRAPHICSLIDINGDOWNBTS_H
