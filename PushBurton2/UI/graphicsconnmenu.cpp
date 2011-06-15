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

#include "graphicsconnmenu.h"

GraphicsConnMenu::GraphicsConnMenu(QObject * parent)
{
    this->setParent(parent);
    this->setZValue(0.2);

    homeBt = new GraphicPixmapBt(":/buttons/home_bt.png", this);
    startBt = new GraphicPixmapBt(":/buttons/search_bt.png", this);
    stopBt = new GraphicPixmapBt(":/buttons/stop_bt.png", this);

    homeBt->setPos(0,0);
    startBt->setPos(110+5,0);
    stopBt->setPos(110+5,-60);

    connect(homeBt, SIGNAL(activated()), this, SIGNAL(home_bt_clicked()));
    connect(startBt, SIGNAL(activated()), this, SIGNAL(startSearch_bt_clicked()));
    connect(stopBt, SIGNAL(activated()), this, SIGNAL(stopSearch_bt_clicked()));

    slidingDownBts = new GraphicSlidingDownBts(this);
    slidingDownBts->setPos(110+5+110+5,0);
    slidingDownBts->setBtsRect(QRectF(0,0,110,54));

    for(int i=1; i < 5;i++) {
        slidingDownBts->addBt(QString("KIT %1").arg(i), QString::number(i));
    }

    slidingDownBts->addBt(QString("SIMULATION"), QString("-1"));

    slidingDownBts->setStartBt(0);
    slidingDownBts->construction_finished();
    connect(slidingDownBts, SIGNAL(bt_selected(QString)), this, SLOT(kit_selected(QString)));

    setupStateMachine();
}

GraphicsConnMenu::~GraphicsConnMenu()
{
    rootState->deleteLater();
}

QRectF GraphicsConnMenu::boundingRect() const
{
    return QRectF();
}

void GraphicsConnMenu::paint(QPainter *painter, const QStyleOptionGraphicsItem * option,QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}



void GraphicsConnMenu::setupStateMachine()
{
    rootState = new QState(QState::ParallelStates);

    btState = new QState(rootState);
        btSearching = new QState(btState);
        btStandBy = new QState(btState);
        btState->setInitialState(btStandBy);
        btStandBy->addTransition(startBt, SIGNAL(activated()), btSearching);
        btSearching->addTransition(stopBt, SIGNAL(activated()), btStandBy);
        btSearching->assignProperty(startBt, "y", -60);
        btSearching->assignProperty(stopBt, "y", 0);
        btStandBy->assignProperty(startBt, "y", 0);
        btStandBy->assignProperty(stopBt, "y", -60);


    machine.setGlobalRestorePolicy(QStateMachine::RestoreProperties);//unusable with Parallel Machines
    machine.addState(rootState);
    machine.setInitialState(rootState);


    machine.addDefaultAnimation(new PushBurtonAnimation(startBt, "y", startBt));
    machine.addDefaultAnimation(new PushBurtonAnimation(stopBt, "y", stopBt));

    machine.start();
}

void GraphicsConnMenu::kit_selected(QString selKit)
{
    int kitN = selKit.toInt();
    emit kit_selected(kitN);
    qDebug() << "Kit " << kitN << " selected";
}

