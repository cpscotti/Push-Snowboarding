#include "graphicsconnmenu.h"

GraphicsConnMenu::GraphicsConnMenu(QObject * parent)
{
    this->setParent(parent);
    this->setZValue(0.2);

    homeBt = new GraphicPixmapBt(":/buttons/home_bt.png", this);
    startBt = new GraphicPixmapBt(":/buttons/search_bt.png", this);
    stopBt = new GraphicPixmapBt(":/buttons/stop_bt.png", this);
    kit1Bt = new GraphicPixmapBt(":/buttons/kit_1.png", this);
    kit2Bt = new GraphicPixmapBt(":/buttons/kit_2.png", this);
    kit3Bt = new GraphicPixmapBt(":/buttons/kit_3.png", this);
    kitSimBt = new GraphicPixmapBt(":/buttons/kit_sim.png", this);

    homeBt->setPos(0,0);
    startBt->setPos(110+5,0);
    stopBt->setPos(110+5,-60);

    kit1Bt->setPos(110*2+5*2, 0);
    kit2Bt->setPos(110*2+5*2, -180);
    kit3Bt->setPos(110*2+5*2, -120);
    kitSimBt->setPos(110*2+5*2, -60);

    connect(homeBt, SIGNAL(activated()), this, SIGNAL(home_bt_clicked()));
    connect(startBt, SIGNAL(activated()), this, SIGNAL(startSearch_bt_clicked()));
    connect(stopBt, SIGNAL(activated()), this, SIGNAL(stopSearch_bt_clicked()));

    setupStateMachine();
}

GraphicsConnMenu::~GraphicsConnMenu()
{
    rootState->deleteLater();
}

QRectF GraphicsConnMenu::boundingRect() const
{
    return QRectF();
}

void GraphicsConnMenu::paint(QPainter *painter, const QStyleOptionGraphicsItem * option,QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}



void GraphicsConnMenu::setupStateMachine()
{
    rootState = new QState(QState::ParallelStates);

    btState = new QState(rootState);
        btSearching = new QState(btState);
        btStandBy = new QState(btState);
        btState->setInitialState(btStandBy);
        btStandBy->addTransition(startBt, SIGNAL(activated()), btSearching);
        btSearching->addTransition(stopBt, SIGNAL(activated()), btStandBy);
        btSearching->assignProperty(startBt, "y", -60);
        btSearching->assignProperty(stopBt, "y", 0);
        btStandBy->assignProperty(startBt, "y", 0);
        btStandBy->assignProperty(stopBt, "y", -60);

    kitState = new QState(rootState);
        dKit1 = new QState(kitState);
        dKit1->assignProperty(kit2Bt, "y", -180);
        dKit1->assignProperty(kit3Bt, "y", -120);
        dKit1->assignProperty(kitSimBt, "y", -60);
        dKit1->assignProperty(kit1Bt, "y", 0);

        dKit2 = new QState(kitState);
        dKit2->assignProperty(kit1Bt, "y", -240);
        dKit2->assignProperty(kit3Bt, "y", -120);
        dKit2->assignProperty(kitSimBt, "y", -60);
        dKit2->assignProperty(kit2Bt, "y", 0);

        dKit3 = new QState(kitState);
        dKit3->assignProperty(kit1Bt, "y", -240);
        dKit3->assignProperty(kit2Bt, "y", -180);
        dKit3->assignProperty(kitSimBt, "y", -60);
        dKit3->assignProperty(kit3Bt, "y", 0);

        dKitSim = new QState(kitState);
        dKitSim->assignProperty(kit1Bt, "y", -240);
        dKitSim->assignProperty(kit2Bt, "y", -180);
        dKitSim->assignProperty(kit3Bt, "y", -120);
        dKitSim->assignProperty(kitSimBt, "y", 0);

        dChooser = new QState(kitState);
        dChooser->assignProperty(kit1Bt, "y", 0);
        dChooser->assignProperty(kit2Bt, "y", 60);
        dChooser->assignProperty(kit3Bt, "y", 120);
        dChooser->assignProperty(kitSimBt, "y", 180);

        kitState->setInitialState(dKit1);

        dKit1->addTransition(kit1Bt, SIGNAL(activated()), dChooser);
        connect(dKit1, SIGNAL(entered()), this, SLOT(kit_1_selected()));
        dKit2->addTransition(kit2Bt, SIGNAL(activated()), dChooser);
        connect(dKit2, SIGNAL(entered()), this, SLOT(kit_2_selected()));
        dKit3->addTransition(kit3Bt, SIGNAL(activated()), dChooser);
        connect(dKit3, SIGNAL(entered()), this, SLOT(kit_3_selected()));

        dKitSim->addTransition(kitSimBt, SIGNAL(activated()), dChooser);
        connect(dKitSim, SIGNAL(entered()), this, SLOT(kit_sim_selected()));

        dChooser->addTransition(kit1Bt, SIGNAL(activated()), dKit1);
        dChooser->addTransition(kit2Bt, SIGNAL(activated()), dKit2);
        dChooser->addTransition(kit3Bt, SIGNAL(activated()), dKit3);
        dChooser->addTransition(kitSimBt, SIGNAL(activated()), dKitSim);


//    machine.setGlobalRestorePolicy(QStateMachine::RestoreProperties);//unusable with Parallel Machines
    machine.addState(rootState);
    machine.setInitialState(rootState);

    machine.addDefaultAnimation(new PushBurtonAnimation(kit1Bt, "y", kit1Bt));
    machine.addDefaultAnimation(new PushBurtonAnimation(kit2Bt, "y", kit2Bt));
    machine.addDefaultAnimation(new PushBurtonAnimation(kit3Bt, "y", kit3Bt));
    machine.addDefaultAnimation(new PushBurtonAnimation(kitSimBt, "y", kitSimBt));
    machine.addDefaultAnimation(new PushBurtonAnimation(startBt, "y", startBt));
    machine.addDefaultAnimation(new PushBurtonAnimation(stopBt, "y", stopBt));

    machine.start();
}

void GraphicsConnMenu::kit_1_selected()
{
    emit kit_selected(1);
    qDebug() << "Kit 1 selected";
}

void GraphicsConnMenu::kit_2_selected()
{
    emit kit_selected(2);
    qDebug() << "Kit 2 selected";
}

void GraphicsConnMenu::kit_3_selected()
{
    emit kit_selected(3);
    qDebug() << "Kit 3 selected";
}


void GraphicsConnMenu::kit_sim_selected()
{
    emit kit_selected(-1);
    qDebug() << "Simulation kit selected";
}
