#ifndef GRAPHICSCONNMENU_H
#define GRAPHICSCONNMENU_H

#include <QGraphicsObject>

#include <QStateMachine>

#include "graphicpixmapbt.h"
#include "pushburtonanimation.h"

class GraphicsConnMenu : public QGraphicsObject
{
    Q_OBJECT
public:
    GraphicsConnMenu(QObject*);
    ~GraphicsConnMenu();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *);

signals:
    void home_bt_clicked();
    void startSearch_bt_clicked();
    void stopSearch_bt_clicked();
    void kit_selected(int n);

private slots:
    void kit_1_selected();
    void kit_2_selected();
    void kit_3_selected();
    void kit_sim_selected();

private:
    bool searching;
    GraphicPixmapBt * homeBt;
    GraphicPixmapBt * startBt;
    GraphicPixmapBt * stopBt;
    GraphicPixmapBt * kit1Bt;
    GraphicPixmapBt * kit2Bt;
    GraphicPixmapBt * kit3Bt;
    GraphicPixmapBt * kitSimBt;

    QState * rootState;
    //unfolds into
        QState * btState;
        //unfolds into
            QState * btStandBy;
            QState * btSearching;
        // parallel to
        QState * kitState;
        //unfolds into
            QState * dKit1;
            QState * dKit2;
            QState * dKit3;
            QState * dKitSim;
            QState * dChooser;

    QStateMachine machine;

    void setupStateMachine();

};

#endif // GRAPHICSCONNMENU_H
