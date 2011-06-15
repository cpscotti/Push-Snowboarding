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

#include "graphicsreportview.h"

GraphicsReportView::GraphicsReportView(QObject *parent=0) :
        speedPlot(this, 0),
        heartPlot(this, 1),
        airTimePlot(this, 2),
        rushPlot(this, 3),
        altitudePlot(this, 4)

{
    this->setParent(parent);
    this->setZValue(0.2);

//    deleteBt = new GraphicPixmapBt(":/buttons/home_bt.png", this);
    deleteBt = new GraphicTextBt("Delete", QRectF(0,0,110, 54) ,this);
    deleteBt->setPos(0,0);
    connect(deleteBt, SIGNAL(released()), this, SLOT(ask_for_delete_confirmation()));

    speedLink = new GraphicPixmapBt(":/buttons/r_speedOff.png", this);
    speedLink->setAltImage(QString(":/buttons/r_speedOn.png"));
    speedLink->setPos(10+0*8+0*58,62);
    heartLink = new GraphicPixmapBt(":/buttons/r_heartOff.png", this);
    heartLink->setAltImage(QString(":/buttons/r_heartOn.png"));
    heartLink->setPos(10+1*8+1*58,62);
    airTimeLink = new GraphicPixmapBt(":/buttons/r_airTimeOff.png", this);
    airTimeLink->setAltImage(QString(":/buttons/r_airTimeOn.png"));
    airTimeLink->setPos(10+2*8+2*58,62);
    rushLink = new GraphicPixmapBt(":/buttons/r_rushOff.png", this);
    rushLink->setAltImage(QString(":/buttons/r_rushOn.png"));
    rushLink->setPos(10+3*8+3*58,62);
    altitudeLink = new GraphicPixmapBt(":/buttons/r_altitudeOff.png", this);
    altitudeLink->setAltImage(QString(":/buttons/r_altitudeOn.png"));
    altitudeLink->setPos(10+4*8+4*58,62);

    speedPlot.setY(124);
    speedPlot.setColor(QColor(2,186,255));
    speedPlot.setPlotTitle("SPEED (MPH)");
    connect(&speedPlot, SIGNAL(hor_rel_drag(qreal)), this, SIGNAL(hor_rel_drag(qreal)));
    connect(this, SIGNAL(hor_rel_drag(qreal)), &speedPlot, SLOT(hor_rel_slide(qreal)));

    heartPlot.setY(124);
    heartPlot.setColor(QColor(163,30,83));
    heartPlot.setPlotTitle("HEART (BPM)");
    heartPlot.addStatBox("50", "AVG");
    heartPlot.addStatBox("100", "MAX");
    connect(&heartPlot, SIGNAL(hor_rel_drag(qreal)), this, SIGNAL(hor_rel_drag(qreal)));
    connect(this, SIGNAL(hor_rel_drag(qreal)), &heartPlot, SLOT(hor_rel_slide(qreal)));
    airTimePlot.setY(124);
    airTimePlot.setColor(QColor(236,92,55));
    airTimePlot.setPlotTitle("AIR TIME (MS)");
    connect(&airTimePlot, SIGNAL(hor_rel_drag(qreal)), this, SIGNAL(hor_rel_drag(qreal)));
    connect(this, SIGNAL(hor_rel_drag(qreal)), &airTimePlot, SLOT(hor_rel_slide(qreal)));
    rushPlot.setY(124);
    rushPlot.setColor(QColor(239,200,37));
    rushPlot.setPlotTitle("RUSH");
    connect(&rushPlot, SIGNAL(hor_rel_drag(qreal)), this, SIGNAL(hor_rel_drag(qreal)));
    connect(this, SIGNAL(hor_rel_drag(qreal)), &rushPlot, SLOT(hor_rel_slide(qreal)));
    altitudePlot.setY(124);
    altitudePlot.setColor(QColor(106,179,71));
    altitudePlot.setPlotTitle("ALTITUDE (M)");
    connect(&altitudePlot, SIGNAL(hor_rel_drag(qreal)), this, SIGNAL(hor_rel_drag(qreal)));
    connect(this, SIGNAL(hor_rel_drag(qreal)), &altitudePlot, SLOT(hor_rel_slide(qreal)));

    init_state_machine();

    slidingDownBts = 0;


    refresh_dirs_graphs();


//    slidingDownBts = new GraphicTextBt(dirName, this);

//    selectedPlot = 0;
}

