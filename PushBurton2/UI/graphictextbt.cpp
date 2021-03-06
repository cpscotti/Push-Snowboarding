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

#include "graphictextbt.h"

GraphicTextBt::GraphicTextBt(QString _text, QGraphicsItem* parent) : QGraphicsObject(parent), text(_text)
{
    toggled = false;

    textColor = QColor(17,83,161);
    btRect = QRectF(0,0,110+5+110,54);
}

GraphicTextBt::GraphicTextBt(QString _text, QRectF _rect, QGraphicsItem* parent)
    : QGraphicsObject(parent), text(_text), btRect(_rect)
{
    toggled = false;
}

void GraphicTextBt::setBtRect(const QRectF& nbtrect)
{
    btRect = nbtrect;
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
        painter->setFont(QFont("Nokia Standard Bold", 6));
    else
        painter->setFont(QFont("Nokia Standard Bold", 5));

    painter->setPen(QColor(17,83,161));

    painter->drawText(btRect, Qt::AlignCenter, text);
}

void GraphicTextBt::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    firstTch = event->screenPos();
    prevTch = event->screenPos();

    if(!toggled) {
        emit activated();
        toggled = true;
        this->update();
    }
}

void GraphicTextBt::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    QPoint now = event->screenPos();
    qreal entireFlight = sqrt(pow(now.x() - firstTch.x(),2.0) + pow(now.y() - firstTch.y(),2.0));

    if(toggled) {
//        qDebug() << "entireFlight = " << entireFlight;
        if(fabs(entireFlight) < 30.0) {
            emit released();
        }
        toggled = false;
        this->update();
    }
}

void GraphicTextBt::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    QPoint now = event->screenPos();
    qreal vertFlight = now.y() - prevTch.y();
//    qDebug() << "entireFlight = " << entireFlight;
//    qreal entireFlight = now.x() - prevTch.x();
    emit v_swipe_hint(vertFlight);
    prevTch = now;

}

void GraphicTextBt::v_swipe_action(qreal dif)
{
    QVariant oldY = this->property("y");
    this->setProperty("y", oldY.toFloat()+dif);
}
