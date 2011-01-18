#ifndef LIVEVIEW_H
#define LIVEVIEW_H

#include <QMainWindow>

#include <QVector>
#include <QListWidget>
#include <QListWidgetItem>

#include "pushdevicesholder.h"
#include "pushburtongenericdevice.h"
#include "npushlogtick.h"

namespace Ui {
    class LiveView;
}

class LiveView : public QMainWindow
{
    Q_OBJECT

public:
    explicit LiveView(PushDevicesHolder * a_configuredDevices, QWidget *parent = 0);
    ~LiveView();

private:

    QList<QListWidgetItem*> listedDevices;

    void UpdateConnectedList();
    void UpdateStreamingConnections();

//    QVector<PushBurtonGenericDevice *> * configuredDevices;
    PushDevicesHolder * configuredDevices;

    QVector<bool> streamingFilter;

    Ui::LiveView *ui;
    bool streaming;
    int liveOutputCount;

signals:
    void leaving();

private slots:
    void on_connectedDevicesList_itemSelectionChanged();
    void on_exitBt_clicked();
    void on_liveControl_clicked();

    void live_output(NPushLogTick *);
};

#endif // LIVEVIEW_H