void GraphicsReportView::refresh_dirs_graphs()
{
    if(slidingDownBts)
        slidingDownBts->deleteLater();

    slidingDownBts = new GraphicSlidingDownBts(this);
    slidingDownBts->setPos(110+5,0);
    slidingDownBts->setBtsRect(QRectF(0,0,110+5+110,54));

    QString dirName;
    load_avDirectories_directories();
    for(int i=0;i<avDirectories.count();i++)
    {

        dirName = FSC_RUNS_FOLDERS_ROOT;

        if(avDirectories[i].length() == 16) // old format
        {
            dirName.append(avDirectories[i]);
            slidingDownBts->addBt(name_from_old_format(avDirectories[i]), dirName);
        } else {
            dirName.append(avDirectories[i]);
            slidingDownBts->addBt(name_from_new_format(avDirectories[i]), dirName);
        }
    }
    slidingDownBts->construction_finished();
    connect(slidingDownBts, SIGNAL(bt_selected(QString)), this, SLOT(file_selected(QString)));
}

GraphicsReportView::~GraphicsReportView()
{
    rootState->deleteLater();
}

QRectF GraphicsReportView::boundingRect() const
{
    return QRectF(0,0,340,382+6+54);
}

void GraphicsReportView::paint(QPainter *painter, const QStyleOptionGraphicsItem * option,QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(Qt::white);
    painter->drawRect(QRect(0,60,340,382));
}

void GraphicsReportView::init_state_machine()
{
    rootState = new QState();
    speedViewState = new QState(rootState);
    airTimeViewState = new QState(rootState);
    heartViewState = new QState(rootState);
    rushViewState = new QState(rootState);
    altitudeViewState = new QState(rootState);
    rootState->setInitialState(speedViewState);

    rootState->addTransition(speedLink, SIGNAL(released()), speedViewState);
    rootState->addTransition(&speedPlot, SIGNAL(swipeLeft()), heartViewState);
    rootState->addTransition(&speedPlot, SIGNAL(swipeRigth()), speedViewState);
    rootState->addTransition(&speedPlot, SIGNAL(bounceBack()), speedViewState);

    rootState->addTransition(heartLink, SIGNAL(released()), heartViewState);
    rootState->addTransition(&heartPlot, SIGNAL(swipeLeft()), airTimeViewState);
    rootState->addTransition(&heartPlot, SIGNAL(swipeRigth()), speedViewState);
    rootState->addTransition(&heartPlot, SIGNAL(bounceBack()), heartViewState);

    rootState->addTransition(airTimeLink, SIGNAL(released()), airTimeViewState);
    rootState->addTransition(&airTimePlot, SIGNAL(swipeLeft()), rushViewState);
    rootState->addTransition(&airTimePlot, SIGNAL(swipeRigth()), heartViewState);
    rootState->addTransition(&airTimePlot, SIGNAL(bounceBack()), airTimeViewState);

    rootState->addTransition(rushLink, SIGNAL(released()), rushViewState);
    rootState->addTransition(&rushPlot, SIGNAL(swipeLeft()), altitudeViewState);
    rootState->addTransition(&rushPlot, SIGNAL(swipeRigth()), airTimeViewState);
    rootState->addTransition(&rushPlot, SIGNAL(bounceBack()), rushViewState);

    rootState->addTransition(altitudeLink, SIGNAL(released()), altitudeViewState);
    rootState->addTransition(&altitudePlot, SIGNAL(swipeRigth()), rushViewState);
    rootState->addTransition(&altitudePlot, SIGNAL(swipeLeft()), altitudeViewState);
    rootState->addTransition(&altitudePlot, SIGNAL(bounceBack()), altitudeViewState);


    setup_link_alt_bts(speedViewState, speedLink);
    setup_link_alt_bts(heartViewState, heartLink);
    setup_link_alt_bts(airTimeViewState, airTimeLink);
    setup_link_alt_bts(rushViewState, rushLink);
    setup_link_alt_bts(altitudeViewState, altitudeLink);

    setup_rolling_plot_properties(speedViewState, 0);
    setup_rolling_plot_properties(heartViewState, 1);
    setup_rolling_plot_properties(airTimeViewState, 2);
    setup_rolling_plot_properties(rushViewState, 3);
    setup_rolling_plot_properties(altitudeViewState, 4);

    machine.setGlobalRestorePolicy(QStateMachine::RestoreProperties);
    machine.addState(rootState);
    machine.setInitialState(rootState);

    machine.addDefaultAnimation(new PushBurtonAnimation(&speedPlot, "x", this));
    machine.addDefaultAnimation(new PushBurtonAnimation(&heartPlot, "x", this));
    machine.addDefaultAnimation(new PushBurtonAnimation(&airTimePlot, "x", this));
    machine.addDefaultAnimation(new PushBurtonAnimation(&rushPlot, "x", this));
    machine.addDefaultAnimation(new PushBurtonAnimation(&altitudePlot, "x", this));

    machine.start();
}

