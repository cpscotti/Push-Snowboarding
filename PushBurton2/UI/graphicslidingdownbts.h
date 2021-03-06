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

#ifndef GRAPHICSLIDINGDOWNBTS_H
#define GRAPHICSLIDINGDOWNBTS_H

#include <QDebug>
#include <QGraphicsObject>
#include <QVector>
#include <QSignalMapper>
#include <QStateMachine>
#include <QHistoryState>

#include <QPropertyAnimation>
#include <QTimer>

#include "graphictextbt.h"


class GraphicSlidingDownBts : public QGraphicsObject
{
    Q_OBJECT
public:
    GraphicSlidingDownBts(QGraphicsItem* parent = 0);
    ~GraphicSlidingDownBts();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *);

    void setBtsRect(const QRectF& a_btsrect);

    void addBt(QString text, QString value);

    void construction_finished();

    void setStartBt(int idx);

signals:
    void bt_selected(const QString& val);

private slots:
    void inn_selected(const QString& val);

private:

    void push_back(GraphicTextBt*, QString);

    int initial_selection;

    bool isOnChooser;
    qreal totalSwipeDisplacement;
    qreal swipeLowerBound;
    qreal swipeUpperBound;

    QRectF btsRect;

    QVector<GraphicTextBt*> graphicBts;
    QVector<QState *> selectedStates;
    QState * rootState;
    QState * chooserState;
    QState * optionSelectedState;
    QStateMachine machine;
    QSignalMapper * selectedMapper;

    QHistoryState * fallBackHistState;
    QTimer * fallBackTimer;
    static const int menuTimeout = 2000;

//    QString lastInnSelection;

signals:
    void do_swipe(qreal ydif);

public slots:
    void get_swipe_hints(qreal ydif);
    void entered_chooser();
    void exited_chooser();

};

#endif // GRAPHICSLIDINGDOWNBTS_H
