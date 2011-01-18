#ifndef GRAPHICSSETTINGS_H
#define GRAPHICSSETTINGS_H

#include <QObject>
#include <QGraphicsObject>
#include "graphictextbt.h"
#include "graphicpixmapbt.h"

#include <QDebug>

#include "pushdevicesholder.h"

//Gambiarra, xunxo, workaround... (LiveView)
#include "liveview.h"

class GraphicsSettings : public QGraphicsObject
{
    Q_OBJECT
public:
    GraphicsSettings(PushDevicesHolder*, QObject*);
    ~GraphicsSettings();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *);

private:
    GraphicTextBt * startLiveViewBt;
    GraphicPixmapBt * homeBt;
    LiveView * liveView;
    PushDevicesHolder * configuredDevices;

signals:
    void home_bt_clicked();

private slots:
    void start_live_view();
    void end_live_view();
};

#endif // GRAPHICSSETTINGS_H
