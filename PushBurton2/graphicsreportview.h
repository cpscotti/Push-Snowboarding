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

#ifndef GRAPHICSREPORTVIEW_H
#define GRAPHICSREPORTVIEW_H

#include <QDir>
#include <QStringList>
#include <QDateTime>

#include <QGraphicsObject>
#include <QStateMachine>
#include <QPropertyAnimation>
#include <QStringList>

#include "pushburtonanimation.h"

#include "graphicpixmapbt.h"
#include "graphictextbt.h"
#include "graphicslidingdownbts.h"

#include "graphicconfirmationrequest.h"

#include "graphicvarplot.h"

#include "npushreportspeed.h"
#include "npushreportheart.h"
#include "npushreportgsr.h"
#include "npushreportaltitude.h"
#include "npushreportairtime.h"

#include "FilesystemConventions.h"

#include <QDebug>

class GraphicsReportView : public QGraphicsObject
{
    Q_OBJECT
public:
    GraphicsReportView(QObject*);
    ~GraphicsReportView();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *);

private:
    QStateMachine machine;
    QState * rootState;
        QState * speedViewState;
        QState * airTimeViewState;
        QState * heartViewState;
        QState * rushViewState;
        QState * altitudeViewState;


    GraphicTextBt * deleteBt;

    GraphicPixmapBt * speedLink;
    GraphicPixmapBt * airTimeLink;
    GraphicPixmapBt * heartLink;
    GraphicPixmapBt * rushLink;
    GraphicPixmapBt * altitudeLink;

    GraphicVarPlot speedPlot;
    NPushReportSpeed speedReport;

    GraphicVarPlot heartPlot;
    NPushReportHeart heartReport;

    GraphicVarPlot airTimePlot;
    NPushReportAirTime airTimeReport;

    GraphicVarPlot rushPlot;
    NPushReportGSR rushReport;

    GraphicVarPlot altitudePlot;
    NPushReportAltitude altitudeReport;

    GraphicSlidingDownBts * slidingDownBts;

    GraphicConfirmationRequest * confirmationRequest;

    void init_state_machine();

    void load_graphs(QString dir);
    void load_avDirectories_directories();

    void delete_run_dir(const QString& dir);

    QStringList avDirectories;

    void setup_link_alt_bts(QState *, QObject *);
    void setup_rolling_plot_properties(QState *,int);
    int get_plot_pos(int pid, int sel);

    QString name_from_old_format(const QString&);
    QString name_from_new_format(const QString&);

    QString currentDir;

signals:
    void home_bt_clicked();
    void hor_rel_drag(qreal ydrag);

public slots:
    void file_selected(QString dir);
    void refresh_dirs_graphs();

    void ask_for_delete_confirmation();
    void deletion_accepted();
    void deletion_rejected();


    void delete_current_dir();
};

#endif // GRAPHICSREPORTVIEW_H
