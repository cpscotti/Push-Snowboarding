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

#ifndef GRAPHICSCONNMENU_H
#define GRAPHICSCONNMENU_H

#include <QGraphicsObject>

#include <QStateMachine>

#include "graphicpixmapbt.h"
#include "pushburtonanimation.h"

#include "graphicslidingdownbts.h"

class GraphicsConnMenu : public QGraphicsObject
{
    Q_OBJECT
public:
    GraphicsConnMenu(QObject*);
    ~GraphicsConnMenu();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *);

signals:
    void home_bt_clicked();
    void startSearch_bt_clicked();
    void stopSearch_bt_clicked();
    void kit_selected(int n);

private slots:
    void kit_selected(QString kit_selected);

//    void kit_1_selected();
//    void kit_2_selected();
//    void kit_3_selected();
//    void kit_sim_selected();

private:
    bool searching;
    GraphicPixmapBt * homeBt;
    GraphicPixmapBt * startBt;
    GraphicPixmapBt * stopBt;
//    GraphicPixmapBt * kit1Bt;
//    GraphicPixmapBt * kit2Bt;
//    GraphicPixmapBt * kit3Bt;
//    GraphicPixmapBt * kitSimBt;

    GraphicSlidingDownBts * slidingDownBts;

    QState * rootState;
    //unfolds into
        QState * btState;
        //unfolds into
            QState * btStandBy;
            QState * btSearching;
        // parallel to
/*        QState * kitState;
        //unfolds into
            QState * dKit1;
            QState * dKit2;
            QState * dKit3;
            QState * dKitSim;
            QState * dChooser;
*/

    QStateMachine machine;

    void setupStateMachine();

};

#endif // GRAPHICSCONNMENU_H
