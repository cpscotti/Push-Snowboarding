#include "graphicconfirmationrequest.h"

GraphicConfirmationRequest::GraphicConfirmationRequest(const QString& question, QGraphicsItem* parent) :
    QGraphicsObject(parent)
{
    int pH = 368;
    questionPane = new GraphicTextBt(question, QRectF(0,0,340, pH), parent);
    questionPane->setPos(0,0);//parent is main graphics scene!
    questionPane->setZValue(2.0);//parent is main graphics scene!
    connect(questionPane, SIGNAL(activated()), this, SIGNAL(rejected()));

    yesPane = new GraphicTextBt(QString("Yes"),QRectF(0,0,168, 54), parent);
    yesPane->setPos(0,pH+6);
    yesPane->setZValue(2.0);
    connect(yesPane, SIGNAL(activated()), this, SIGNAL(accepted()));

    noPane = new GraphicTextBt(QString("No"),  QRectF(0,0,168, 54), parent);
    noPane->setPos(168+4,pH+6);
    noPane->setZValue(2.0);
    connect(noPane, SIGNAL(activated()), this, SIGNAL(rejected()));

}

GraphicConfirmationRequest::~GraphicConfirmationRequest()
{
    if(questionPane) questionPane->deleteLater();
    if(yesPane) yesPane->deleteLater();
    if(noPane) noPane->deleteLater();
    qDebug() << "hide() called";
}

void GraphicConfirmationRequest::hide()
{
    questionPane->deleteLater();
    yesPane->deleteLater();
    noPane->deleteLater();

    questionPane = yesPane = noPane = 0;

    qDebug() << "hide() called";
}

QRectF GraphicConfirmationRequest::boundingRect() const
{
    return QRectF();
}
void GraphicConfirmationRequest::paint(QPainter *painter, const QStyleOptionGraphicsItem *,QWidget *)
{
    Q_UNUSED(painter);
}
