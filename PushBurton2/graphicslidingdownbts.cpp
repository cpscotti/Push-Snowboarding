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

#include "graphicslidingdownbts.h"

GraphicSlidingDownBts::GraphicSlidingDownBts(QGraphicsItem* parent) : QGraphicsObject(parent)
{

    this->setZValue(0.8);
    selectedMapper = new QSignalMapper(this);
    connect(selectedMapper, SIGNAL(mapped(const QString&)), this, SIGNAL(bt_selected(const QString&)));
    connect(selectedMapper, SIGNAL(mapped(const QString&)), this, SLOT(inn_selected(const QString&)));

    rootState = new QState();
    chooserState = new QState(rootState);

//    machine.setGlobalRestorePolicy(QStateMachine::RestoreProperties);
    machine.addState(rootState);
    machine.setInitialState(rootState);
    rootState->setInitialState(chooserState);
}

void GraphicSlidingDownBts::addBt(QString text, QString value)
{
    GraphicTextBt * newBt = new GraphicTextBt(text, this);
    newBt->setPos(0,0);
    newBt->setZValue(1.0);
    push_back(newBt, value);
}

void GraphicSlidingDownBts::push_back(GraphicTextBt* newBt, QString val)
{
    QState * newState = new QState(rootState);
    selectedStates.push_back(newState);
    graphicBts.push_back(newBt);

    newState->addTransition(newBt, SIGNAL(activated()), chooserState);
//    newState->assignProperty(newBt, "y", 0);

    connect(newState, SIGNAL(entered()), selectedMapper, SLOT(map()));
    selectedMapper->setMapping(newState, val);

    machine.addDefaultAnimation(new QPropertyAnimation(newBt, "y", this));

    rootState->setInitialState(newState);
}

void GraphicSlidingDownBts::construction_finished()
{
    int cnt = graphicBts.count();
    Q_ASSERT(cnt == selectedStates.count());

    for(int i=0;i < cnt;i++)//for each state
    {
        for(int j=0;j<cnt;j++)//each bt has its position
        {
            if(i!=j){
                selectedStates[i]->assignProperty(graphicBts[j], "y", -60*(cnt-j));
            } else {
                selectedStates[i]->assignProperty(graphicBts[j], "y", 0);
            }
        }
    }

    for(int i=0;i < cnt;i++)
    {
        chooserState->assignProperty(graphicBts[i], "y", 60*i);//chooser position for all bts (unfolded down)
        chooserState->addTransition(graphicBts[i], SIGNAL(activated()), selectedStates[i]);
    }

    machine.start();
    this->update();
}

GraphicSlidingDownBts::~GraphicSlidingDownBts()
{
    if(rootState)
        rootState->deleteLater();
}

QRectF GraphicSlidingDownBts::boundingRect() const
{
    return QRectF();
}
void GraphicSlidingDownBts::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *)
{
    Q_UNUSED(painter);
}

void GraphicSlidingDownBts::inn_selected(const QString& val)
{
    qDebug() << "Inner selection is " << val;
}
