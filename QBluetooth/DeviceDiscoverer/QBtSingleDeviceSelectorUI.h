/*
 * QBtSingleDeviceSelectorUI.h
 *
 *
 *      Author: ftylitak
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

#ifndef QBTSINGLEDEVICESELECTORUI_H_
#define QBTSINGLEDEVICESELECTORUI_H_

#include <QBtGlobal.h>
#include <QBtTypes.h>
#include <QBtDeviceDiscoverer.h>
#include <QtGui/QListWidget>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMenuBar>
#include <QAction>
#include <QModelIndex>
#include <QDialog>

/**
 * This class provides UI for searching remote bluetooth
 * devices, displaying them and selecting one device.
 *
 * When a device is selected, the UI is closed and the
 * device instance is returned through discoveryCompleted()
 * signal.
 * (NOTE: when closed it is not destroyed, just hiden)
 *
 * To use, create an instance of the class and call
 * show(). When shown, discovery starts as well.
 *
 * To get the selected remote device, the discoveryCompleted()
 * signal must be handled.
 */
class DLL_EXPORT QBtSingleDeviceSelectorUI : public QDialog
{
    Q_OBJECT

public:
    /**
     * if parent == NULL, the list will be showed as indepented
     * widget on the screen, thus it's not necessary for the client application
     * to have a GUI interface.
     */
    QBtSingleDeviceSelectorUI(QWidget* parent = NULL);

    /**
     * Destructor.
     */
    ~QBtSingleDeviceSelectorUI();


public slots:
    /**
     * On show, he device discovery starts.
     */
    void show();

protected slots:
    void populateDeviceList(const QBtDevice& device);
    void devSelected(QListWidgetItem* devName);
    void selectPressed();
    void exitPressed();

signals:
    /**
     * Emitted after device discovery stops and user selects the desired device.
     * @param selectedDevice The selected device.
     */
    void discoveryCompleted (const QBtDevice & selectedDev);

    /**
     * Emitted after calling show() if and only if the device discovery starts successfully
     */
    void discoveryStarted();
    
    
    /**
     * Emitted when the user closes the UI without choosing any device.
     */
    void userCanceled ();    

private:
    QWidget * _parent;
    QBtDevice* _selectedDevice;    
    QBtDeviceDiscoverer* _discoverer;
    QListWidget* _devList;
    QLabel* _label;
    QVBoxLayout *_verticalLayout;
    QList<QBtDevice> _devicesFound;

    QMenuBar* _menu;
    QAction* _select;
    QAction* _exit;
};

#endif /* QBTSINGLEDEVICESELECTORUI_H_ */
