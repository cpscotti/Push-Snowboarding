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

#include "graphicsdeviceinfo.h"

GraphicsDeviceInfo::GraphicsDeviceInfo(QObject * parent)
{
    this->setParent(parent);
    centralPx = QPixmap(":/images/status_inner.png");

    timerId = startTimer(250);


    locationOn = snowbOn = heartOn = armOn = rightBootOn = leftBootOn = false;
    locationBlink = snowbBlink = heartBlink = armBlink = rightBootBlink = leftBootBlink = 0;
}

GraphicsDeviceInfo::~GraphicsDeviceInfo()
{

}

QRectF GraphicsDeviceInfo::boundingRect() const
{
    return QRectF(QPointF(0.0,0.0), centralPx.size());
}

QPainterPath GraphicsDeviceInfo::shape() const
{
    QPainterPath path;
    path.addRect(QRectF(QPointF(0.0,0.0), centralPx.size()));
    return path;
}

void GraphicsDeviceInfo::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *)
{
    painter->drawPixmap(QRect(0,0,centralPx.width(),centralPx.height()), centralPx);

    if(snowbBlink)
        snowbBlink *= -1;
    if(locationBlink)
        locationBlink *= -1;
    if(heartBlink)
        heartBlink *= -1;
    if(armBlink)
        armBlink *= -1;
    if(leftBootBlink)
        leftBootBlink *= -1;
    if(rightBootBlink)
        rightBootBlink *= -1;

    if(snowbOn || (snowbBlink > 0))
        painter->setBrush(Qt::green);
    else
        painter->setBrush(Qt::red);    
    painter->drawEllipse(QRectF(225, 216, 15, 15));

    if(locationOn || (locationBlink > 0))
        painter->setBrush(Qt::green);
    else
        painter->setBrush(Qt::red);
    painter->drawEllipse(QRectF(187, 95, 15, 15));

    if(heartOn || (heartBlink > 0))
        painter->setBrush(Qt::green);
    else
        painter->setBrush(Qt::red);
    painter->drawEllipse(QRectF(140, 134, 15, 15));

    if(armOn || (armBlink > 0))
        painter->setBrush(Qt::green);
    else
        painter->setBrush(Qt::red);
    painter->drawEllipse(QRectF(92, 191, 15, 15));

    if(leftBootOn || (leftBootBlink > 0))
        painter->setBrush(Qt::green);
    else
        painter->setBrush(Qt::red);
    painter->drawEllipse(QRectF(167, 316, 15, 15));

    if(rightBootOn || (rightBootBlink > 0))
        painter->setBrush(Qt::green);
    else
        painter->setBrush(Qt::red);
    painter->drawEllipse(QRectF(112, 316, 15, 15));

}

void GraphicsDeviceInfo::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    emit activated();
}

void GraphicsDeviceInfo::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
}

void GraphicsDeviceInfo::location_online()
{
    locationOn = true;
    locationBlink = 0;
    this->update();
}

void GraphicsDeviceInfo::location_conn()
{
    locationBlink = 1;
}

void GraphicsDeviceInfo::snowb_online()
{
    snowbOn = true;
    snowbBlink = 0;
    this->update();
}

void GraphicsDeviceInfo::snowb_conn()
{
    snowbBlink = 1;
}

void GraphicsDeviceInfo::heart_online()
{
    heartOn = true;
    heartBlink = 0;
    this->update();
}

void GraphicsDeviceInfo::heart_conn()
{
    heartBlink = 1;
}

void GraphicsDeviceInfo::arm_online()
{
    armOn = true;
    armBlink = 0;
    this->update();
}

void GraphicsDeviceInfo::arm_conn()
{
    armBlink = 1;
}

void GraphicsDeviceInfo::boot_r_online()
{
    rightBootOn = true;
    rightBootBlink = 0;
    this->update();
}

void GraphicsDeviceInfo::boot_r_conn()
{
    rightBootBlink = 1;
}

void GraphicsDeviceInfo::boot_l_online()
{
    leftBootOn = true;
    leftBootBlink = 0;
    this->update();
}

void GraphicsDeviceInfo::boot_l_conn()
{
    leftBootBlink = 1;
}

void GraphicsDeviceInfo::timerEvent(QTimerEvent * event)
{
    if(event->timerId() == timerId) {
        if(leftBootBlink
           || rightBootBlink
           || armBlink
           || heartBlink
           || snowbBlink
           || locationBlink)
        {
            this->update();
        }
    }
}
