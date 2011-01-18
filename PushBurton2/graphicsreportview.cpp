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

    homeBt = new GraphicPixmapBt(":/buttons/home_bt.png", this);
    homeBt->setPos(0,0);
    connect(homeBt, SIGNAL(activated()), this, SIGNAL(home_bt_clicked()));

    qDebug() << "before creating reports bts";

    speedLink = new GraphicPixmapBt(":/buttons/r_speedOff.png", this);
    speedLink->setPos(10+0*8+0*58,62);
    heartLink = new GraphicPixmapBt(":/buttons/r_heartOff.png", this);
    heartLink->setPos(10+1*8+1*58,62);
    airTimeLink = new GraphicPixmapBt(":/buttons/r_airTimeOff.png", this);
    airTimeLink->setPos(10+2*8+2*58,62);
    rushLink = new GraphicPixmapBt(":/buttons/r_rushOff.png", this);
    rushLink->setPos(10+3*8+3*58,62);
    altitudeLink = new GraphicPixmapBt(":/buttons/r_altitudeOff.png", this);
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

    qDebug() << "before creating report fsm";

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

    QString dirName;
    load_avDirectories_directories();
    for(int i=0;i<avDirectories.count();i++)
    {
        dirName = "E:/";
        dirName.append(avDirectories[i]);
        slidingDownBts->addBt(run_name_from_utime(avDirectories[i]), dirName);
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
    speedPlot.addStatBox(QString::number(speedReport.Speed_max*2.25, 'f',1), "MAX");
    speedPlot.addStatBox(QString::number(speedReport.Speed_avg*2.25, 'f',1), "AVERAGE");

    heartReport.load_from_dir(dir);
    heartPlot.setVerticalRange(50.0, (float)heartReport.Heart_max*1.1);
    heartPlot.setGraphPoints(&(heartReport.graphPoints));
    heartPlot.clearStatBoxes();
    heartPlot.addStatBox(QString::number(heartReport.Heart_max), "MAX");
    heartPlot.addStatBox(QString::number(heartReport.Heart_avg), "AVERAGE");

    airTimeReport.load_from_dir(dir);
    airTimePlot.setPlotType(GraphicVarPlot::Bars);
    airTimePlot.setVerticalRange(0.0, airTimeReport.longest_air_time*1.1);
    airTimePlot.setHorizontalRange(airTimeReport.tRunStart, airTimeReport.tRunEnd);
    airTimePlot.setGraphPoints(&(airTimeReport.tstamps),&(airTimeReport.air_times));
    airTimePlot.clearStatBoxes();
    airTimePlot.addStatBox(QString::number(airTimeReport.longest_air_time, 'f', 1), "MAX");
    airTimePlot.addStatBox(QString::number(airTimeReport.total_air_time, 'f', 1), "TOTAL");

    rushReport.load_from_dir(dir);
    rushPlot.setVerticalRange(0.2*(float)rushReport.GSR_avg, (float)rushReport.GSR_max*1.1);
    rushPlot.setGraphPoints(&(rushReport.graphPoints));
    rushPlot.clearStatBoxes();
    rushPlot.addStatBox(QString::number(rushReport.GSR_max), "MAX");
    rushPlot.addStatBox(QString::number(rushReport.GSR_avg), "AVERAGE");

    altitudeReport.load_from_dir(dir);
    altitudePlot.setVerticalRange(altitudeReport.Altitude_min, altitudeReport.Altitude_max);
    altitudePlot.setGraphPoints(&(altitudeReport.graphPoints));
    altitudePlot.clearStatBoxes();
    altitudePlot.addStatBox(QString::number(altitudeReport.Altitude_max, 'f', 0), "MAX");
    altitudePlot.addStatBox(QString::number(altitudeReport.Altitude_max-altitudeReport.Altitude_min, 'f', 0), "SPAN");
    altitudePlot.addStatBox(QString::number(altitudeReport.Altitude_min, 'f', 0), "MIN");
}

void GraphicsReportView::load_avDirectories_directories()
{
    QDir dirIf("E:/");

    QStringList fileFilter;
    fileFilter << "NPBRun*";

    dirIf.setNameFilters(fileFilter);

    avDirectories = dirIf.entryList();
    for(int i=0;i<avDirectories.count();i++) {

        qDebug() << "Found dir: " << avDirectories[i];
    }
}

void GraphicsReportView::file_selected(QString dir)
{
    qDebug() << "Selected directory: " << dir;
    load_graphs(dir);
}

QString GraphicsReportView::run_name_from_utime(const QString& inStr)
{
    uint utime = inStr.right(10).toUInt();
    return QDateTime::fromTime_t(utime).toLocalTime().toString("hh:mm, ddd, d/M/yy");
}
