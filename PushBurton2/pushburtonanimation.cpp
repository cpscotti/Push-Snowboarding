#include "pushburtonanimation.h"

PushBurtonAnimation::PushBurtonAnimation(QObject *target,
                                         const QByteArray &propertyName,
                                         QObject *parent=0)
    : QPropertyAnimation(target, propertyName, parent)
{
    QPropertyAnimation::setDuration(250);
    QPropertyAnimation::setEasingCurve(QEasingCurve::InOutQuad);
}
