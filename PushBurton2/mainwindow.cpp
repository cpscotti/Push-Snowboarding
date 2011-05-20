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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWidget>

MainWindow::MainWindow(const QString& simulationData, QWidget *parent) :
    QMainWindow(parent),
    devicesManager(&devicesHolder,parent),
    runManager(&devicesHolder,parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    this->setWindowTitle("Push Snowboarding");
    QWidget::setWindowFlags(Qt::WindowSoftkeysVisibleHint);

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 360, 480);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    ui->graphicsView->setScene(scene);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(215, 215, 215)));
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->graphicsView->setWindowTitle("Push Snowboarding");
    ui->graphicsView->showMaximized();
    ui->graphicsView->show();

    //Building state machine hierarchy (no connections)

    rootState = new QState();
    //Unfolds into
        bootState = new QState(rootState);
        userState = new QState(rootState);
        //Unfolds into
            homeUsrState = new QState(userState);
            connectUsrState = new QState(userState);
            runUsrState = new QState(userState);
            reportUsrState = new QState(userState);
            settingsUsrState = new QState(userState);
            userState->setInitialState(homeUsrState);
        leavingState = new QState(rootState);
        rootState->setInitialState(bootState);

    fillStatesProperties();
    fillMainMenuProperties();

    startStateMachine();

    QMenuBar * menuBar = new QMenuBar(this);


    QAction * takeScreenshotAc = new QAction("Screenshot", menuBar);
    takeScreenshotAc->setSoftKeyRole(QAction::SelectSoftKey);
    connect(takeScreenshotAc, SIGNAL(triggered()), this, SLOT(scheduleSshot()));

//    connect(homeUsrState, SIGNAL(entered()), this, SLOT(scheduleSshot()));
    menuBar->addAction(takeScreenshotAc);
    setMenuBar(menuBar);

    if(simulationData != "") {
        //If simulation file was passed bo main window (via command line arg, for example)

        connect(&devicesManager, SIGNAL(request_run_start()), &runManager, SLOT(start_logging()));
        connect(&devicesManager, SIGNAL(request_run_end()), &runManager, SLOT(stop_logging()));

        connect(&runManager, SIGNAL(dataSaved()), this, SLOT(close()));

        devicesManager.switch_to_simulation_device(simulationData);
    }
}

void MainWindow::fillStatesProperties()
{
    //Filling in state machine (each state configuration)

    //Devices infographic (snowboarder)
    devInfo = new GraphicsDeviceInfo(this);
    devInfo->setZValue(0.1);
    devInfo->setPos(10,-390);
    scene->addItem(devInfo);
    homeUsrState->assignProperty(devInfo, "y", 5);
    connectUsrState->assignProperty(devInfo, "y", 65);
    machine.addDefaultAnimation(new PushBurtonAnimation(devInfo, "y", devInfo));

    connect(&devicesManager, SIGNAL(device_connecting(QString)), devInfo, SLOT(device_connecting(QString)));
    connect(&devicesManager, SIGNAL(device_connected(QString)), devInfo, SLOT(device_connected(QString)));
    connect(&devicesManager, SIGNAL(device_disconnected(QString)), devInfo, SLOT(device_disconnected(QString)));

    homeUsrState->addTransition(devInfo, SIGNAL(activated()), connectUsrState);

    //Splash Screen
    splashScreen = new GraphicPixmapBt(":/images/push_splash.png");
    splashScreen->set_clickable(false);
    splashScreen->setPos(11,5);
    scene->addItem(splashScreen);
    rootState->assignProperty(splashScreen, "x", 11);//filling screen
    userState->assignProperty(splashScreen, "x", -360);
    leavingState->assignProperty(splashScreen, "x", 11);
    machine.addDefaultAnimation(new PushBurtonAnimation(splashScreen, "x", splashScreen));

    //devices connection interface (search, stop, kit selection)
    connMenu = new GraphicsConnMenu(this);
    connect(connMenu, SIGNAL(startSearch_bt_clicked()), &devicesManager, SLOT(start_bt_search()));
    connect(connMenu, SIGNAL(stopSearch_bt_clicked()), &devicesManager, SLOT(stop_bt_search()));
    connect(connMenu, SIGNAL(kit_selected(int)), &devicesManager, SLOT(kit_selected(int)));
    connMenu->setPos(10, -180);
    scene->addItem(connMenu);
    connectUsrState->assignProperty(connMenu, "y", 5);
    machine.addDefaultAnimation(new PushBurtonAnimation(connMenu, "y", connMenu));
    connectUsrState->addTransition(connMenu, SIGNAL(home_bt_clicked()), homeUsrState);

    //Run control dialog.. start_log, stop_log...
    runDialog = new RunControlDialog(this);
    connect(runDialog, SIGNAL(start_run()), &runManager, SLOT(start_logging()));
    connect(runDialog, SIGNAL(stop_run()), &runManager, SLOT(stop_logging()));
    runDialog->setPos(10, -450);
    scene->addItem(runDialog);
    runUsrState->assignProperty(runDialog, "y", 5);
    machine.addDefaultAnimation(new PushBurtonAnimation(runDialog, "y", runDialog));
    runUsrState->addTransition(runDialog, SIGNAL(home_bt_clicked()), homeUsrState);

    //Report screen
    reportView = new GraphicsReportView(this);
    reportView->setPos(10, -450);
    scene->addItem(reportView);
    reportUsrState->assignProperty(reportView,"y", 5);
    machine.addDefaultAnimation(new PushBurtonAnimation(reportView, "y", reportView));
    reportUsrState->addTransition(reportView, SIGNAL(home_bt_clicked()), homeUsrState);
    connect(runDialog, SIGNAL(stop_run()), reportView, SLOT(refresh_dirs_graphs()));

    //Settings screen
    settingsView = new GraphicsSettings(&devicesHolder, this);
    settingsView->setPos(10,-450);
    scene->addItem(settingsView);
    settingsView->initForm();
    settingsUsrState->assignProperty(settingsView,"y", 5);
    machine.addDefaultAnimation(new PushBurtonAnimation(settingsView, "y", settingsView));
    settingsUsrState->addTransition(settingsView, SIGNAL(home_bt_clicked()), homeUsrState);

    //this should be someplace else
    QTimer * timer = new QTimer(this);
    timer->start(2000);
    timer->setSingleShot(true);
    bootState->addTransition(timer, SIGNAL(timeout()), userState);
}

