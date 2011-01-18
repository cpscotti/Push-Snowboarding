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

    painter->setPen(QColor(17,83,161));

    painter->setFont(QFont("Nokia Standard Bold", 6));

    painter->drawText(QRect(0,60,340,382), Qt::AlignCenter, "Woohoo, nothing creepy here!\n We set it all up for you!");
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