void GraphicsReportView::setup_rolling_plot_properties(QState * state, int idx)
{
    state->assignProperty(&speedPlot, "x", get_plot_pos(speedPlot.plotIdx, idx));
    state->assignProperty(&heartPlot, "x", get_plot_pos(heartPlot.plotIdx, idx));
    state->assignProperty(&airTimePlot, "x", get_plot_pos(airTimePlot.plotIdx, idx));
    state->assignProperty(&rushPlot, "x", get_plot_pos(rushPlot.plotIdx, idx));
    state->assignProperty(&altitudePlot, "x", get_plot_pos(altitudePlot.plotIdx, idx));
}

int GraphicsReportView::get_plot_pos(int pid, int sel)
{
    return (pid-sel)*(340+20);
}


void GraphicsReportView::load_graphs(QString dir)
{
    qDebug() << "loading: " << dir;
    speedReport.load_from_dir(dir);
    speedPlot.setValTransform(3.6/1.6);
    speedPlot.setVerticalRange(0.0, speedReport.Speed_max*1.1);
    speedPlot.setGraphPoints(&(speedReport.graphPoints));
    speedPlot.clearStatBoxes();
    if(speedReport.graphPoints.count() > 0) {
        speedPlot.addStatBox(QString::number(speedReport.Speed_max*2.25, 'f',1), "MAX");
        speedPlot.addStatBox(QString::number(speedReport.Speed_avg*2.25, 'f',1), "AVERAGE");
    }

    heartReport.load_from_dir(dir);
    heartPlot.setVerticalRange(50.0, (float)heartReport.Heart_max*1.1);
    heartPlot.setGraphPoints(&(heartReport.graphPoints));
    heartPlot.clearStatBoxes();
    if(heartReport.graphPoints.count() > 0) {
        heartPlot.addStatBox(QString::number(heartReport.Heart_max), "MAX");
        heartPlot.addStatBox(QString::number(heartReport.Heart_avg), "AVERAGE");
    }

    airTimeReport.load_from_dir(dir);
    airTimePlot.setPlotType(GraphicVarPlot::Bars);
    airTimePlot.setVerticalRange(0.0, airTimeReport.longest_air_time*1.1);
    airTimePlot.setHorizontalRange(airTimeReport.tRunStart, airTimeReport.tRunEnd);
    airTimePlot.setGraphPoints(&(airTimeReport.tstamps),&(airTimeReport.air_times));
    airTimePlot.clearStatBoxes();
    if(airTimeReport.tstamps.count() > 0) {
        airTimePlot.addStatBox(QString::number(airTimeReport.longest_air_time, 'f', 1), "MAX");
        airTimePlot.addStatBox(QString::number(airTimeReport.total_air_time, 'f', 1), "TOTAL");
    }

    rushReport.load_from_dir(dir);
    rushPlot.setVerticalRange(0.2*(float)rushReport.GSR_avg, (float)rushReport.GSR_max*1.1);
    rushPlot.setGraphPoints(&(rushReport.graphPoints));
    rushPlot.clearStatBoxes();
    if(rushReport.graphPoints.count()> 0) {
        rushPlot.addStatBox(QString::number(rushReport.GSR_max), "MAX");
        rushPlot.addStatBox(QString::number(rushReport.GSR_avg), "AVERAGE");
    }

    altitudeReport.load_from_dir(dir);
    if(altitudeReport.Altitude_max > altitudeReport.Altitude_min) {
        altitudePlot.setVerticalRange(altitudeReport.Altitude_min, altitudeReport.Altitude_max);
    } else {
        altitudePlot.setVerticalRange(0.0, 10000.0);
    }
    altitudePlot.setGraphPoints(&(altitudeReport.graphPoints));
    altitudePlot.clearStatBoxes();
    if(altitudeReport.graphPoints.count() > 0) {
        altitudePlot.addStatBox(QString::number(altitudeReport.Altitude_max, 'f', 0), "MAX");
        altitudePlot.addStatBox(QString::number(altitudeReport.Altitude_max-altitudeReport.Altitude_min, 'f', 0), "SPAN");
        altitudePlot.addStatBox(QString::number(altitudeReport.Altitude_min, 'f', 0), "MIN");
    }
}

