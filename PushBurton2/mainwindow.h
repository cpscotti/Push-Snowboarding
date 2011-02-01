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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMenuBar>
#include <QAction>
#include <QPixmap>
#include <QDesktopWidget>
#include <QDateTime>

#include <QGraphicsScene>
#include "pushburtonanimation.h"

#include <QTimer>

#include <QDebug>

#include "FilesystemConventions.h"

#include "graphicpixmapbt.h"
#include "graphicsdeviceinfo.h"
#include "graphicsconnmenu.h"
#include "runcontroldialog.h"//should rename to graphicsrundialog
#include "graphicsreportview.h"
#include "graphicssettings.h"

#include <QStateMachine>

#include <QFontDatabase>


//PushBurton Control modules
#include "devicesmanager.h"
#include "pushdevicesholder.h"
#include "runmanager.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //Control modules
    PushDevicesHolder devicesHolder;
    DevicesManager devicesManager;
    RunManager runManager;


    //GUI States
    QState * rootState;
    //Unfolds into
        QState * bootState;
        QState * userState;
        //Unfolds into
            QState * homeUsrState;
            QState * connectUsrState;
            QState * runUsrState;
            QState * reportUsrState;
            QState * settingsUsrState;
        QState * leavingState;

    QStateMachine machine;

    GraphicsDeviceInfo * devInfo;
    GraphicsConnMenu * connMenu;
    RunControlDialog * runDialog;
    GraphicsReportView * reportView;
    GraphicsSettings * settingsView;

    GraphicPixmapBt * splashScreen;

private:
    Ui::MainWindow *ui;
    QGraphicsScene * scene;

    void fillStatesProperties();
    void fillMainMenuProperties();
    void startStateMachine();

    QTimer * sshotTimer;

public slots:
    void drawMainScreen();

    void scheduleSshot();
    void takeScreenShot();
};

#endif // MAINWINDOW_H
