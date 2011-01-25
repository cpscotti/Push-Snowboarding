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

#ifndef GRAPHICSDEVICEINFO_H
#define GRAPHICSDEVICEINFO_H

#include <QGraphicsObject>
#include <QPixmap>
#include <QPainter>
#include <QDebug>

#include <QTimerEvent>
#include <QFont>

class GraphicsDeviceInfo : public QGraphicsObject
{
    Q_OBJECT
public:
    GraphicsDeviceInfo(QObject*);
    ~GraphicsDeviceInfo();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *);

signals:
    void activated();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void timerEvent(QTimerEvent *);

public slots:
    void location_online();
    void snowb_online();
    void heart_online();
    void arm_online();
    void boot_r_online();
    void boot_l_online();

    void location_conn();
    void snowb_conn();
    void heart_conn();
    void arm_conn();
    void boot_r_conn();
    void boot_l_conn();

private:
    int timerId;
    QPixmap centralPx;

    bool locationOn;
    int locationBlink;

    bool snowbOn;
    int snowbBlink;

    bool heartOn;
    int heartBlink;

    bool armOn;
    int armBlink;

    bool rightBootOn;
    int rightBootBlink;

    bool leftBootOn;
    int leftBootBlink;
};

#endif // GRAPHICSDEVICEINFO_H
