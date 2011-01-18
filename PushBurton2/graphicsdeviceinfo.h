#ifndef GRAPHICSDEVICEINFO_H
#define GRAPHICSDEVICEINFO_H

#include <QGraphicsObject>
#include <QPixmap>
#include <QPainter>
#include <QDebug>

#include <QTimerEvent>
#include <QFont>

class GraphicsDeviceInfo : public QGraphicsObject
{
    Q_OBJECT
public:
    GraphicsDeviceInfo(QObject*);
    ~GraphicsDeviceInfo();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *);

signals:
    void activated();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void timerEvent(QTimerEvent *);

public slots:
    void location_online();
    void snowb_online();
    void heart_online();
    void arm_online();
    void boot_r_online();
    void boot_l_online();

    void location_conn();
    void snowb_conn();
    void heart_conn();
    void arm_conn();
    void boot_r_conn();
    void boot_l_conn();

private:
    int timerId;
    QPixmap centralPx;

    bool locationOn;
    int locationBlink;

    bool snowbOn;
    int snowbBlink;

    bool heartOn;
    int heartBlink;

    bool armOn;
    int armBlink;

    bool rightBootOn;
    int rightBootBlink;

    bool leftBootOn;
    int leftBootBlink;
};

#endif // GRAPHICSDEVICEINFO_H
