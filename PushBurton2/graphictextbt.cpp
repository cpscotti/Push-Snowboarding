#include "graphictextbt.h"

GraphicTextBt::GraphicTextBt(QString _text, QGraphicsItem* parent) : QGraphicsObject(parent), text(_text)
{
    toggled = false;

    textColor = QColor(17,83,161);
    btRect = QRectF(0,0,110+5+110,54);
}

GraphicTextBt::~GraphicTextBt()
{

}

QRectF GraphicTextBt::boundingRect() const
{
    return btRect;
}

void GraphicTextBt::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *)
{
    painter->setBrush(Qt::white);

    if(!toggled)
        painter->drawRect(btRect);
    else
        painter->drawRect(QRect(0.1*btRect.width(),0.1*btRect.height(),0.8*btRect.width(),0.8*btRect.height()));

    painter->setPen(textColor);

    if(!toggled)
        painter->setFont(QFont("Nokia Standard Bold", 5));
    else
        painter->setFont(QFont("Nokia Standard Bold", 4));

    painter->drawText(btRect, Qt::AlignCenter, text);
}

void GraphicTextBt::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    if(!toggled) {
        emit activated();
        toggled = true;
        this->update();
    }
}

void GraphicTextBt::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    if(toggled) {
        emit released();
        toggled = false;
        this->update();
    }
}
