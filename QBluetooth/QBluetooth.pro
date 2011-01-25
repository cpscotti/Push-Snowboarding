#MMP_RULES += EXPORTUNFROZEN
#
# Freeze Howto:
# $set EPOCROOT=\NokiaQtSDK\Symbian\SDK\
# $abld freeze gcce
#
#

#uid3 = 0x9003328D


TEMPLATE = lib
TARGET   = QBluetooth
DEFINES  += BLUETOOTH_LIB

QT += core

PUBLIC_HEADERS += QBtGlobal.h \
    QBtAuxFunctions.h \
	DeviceDiscoverer/QBtDeviceDiscoverer.h \
    BTTypes/QBtAddress.h \
    BTTypes/QBtDevice.h \
    BTTypes/QBtService.h \
    BTTypes/QBtConstants.h \
    BTTypes/QBtTypes.h \
    DeviceDiscoverer/QBtSingleDeviceSelectorUI.h \
    ServiceDiscoverer/QBtServiceDiscoverer.h \
    ServiceAdvertiser/QBtServiceAdvertiser.h \
    Connection/SerialPort/Server/QBtSerialPortServer.h \
    Connection/SerialPort/Client/QBtSerialPortClient.h \
    Connection/ObjectExchange/Client/QBtObjectExchangeClient.h \
    LocalDevice/QBtLocalDevice.h \
    Connection/ObjectExchange/Server/QBtObjectExchangeServer.h \
    QBluetooth.h

INCLUDEPATH += $$EPOCROOT\epoc32\include
HEADERS += $$PUBLIC_HEADERS

SOURCES += Connection/ObjectExchange/Server/Impl/QBtObjectExchangeServer.cpp \
    LocalDevice/Impl/QBtLocalDevice.cpp \
    Connection/ObjectExchange/Client/Impl/QBtObjectExchangeClient.cpp \
    Connection/SerialPort/Client/Impl/QBtSerialPortClient.cpp \
    Connection/SerialPort/Server/Impl/QBtSerialPortServer.cpp \
    ServiceAdvertiser/Impl/QBtServiceAdvertiser.cpp \
    BTTypes/Impl/QBtService.cpp \
    ServiceDiscoverer/Impl/QBtServiceDiscoverer.cpp \
    DeviceDiscoverer/Impl/QBtSingleDeviceSelectorUI.cpp \
    DeviceDiscoverer/Impl/QBtDeviceDiscoverer.cpp \
    BTTypes/Impl/QBtAddress.cpp \
    BTTypes/Impl/QBtDevice.cpp

FORMS += 

RESOURCES +=  

symbian { 
	# fix for Qt Creator to find the symbian headers
	INCLUDEPATH += $$EPOCROOT\epoc32\include

    deploy.path = $$EPOCROOT
    exportheaders.sources = $$PUBLIC_HEADERS
    exportheaders.path = epoc32/include

    INCLUDEPATH += $$deploy.path$$exportheaders.path/QBluetooth/

    INCLUDEPATH += $$deploy.path$$exportheaders.path


    TARGET.UID3 = 0x9003328D

    TARGET.EPOCALLOWDLLDATA = 1

    HEADERS += Connection/ObjectExchange/Server/QBtObjectExchangeServer_symbian.h \
        LocalDevice/QBtLocalDevice_symbian.h \
        Connection/ObjectExchange/Client/QBtObjectExchangeClient_symbian.h \
        Connection/SerialPort/Client/QBtSerialPortClient_symbian.h \
        Connection/SerialPort/Server/QBtSerialPortServer_symbian.h \
        ServiceAdvertiser/QBtServiceAdvertiser_symbian.h \
        ServiceDiscoverer/QBtServiceDiscoverer_symbian.h \
        DeviceDiscoverer/QBtDeviceDiscoverer_symbian.h

    SOURCES += Connection/ObjectExchange/Server/Impl/QBtObjectExchangeServer_symbian.cpp \
        LocalDevice/Impl/QBtLocalDevice_symbian.cpp \
        Connection/ObjectExchange/Client/Impl/QBtObjectExchangeClient_symbian.cpp \
        Connection/SerialPort/Client/Impl/QBtSerialPortClient_symbian.cpp \
        Connection/SerialPort/Server/Impl/QBtSerialPortServer_symbian.cpp \
        ServiceAdvertiser/Impl/QBtServiceAdvertiser_symbian.cpp \
        ServiceDiscoverer/Impl/QBtServiceDiscoverer_symbian.cpp \
        DeviceDiscoverer/Impl/QBtDeviceDiscoverer_symbian.cpp
    
    # more libs based on s60 version
    contains (S60_VERSION, 3.1):BT_PLUGIN_LIB = -lbteng
    else:BT_PLUGIN_LIB = -lbtengsettings
    
    #
    LIBS += -lbluetooth \
        -lsdpdatabase \
        -lsdpagent \
        -lirobex \
        -lefsrv \
        -lfeatdiscovery \
        -lcentralrepository \
        -lbafl \
        -leikcore \
        -lcone \
        -lbtdevice \
        -lbtmanclient \
        -lesock \    	
    	-lapmime \
    	-lcommonui \
    	-lplatformenv \
    	-lcharconv
    	
    	
    LIBS += $$BT_PLUGIN_LIB
    
    TARGET.CAPABILITY = LocalServices NetworkServices UserEnvironment ReadUserData WriteUserData Location
#    TARGET.CAPABILITY = All -TCB -AllFiles -DRM
    
    for(header, exportheaders.sources):BLD_INF_RULES.prj_exports += "$$header $$deploy.path$$exportheaders.path/QBluetooth/$$basename(header)"
	
	# add this for Qt Creator to generate a pkg file, it seems to be a bug in the current version (2.0.0)
        #addFiles.sources = QBluetooth.dll
        #addFiles.path = !:\sys\bin
        #DEPLOYMENT += addFiles
}
