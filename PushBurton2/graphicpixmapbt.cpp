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

#include "graphicpixmapbt.h"

GraphicPixmapBt::GraphicPixmapBt(
        QString filename,
        QGraphicsItem* parent) :
        QGraphicsObject(parent)
{
    px = new QPixmap;
    if(!px->load(filename)) {
        qDebug() << "Could not load pixmap at " << filename;
        delete px;
        px = 0;
    }

    altPx = 0;
    toggled = false;
    clickable = true;
    altImgActivated = false;
}

GraphicPixmapBt::~GraphicPixmapBt()
{
    if(altPx)
        delete altPx;
    if(px)
        delete px;
}

QRectF GraphicPixmapBt::boundingRect() const
{
    if(px)
        return QRectF(QPointF(0.0,0.0), px->size());
    else
        return QRectF();
}

QPainterPath GraphicPixmapBt::shape() const
{
    QPainterPath path;
    if(px)
        path.addRect(QRectF(QPointF(0.0,0.0), px->size()));
    return path;
}


void GraphicPixmapBt::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *)
{
    if(px) {
        QPixmap * p_px = (altImgActivated && altPx)? altPx : px;

        if(!toggled)
            painter->drawPixmap(QRect(0,0,p_px->width(),p_px->height()), *p_px);
        else
            painter->drawPixmap(QRect(0.1*p_px->width(),0.1*p_px->height(),0.8*p_px->width(),0.8*p_px->height()), *p_px);
    }

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

void GraphicPixmapBt::setAltImage(const QString& filename)
{
    altImgActivated = false;

    altPx = new QPixmap;

    if(!altPx->load(filename))
    {
        qDebug() << "Could not load pixmap at " << filename;
        delete altPx;
        altPx = 0;
    }
}

void GraphicPixmapBt::toggleAltImg(bool a)
{
    altImgActivated = a;
    this->update();
}

bool GraphicPixmapBt::getAltImg()
{
    return altImgActivated;
}
