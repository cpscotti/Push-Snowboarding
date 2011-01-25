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

#include "graphicssettings.h"

GraphicsSettings::GraphicsSettings(PushDevicesHolder* devs, QObject* parent=0) : configuredDevices(devs)
{
    this->setParent(parent);
    this->setZValue(0.2);

    homeBt = new GraphicPixmapBt(":/buttons/home_bt.png", this);
    homeBt->setPos(0,0);
    connect(homeBt, SIGNAL(activated()), this, SIGNAL(home_bt_clicked()));

    liveView = 0;

    startLiveViewBt = new GraphicTextBt("LIVE VIEW (DEBUG)", this);
    startLiveViewBt->setPos(110+5,0);
    connect(startLiveViewBt, SIGNAL(activated()), this, SLOT(start_live_view()));
}

GraphicsSettings::~GraphicsSettings()
{
    if(liveView)
        liveView->deleteLater();
}

QRectF GraphicsSettings::boundingRect() const
{
    return QRectF(0,0,340,382+6+54);
}

void GraphicsSettings::paint(QPainter *painter, const QStyleOptionGraphicsItem * option,QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(Qt::white);
    painter->drawRect(QRect(0,60,340,382));

    painter->setPen(QColor(17,83,161));

    painter->setFont(QFont("Nokia Standard Bold", 6));

    painter->drawText(QRect(0,60,340,382), Qt::AlignCenter, "Woohoo, nothing creepy here!\n We set it all up for you!");
}

void GraphicsSettings::start_live_view()
{
    if(liveView)
        liveView->deleteLater();

    for(int i=0;i<configuredDevices->count();i++) {
        configuredDevices->at(i)->start_readings();
    }

    qDebug() << "Opening up live viewer";

    liveView = new LiveView(configuredDevices);

    connect(liveView, SIGNAL(leaving()), this, SLOT(end_live_view()));

    liveView->showMaximized();
}

void GraphicsSettings::end_live_view()
{
//    for(int i=0;i<devicesHolder.count();i++) {
//        configuredDevices->at(i)->stop_readings();
//    }
    liveView->deleteLater();
    liveView = 0;
}
