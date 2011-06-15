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

#ifndef LIVEVIEW_H
#define LIVEVIEW_H

#include <QMainWindow>

#include <QVector>
#include <QListWidget>
#include <QListWidgetItem>

#include "pushdevicesholder.h"
#include "pushburtongenericdevice.h"
#include "npushlogtick.h"

namespace Ui {
    class LiveView;
}

class LiveView : public QMainWindow
{
    Q_OBJECT

public:
    explicit LiveView(PushDevicesHolder * a_configuredDevices, QWidget *parent = 0);
    ~LiveView();

private:

    QList<QListWidgetItem*> listedDevices;

    void UpdateConnectedList();
    void UpdateStreamingConnections();

//    QVector<PushBurtonGenericDevice *> * configuredDevices;
    PushDevicesHolder * configuredDevices;

    QVector<bool> streamingFilter;

    Ui::LiveView *ui;
    bool streaming;
    int liveOutputCount;

signals:
    void leaving();

private slots:
    void on_connectedDevicesList_itemSelectionChanged();
    void on_exitBt_clicked();
    void on_liveControl_clicked();

    void live_output(NPushLogTick *);
};

#endif // LIVEVIEW_H
