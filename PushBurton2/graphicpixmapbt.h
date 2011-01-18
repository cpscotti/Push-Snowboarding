#ifndef GRAPHICPIXMAPBT_H
#define GRAPHICPIXMAPBT_H

#include <QGraphicsObject>
#include <QDebug>
#include <QPixmap>
#include <QPainter>

class GraphicPixmapBt : public QGraphicsObject
{
    Q_OBJECT

public:
    GraphicPixmapBt(QString, QGraphicsItem* parent = 0);
    ~GraphicPixmapBt();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *);

    void set_clickable(bool a){
        clickable = a;
    }

signals:
    void activated();
    void released();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    bool toggled;
    bool clickable;
    QPixmap px;


};

#endif // GRAPHICPIXMAPBT_H
