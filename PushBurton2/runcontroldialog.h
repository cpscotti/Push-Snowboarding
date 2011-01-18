#ifndef RUNCONTROLDIALOG_H
#define RUNCONTROLDIALOG_H

#include <QDebug>
#include <QGraphicsObject>
#include <QStateMachine>
#include <QFont>
#include <QDateTime>
#include <QTime>
#include "graphicpixmapbt.h"

class RunControlDialog : public QGraphicsObject
{
    Q_OBJECT
public:
    RunControlDialog(QObject*);
    ~RunControlDialog();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *);

protected:
    void timerEvent(QTimerEvent *);

private:
    GraphicPixmapBt * homeBt;
    GraphicPixmapBt * startRunBt;
    GraphicPixmapBt * stopRunBt;

    QState * rootState;
    QState * standBy;
    QState * logging;
    QStateMachine machine;
    QFont nokiaFont;

    void init_state_machine();

    uint runStart;
    bool run_active;
    int timerId;

private slots:
    void logging_start();
    void logging_end();


signals:
    void home_bt_clicked();
    void start_run();
    void stop_run();

};

#endif // RUNCONTROLDIALOG_H
