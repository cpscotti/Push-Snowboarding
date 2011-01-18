#ifndef PUSHBURTONANIMATION_H
#define PUSHBURTONANIMATION_H

#include <QPropertyAnimation>
#include <QEasingCurve>

class PushBurtonAnimation : public QPropertyAnimation
{
public:
    PushBurtonAnimation(QObject *,
                        const QByteArray &,
                        QObject *);
};

#endif // PUSHBURTONANIMATION_H
