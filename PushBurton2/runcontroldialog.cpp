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

#include "runcontroldialog.h"

RunControlDialog::RunControlDialog(QObject* parent=0) :
        nokiaFont("Nokia Standard Bold", 13)
{
    this->setParent(parent);
    this->setZValue(0.2);

    homeBt = new GraphicPixmapBt(":/buttons/home_bt.png", this);
    homeBt->setPos(0,0);

    startRunBt = new GraphicPixmapBt(":/buttons/run_start_bt.png", this);
    startRunBt->setPos(170-49,(382-118-20)+6+54);
    startRunBt->setVisible(false);

    stopRunBt = new GraphicPixmapBt(":/buttons/run_stop_bt.png", this);
    stopRunBt->setPos(170-49,(382-118-20)+6+54);
    stopRunBt->setVisible(false);

    connect(homeBt, SIGNAL(activated()), this, SIGNAL(home_bt_clicked()));

    init_state_machine();

    runStart = 0;
    run_active = false;

    timerId = startTimer(1000.0/15.0);
}

void RunControlDialog::init_state_machine()
{
    rootState = new QState();
    standBy = new QState(rootState);
    logging = new QState(rootState);

    rootState->setInitialState(standBy);

    standBy->assignProperty(startRunBt, "visible", true);
    logging->assignProperty(stopRunBt, "visible", true);

    standBy->addTransition(startRunBt, SIGNAL(released()), logging);
    connect(logging, SIGNAL(entered()), this, SLOT(logging_start()));
    connect(logging, SIGNAL(entered()), this, SIGNAL(start_run()));
    connect(logging, SIGNAL(exited()), this, SLOT(logging_end()));
    connect(logging, SIGNAL(exited()), this, SIGNAL(stop_run()));

    logging->addTransition(stopRunBt, SIGNAL(released()), standBy);

    machine.setGlobalRestorePolicy(QStateMachine::RestoreProperties);
    machine.addState(rootState);
    machine.setInitialState(rootState);

    machine.start();
}

RunControlDialog::~RunControlDialog()
{
    rootState->deleteLater();
}


QRectF RunControlDialog::boundingRect() const
{
    return QRectF(0,0,340,382+6+54);
}

void RunControlDialog::paint(QPainter *painter, const QStyleOptionGraphicsItem * option,QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
//    painter->drawEllipse(QRectF(225, 216, 15, 15));
    painter->setBrush(Qt::white);
    painter->drawRect(QRect(0,60,340,382));

    painter->setBrush(QColor(2,186,255));
    painter->drawRect(QRect(5,65,330,120));

    painter->setBrush(QColor(130,222,63));
    painter->drawRect(QRect(5,65+118+5,330,90));

    uint timeNow = QDateTime::currentDateTime().toUTC().toTime_t();
    QDateTime elapsedTime = QDateTime::fromTime_t(timeNow - runStart);


    //fetch the timestamp WITH msecs
    uint msecs;
    double dmsecs;
    msecs = QTime::currentTime().msec();
    uint aft = QDateTime::currentDateTime().toUTC().toTime_t();

    //ugly work around, will be fized with msecsToEpoc from Qt 4.7
    if(aft != timeNow)//on a second slip
    {
        msecs = 0;
        timeNow = aft;
    }

    dmsecs = msecs*0.001;


    //Draw unix time
    painter->setPen(Qt::white);
    painter->setFont(QFont("Nokia Standard Bold", 20));
    painter->drawText(QRect(5,65,330,100), Qt::AlignCenter, QString("%1%2")
                      .arg(QString::number(timeNow).right(7))
                      .arg(QString::number(dmsecs, 'f', 3).right(4)));
    painter->setFont(QFont("Nokia Standard Bold", 6));
    painter->drawText(QRect(5,155,330,20), Qt::AlignCenter, "UNIX TIME");

//    painter->setFont(QFont("Nokia Standard Bold", 12));
//    painter->drawText(QRect(240,135,90,50), Qt::AlignRight, QString::number(dmsecs, 'f', 3).right(4));

    //Draw run duration
    painter->setPen(Qt::white);
    painter->setFont(QFont("Nokia Standard Bold", 18));
    painter->drawText(QRect(5,65+118+5,330,70), Qt::AlignCenter, (run_active)?elapsedTime.toString("mm:ss"):"--:--");
    painter->setFont(QFont("Nokia Standard Bold", 6));
    painter->drawText(QRect(5,65+118+5+60,330,20), Qt::AlignCenter, "RUN DURATION");
}

void RunControlDialog::logging_start()
{
    runStart = QDateTime::currentDateTime().toUTC().toTime_t();
    run_active = true;
}

void RunControlDialog::logging_end()
{
    run_active = false;
}

void RunControlDialog::timerEvent(QTimerEvent * event)
{
    if(event->timerId() == timerId)
    {
        this->update();
    }
}
