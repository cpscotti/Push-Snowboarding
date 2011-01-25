/*
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <QBtSingleDeviceSelectorUI.h>
#include <QList>
#include <QBtAuxFunctions.h>
#include <QDesktopWidget>
#include <QApplication>

// ----------------------------------------------------------------------------
// QBtSingleDeviceSelectorUI::::QBtSingleDeviceSelectorUI(QWidget* parent) :
//		QFrame(parent), prnt(parent)
//
// constuctor
// ----------------------------------------------------------------------------
QBtSingleDeviceSelectorUI::QBtSingleDeviceSelectorUI(QWidget* parent) :
    QDialog(parent), _parent(parent), _selectedDevice(NULL), _discoverer(NULL),
    _devList(NULL), _menu(NULL), _select(NULL), _exit(NULL)
{
    //setup UI
    setObjectName(QString::fromUtf8("btDeviceSelectorUIWidget"));
    setAutoFillBackground(true);
    //setFrameShape(QFrame::WinPanel);
    //setFrameShadow(QFrame::Sunken);
    this->setFocus();
    
    
    QDesktopWidget* dw = QApplication::desktop();
    int width = dw->screenGeometry().width();
    int height = dw->screenGeometry().height() /4;
        
    setGeometry(QRect(0, 148 , width, height));

    _verticalLayout = new QVBoxLayout(this);
    _verticalLayout->setSpacing(6);
    _verticalLayout->setMargin(11);
    _verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

    _label = new QLabel(this);
    _label->setText("Devices:");
    _verticalLayout->addWidget(_label);

    _devList = new QListWidget(this);

    /*QAction* clear = new QAction(tr("&Exit"), this);
    clear->setSoftKeyRole(QAction::NegativeSoftKey);
    connect(clear, SIGNAL(triggered()), this, SLOT(closeWidget()));

    devList->addAction(clear);
    addAction(clear);*/

    _devList->setFocusPolicy(Qt::WheelFocus);
    _devList->setFrameShape(QFrame::WinPanel);
    _devList->setFrameShadow(QFrame::Sunken);
    _devList->setFocus();
    _verticalLayout->addWidget(_devList);

    _select = new QAction(QString("Select"), this);
    _select->setSoftKeyRole(QAction::PositiveSoftKey);
    connect(_select, SIGNAL(triggered()), this, SLOT(selectPressed()));
    addAction(_select);


    _exit = new QAction(QString("Cancel"), this);
    _exit->setSoftKeyRole(QAction::NegativeSoftKey);
    connect(_exit, SIGNAL(triggered()), this, SLOT(exitPressed()));
    addAction(_exit);

    /*menu = new QMenuBar(this);
    menu->addAction(select);
    menu->addAction(exit);

    menu->setNativeMenuBar(true);
    verticalLayout->setMenuBar(menu);*/

    ////////////////

    _discoverer = new QBtDeviceDiscoverer(parent);

    connect(_discoverer, SIGNAL(newDeviceFound(const QBtDevice&)),
            this, SLOT(populateDeviceList(const QBtDevice&)));

    connect(_devList, SIGNAL(itemActivated(QListWidgetItem *)),
                    this, SLOT(devSelected(QListWidgetItem *)));
    this->hide();

}

// ----------------------------------------------------------------------------
// QBtSingleDeviceSelectorUI::~QBtSingleDeviceSelectorUI()
// destructor
// ----------------------------------------------------------------------------
QBtSingleDeviceSelectorUI::~QBtSingleDeviceSelectorUI()
{
    SafeDelete(_discoverer);
    //SafeDelete(selectedDevice);
    SafeDelete(_menu);
    SafeDelete(_select);
    SafeDelete(_exit);
}

// ----------------------------------------------------------------------------
// QBtSingleDeviceSelectorUI::show()
//
// On show, the device discovery starts an the UI is displayed
// ----------------------------------------------------------------------------
void QBtSingleDeviceSelectorUI::show()
{
    _devicesFound.clear();

    //menu->show();

    //int y1 = menu->geometry().top() - 150;
    //setGeometry(QRect(0, (y1 >= 0) ? y1 : 0 , 240, 150));
    QDialog::show();
    _devList->setFocus(Qt::PopupFocusReason);

    _discoverer->startDiscovery();
    emit discoveryStarted();
}

void QBtSingleDeviceSelectorUI::populateDeviceList(const QBtDevice& device)
{
    QBtDevice* newDev = new QBtDevice(device);
    _devicesFound.append(*newDev);
    _devList->addItem(device.getName());
}

void QBtSingleDeviceSelectorUI::devSelected(QListWidgetItem* devName)
{
    for(int i=0; i<_devicesFound.size(); i++)
    {
        if(_devicesFound[i].getName() == devName->text())
        {
            _selectedDevice = &(_devicesFound[i]);
            break;
        }
    }

    if(_selectedDevice)
        emit discoveryCompleted(*_selectedDevice);
    else
        emit discoveryCompleted(QBtDevice());
    this->hide();
    _parent->setFocusProxy(NULL);

    _discoverer->stopDiscovery();

    _devicesFound.clear();
    _devList->clear();
}

void QBtSingleDeviceSelectorUI::selectPressed()
{
    QList<QListWidgetItem*> selectedDevices = _devList->selectedItems();
    if(selectedDevices.size() > 0)
        devSelected(selectedDevices[0]);
}

void QBtSingleDeviceSelectorUI::exitPressed()
{
    this->hide();
    emit userCanceled();
}
