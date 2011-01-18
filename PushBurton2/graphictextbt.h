#ifndef GRAPHICTEXTBT_H
#define GRAPHICTEXTBT_H

#include <QGraphicsObject>
#include <QPainter>
#include <QString>
#include <QFont>
#include <QColor>

#include <QDebug>

class GraphicTextBt : public QGraphicsObject
{
    Q_OBJECT
public:
    GraphicTextBt(QString, QGraphicsItem* parent = 0);
    ~GraphicTextBt();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *);

signals:
    void activated();
    void released();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool toggled;
    QString text;
    QColor textColor;
    QRectF btRect;
};

#endif // GRAPHICTEXTBT_H
