#include "liveview.h"
#include "ui_liveview.h"

LiveView::LiveView(PushDevicesHolder * a_configuredDevices, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LiveView)
{
    ui->setupUi(this);

    configuredDevices = a_configuredDevices;

    streaming = false;

    //We should have a timer here to update the states of the devices but then every time the timer goes off
    //  the selection of "displaying" devices would be lost (if implemented roughly)/ TODO!
    UpdateConnectedList();

    for(int i=0;i<configuredDevices->count();i++)
        streamingFilter.push_back(false);

    liveOutputCount = 0;
}

LiveView::~LiveView()
{
    delete ui;
}

void LiveView::on_liveControl_clicked()
{
    if(!streaming)
    {
        ui->liveOutput->clear();
        //start streaming
        ui->liveControl->setText("Stop");
        streaming = true;
    } else {
        //stop streaming
        ui->liveControl->setText("Start");
        streaming = false;
    }

    UpdateStreamingConnections();
}

void LiveView::live_output(NPushLogTick * tick)
{
//    switch(typeid(*tick))
//    qDebug() << "Incoming type is " << typeid(*tick).name();
    ui->liveOutput->append(tick->get_pretty_print());
    liveOutputCount++;
    if(liveOutputCount > 1000)
    {
        ui->liveOutput->clear();
        liveOutputCount = 0;
        qDebug() << "Clearing textedit";
    }
}

void LiveView::on_exitBt_clicked()
{
    streaming = false;
    for(int i=0;i<streamingFilter.count(); i++)
        streamingFilter[i] = false;
    UpdateStreamingConnections();
    ui->liveOutput->clear();
    qDebug() << "destroying";
    this->close();
    emit leaving();

}

void LiveView::UpdateConnectedList()
{
    listedDevices.clear();
    ui->connectedDevicesList->clear();
    for(int i=0; i < configuredDevices->count(); i++)
    {
        QListWidgetItem * devItem = new QListWidgetItem(configuredDevices->at(i)->get_description(), ui->connectedDevicesList);
        if(configuredDevices->at(i)->is_online())
            devItem->setIcon(QIcon(":/main_menu/icon/images/online.png"));
        else
            devItem->setIcon(QIcon(":/main_menu/icon/images/offline.png"));

        listedDevices.push_back(devItem);
    }
}

void LiveView::on_connectedDevicesList_itemSelectionChanged()
{

    if(streamingFilter.count() != configuredDevices->count())
    {
        qDebug() << "Coding error here2!!!";
        return;
    }

    QList<QListWidgetItem*> selectedOnes = ui->connectedDevicesList->selectedItems();

    ui->liveOutput->clear();

    for(int i=0;i<configuredDevices->count();i++)
        streamingFilter[i] = false;

    for(int i=0;i<selectedOnes.count();i++)
    {
        for(int j=0;j<listedDevices.count();j++)
        {
            if(selectedOnes[i] == listedDevices[j])
            {
                streamingFilter[j] = true;
                qDebug() << "Enabled " << j;
            }
        }
    }

    UpdateStreamingConnections();
}

void LiveView::UpdateStreamingConnections()
{
    qDebug() << "Updating streaming";
    if(streamingFilter.count() != configuredDevices->count())
    {
        qDebug() << "Coding error here!!!";
        return;
    }

    for(int i=0;i<configuredDevices->count();i++)
    {
        PushBurtonGenericDevice * dev = configuredDevices->at(i);
        disconnect(dev, SIGNAL(reading_ready(NPushLogTick*)), this, SLOT(live_output(NPushLogTick*)));
        if(streaming)
        {
            if(streamingFilter[i]) {
                qDebug() << "Enabling" << i;
                connect(dev, SIGNAL(reading_ready(NPushLogTick*)), this, SLOT(live_output(NPushLogTick*)), Qt::UniqueConnection);
            }
        }
    }

    qDebug() << "DONE";
}