void MainWindow::fillMainMenuProperties()
{
    GraphicPixmapBt * cnbt = new GraphicPixmapBt(":/buttons/connect_bt.png");
    GraphicPixmapBt * runbt = new GraphicPixmapBt(":/buttons/run_logger_bt.png");
    GraphicPixmapBt * repbt = new GraphicPixmapBt(":/buttons/report_bt.png");
    GraphicPixmapBt * setbt = new GraphicPixmapBt(":/buttons/settings_bt.png");

    cnbt->setPos(10,500);
    runbt->setPos(10+82+4,600);
    repbt->setPos(10+82*2+4*2,700);
    setbt->setPos(10+82*3+4*3,800);

    scene->addItem(cnbt);
    scene->addItem(runbt);
    scene->addItem(repbt);
    scene->addItem(setbt);

    rootState->assignProperty(cnbt, "y", 500);
    rootState->assignProperty(runbt, "y", 600);
    rootState->assignProperty(repbt, "y", 700);
    rootState->assignProperty(setbt, "y", 800);

    userState->assignProperty(cnbt, "y", 393);
    userState->assignProperty(runbt, "y", 393);
    userState->assignProperty(repbt, "y", 393);
    userState->assignProperty(setbt, "y", 393);

    userState->addTransition(cnbt, SIGNAL(activated()), connectUsrState);
    userState->addTransition(runbt, SIGNAL(activated()), runUsrState);
    userState->addTransition(repbt, SIGNAL(activated()), reportUsrState);
    userState->addTransition(setbt, SIGNAL(activated()), settingsUsrState);

    machine.addDefaultAnimation(new PushBurtonAnimation(cnbt, "y", cnbt));
    machine.addDefaultAnimation(new PushBurtonAnimation(runbt, "y", runbt));
    machine.addDefaultAnimation(new PushBurtonAnimation(repbt, "y", repbt));
    machine.addDefaultAnimation(new PushBurtonAnimation(setbt, "y", setbt));
}

void MainWindow::startStateMachine()
{
    //Configuring & kickstarting state machine
    machine.setGlobalRestorePolicy(QStateMachine::RestoreProperties);
    machine.addState(rootState);
    machine.setInitialState(rootState);

    machine.start();
}

void MainWindow::drawMainScreen()
{

}

MainWindow::~MainWindow()
{
    rootState->deleteLater();
    delete ui;
}

void MainWindow::scheduleSshot()
{
    sshotTimer = new QTimer(this);
    sshotTimer->start(300);
    sshotTimer->setSingleShot(true);
    connect(sshotTimer, SIGNAL(timeout()), this, SLOT(takeScreenShot()));
}

void MainWindow::takeScreenShot()
{
    QPixmap sshot;
    sshot = QPixmap::grabWindow(QApplication::desktop()->winId());
    sshot.save(QString(FSC_RUNS_FOLDERS_ROOT) + QString("sshot%1.png").arg(QString::number(QDateTime::currentDateTime().toTime_t())), "PNG");

    sshotTimer->deleteLater();
}
