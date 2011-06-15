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

#ifndef RUNCONTROLDIALOG_H
#define RUNCONTROLDIALOG_H

#include <QDebug>
#include <QGraphicsObject>
#include <QStateMachine>
#include <QFont>
#include <QDateTime>
#include <QTime>
#include "graphicpixmapbt.h"

class RunControlDialog : public QGraphicsObject
{
    Q_OBJECT
public:
    RunControlDialog(QObject*);
    ~RunControlDialog();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *);

protected:
    void timerEvent(QTimerEvent *);

private:
    GraphicPixmapBt * homeBt;
    GraphicPixmapBt * startRunBt;
    GraphicPixmapBt * stopRunBt;

    QState * rootState;
    QState * standBy;
    QState * logging;
    QStateMachine machine;
    QFont nokiaFont;

    void init_state_machine();

    uint runStart;
    bool run_active;
    int timerId;

private slots:
    void logging_start();
    void logging_end();


signals:
    void home_bt_clicked();
    void start_run();
    void stop_run();

};

#endif // RUNCONTROLDIALOG_H