void GraphicsReportView::load_avDirectories_directories()
{
    QDir dirIf(FSC_RUNS_FOLDERS_ROOT);

    QStringList fileFilter;
    fileFilter << FSC_RUNS_FILTER;

    dirIf.setNameFilters(fileFilter);

    avDirectories = dirIf.entryList();
    for(int i=0;i<avDirectories.count();i++) {

        qDebug() << "Found dir: " << avDirectories[i];
    }
}

void GraphicsReportView::delete_run_dir(const QString& dir)
{
    QDir runDir(dir);

    QStringList filesInDir;
    filesInDir = runDir.entryList();
    foreach(QString fileEntry, filesInDir)
    {
        runDir.remove(fileEntry);
    }

    QDir root(FSC_RUNS_FOLDERS_ROOT);
    root.rmdir(dir);
}

void GraphicsReportView::delete_current_dir()
{
    qDebug() << "Removing: " << currentDir;
    delete_run_dir(currentDir);
    refresh_dirs_graphs();
}

void GraphicsReportView::file_selected(QString dir)
{
    qDebug() << "Selected directory: " << dir;
    currentDir = dir;
    load_graphs(currentDir);
}

QString GraphicsReportView::name_from_old_format(const QString& inStr)
{
    uint utime = inStr.right(10).toUInt();
    return QDateTime::fromTime_t(utime).toString("hh:mm, ddd, d/M/yy");
}

QString GraphicsReportView::name_from_new_format(const QString& inStr)
{
    uint utime = inStr.left(16).right(10).toUInt();
    return QDateTime::fromTime_t(utime).toString("hh:mm, ddd, d/M/yy");
}

void GraphicsReportView::setup_link_alt_bts(QState * state, QObject * bt)
{

    state->assignProperty(speedLink, "altImgToggle", (bt==speedLink));
    state->assignProperty(heartLink, "altImgToggle", (bt==heartLink));
    state->assignProperty(airTimeLink, "altImgToggle", (bt==airTimeLink));
    state->assignProperty(rushLink, "altImgToggle", (bt==rushLink));
    state->assignProperty(altitudeLink, "altImgToggle", (bt==altitudeLink));
}

void GraphicsReportView::ask_for_delete_confirmation()
{
    QMessageBox msgBox;
    msgBox.setText("Are you sure you want to delete this run?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    if(ret == QMessageBox::Yes) {
        this->delete_current_dir();
    }

}

void GraphicsReportView::deletion_accepted()
{
    this->delete_current_dir();
}

void GraphicsReportView::deletion_rejected()
{
}
