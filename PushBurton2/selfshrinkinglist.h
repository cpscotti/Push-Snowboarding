#ifndef SELFSHRINKINGLIST_H
#define SELFSHRINKINGLIST_H

#include <QList>
#include <QListIterator>

#include <QQueue>
#include <QDebug>

class SelfShrinkingList : public QList<double>
{
public:
    SelfShrinkingList(int);
    SelfShrinkingList(int, double);
    ~SelfShrinkingList();

    void push_back(double);
    void clear();

private:
    int maxLength;
    int valsPrescaler;
    double invalid_val;
    QQueue<double> inputBuffer;
};

#endif // SELFSHRINKINGLIST_H
