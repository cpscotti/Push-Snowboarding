#include "graphicpixmapbt.h"

GraphicPixmapBt::GraphicPixmapBt(
        QString filename,
        QGraphicsItem* parent) :
        QGraphicsObject(parent)
{
    px = QPixmap(filename);
    toggled = false;
    clickable = true;
}

GraphicPixmapBt::~GraphicPixmapBt()
{

}

QRectF GraphicPixmapBt::boundingRect() const
{
    return QRectF(QPointF(0.0,0.0), px.size());
}

QPainterPath GraphicPixmapBt::shape() const
{
    QPainterPath path;
    path.addRect(QRectF(QPointF(0.0,0.0), px.size()));
    return path;
}


void GraphicPixmapBt::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *)
{
    if(!toggled)
        painter->drawPixmap(QRect(0,0,px.width(),px.height()), px);
    else
        painter->drawPixmap(QRect(0.1*px.width(),0.1*px.height(),0.8*px.width(),0.8*px.height()), px);
}

void GraphicPixmapBt::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    if(!toggled && clickable) {
        emit activated();
        toggled = true;
        this->update();
    }
}

void GraphicPixmapBt::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    if(toggled && clickable) {
        emit released();
        toggled = false;
        this->update();
    }
}
