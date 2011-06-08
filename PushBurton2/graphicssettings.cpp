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

    //Settings form:

    userNameField = 0;
    proxy2edit = 0;

    soundEffectsCheck = 0;
    proxy2soundEffects = 0;


    serverAddress = 0;
    proxy2Addr = 0;
    serverPort = 0;
    proxy2Port = 0;
    connectPushBt = 0;
    proxy2connect = 0;

    buildForm();

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

//    painter->setPen(QColor(17,83,161));

//    painter->setFont(QFont("Nokia Standard Bold", 6));

//    painter->drawText(QRect(0,60,340,382), Qt::AlignCenter, "Woohoo, nothing creepy here!\n We set it all up for you!");
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


void GraphicsSettings::update_soundEffects(bool checked)
{
    if(configuredDevices) {
        for(int i=0;i< configuredDevices->size();i++) {
            PushBurtonGenericDevice * dev = configuredDevices->at(i);
            if(typeid(*dev) == typeid(VirtualBrosDevice)) {
                ((VirtualBrosDevice*)dev)->setEnabled(checked);
                break;
            }
        }
    }
}

bool GraphicsSettings::get_soundEffects()
{
    if(configuredDevices) {
        for(int i=0;i< configuredDevices->size();i++) {
            PushBurtonGenericDevice * dev = configuredDevices->at(i);
            if(typeid(*dev) == typeid(VirtualBrosDevice)) {
                return ((VirtualBrosDevice*)dev)->getEnabled();
            }
        }
    }
    return false;
}


void GraphicsSettings::buildForm()
{
    QString widStyle("color: blue;"
                      "background-color: transparent;"
                      "border-color: blue;"
                      "border-style: solid;"
                      "border-width: 1px;");

    QRect widGeom(0,0,180, 30);

    //Username field
    userNameField = new QLineEdit("Username");
    userNameField->setStyleSheet(widStyle);
    proxy2edit = new QGraphicsProxyWidget(this);
    proxy2edit->setWidget(userNameField);
    userNameField->setGeometry(widGeom);
    proxy2edit->setVisible(true);
    proxy2edit->setZValue(2);
    proxy2edit->setPos(40, 120);



    //Sound Effects Field
    soundEffectsCheck = new QCheckBox("Sound Effects");
    soundEffectsCheck->setStyleSheet("color: blue;"
                                     "background-color: transparent;"
                                     "border-width: 0px;");
    proxy2soundEffects = new QGraphicsProxyWidget(this);
    proxy2soundEffects->setWidget(soundEffectsCheck);
    soundEffectsCheck->setGeometry(widGeom);
    proxy2soundEffects->setVisible(true);
    proxy2soundEffects->setZValue(2);
    proxy2soundEffects->setPos(40,160);
    soundEffectsCheck->setChecked(get_soundEffects());
    connect(soundEffectsCheck, SIGNAL(toggled(bool)), this, SLOT(update_soundEffects(bool)));

    //Address and Port line edits
    serverAddress = new QLineEdit("Address");
    serverAddress->setStyleSheet(widStyle);
    proxy2Addr = new QGraphicsProxyWidget(this);
    proxy2Addr->setWidget(serverAddress);
    serverAddress->setGeometry(0,0,150,30);
    proxy2Addr->setVisible(true);
    proxy2Addr->setZValue(2);
    proxy2Addr->setPos(40,200);

    serverPort = new QLineEdit("Port");
    serverPort->setStyleSheet(widStyle);
    proxy2Port = new QGraphicsProxyWidget(this);
    proxy2Port->setWidget(serverPort);
    serverPort->setGeometry(0,0,80, 30);
    proxy2Port->setVisible(true);
    proxy2Port->setZValue(2);
    proxy2Port->setPos(200, 200);


    connectPushBt = new QPushButton("Connect");
    connectPushBt->setStyleSheet(widStyle);
    proxy2connect = new QGraphicsProxyWidget(this);
    proxy2connect->setWidget(connectPushBt);
    connectPushBt->setGeometry(widGeom);
    proxy2connect->setVisible(true);
    proxy2connect->setZValue(2);
    proxy2connect->setPos(40,240);

    connect(connectPushBt, SIGNAL(released()), this, SLOT(connectBroadcaster()));


}

void GraphicsSettings::connectBroadcaster()
{
    if(configuredDevices) {
        for(int i=0;i< configuredDevices->size();i++) {
            PushBurtonGenericDevice * dev = configuredDevices->at(i);
            if(typeid(*dev) == typeid(BroadcasterDevice)) {
                BroadcasterDevice * bdev = (BroadcasterDevice*)dev;
                connect(bdev, SIGNAL(connectionUpdate(bool)), this, SLOT(updateConnectionState(bool)));

                bdev->setServerAddress(serverAddress->text());
                bdev->setServerPort((quint16)serverPort->text().toInt());

                return bdev->tryToConnect();
            }
        }
    }

}

void GraphicsSettings::updateConnectionState(bool connected)
{
    if(connectPushBt) {
        if(connected) {
            connectPushBt->setText("Disconnect");
            connectPushBt->setEnabled(false);
        } else {
            connectPushBt->setText("Connect");
        }
    }
}
