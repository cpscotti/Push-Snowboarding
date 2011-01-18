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

#include "graphicvarplot.h"

#include "npushreportspeed.h"
#include "npushreportheart.h"
#include "npushreportgsr.h"
#include "npushreportaltitude.h"
#include "npushreportairtime.h"

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


    GraphicPixmapBt * homeBt;
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

    void init_state_machine();

    void load_graphs(QString dir);
    void load_avDirectories_directories();
    QStringList avDirectories;

    void setup_rolling_plot_properties(QState *,int);
    int get_plot_pos(int pid, int sel);

    QString run_name_from_utime(const QString&);

signals:
    void home_bt_clicked();
    void hor_rel_drag(qreal ydrag);

public slots:
    void file_selected(QString dir);
    void refresh_dirs_graphs();
};

#endif // GRAPHICSREPORTVIEW_H
