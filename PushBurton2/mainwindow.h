#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGraphicsScene>
#include "pushburtonanimation.h"

#include <QTimer>

#include <QDebug>

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

public slots:
    void drawMainScreen();
};

#endif // MAINWINDOW_H
