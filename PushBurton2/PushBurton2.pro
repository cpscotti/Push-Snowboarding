#-------------------------------------------------
#
# Project created by QtCreator 2011-01-05T12:44:22
#
#-------------------------------------------------

QT       += core gui
QT      += xml

TARGET = PushBurton2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphicpixmapbt.cpp \
    graphicsdeviceinfo.cpp \
    pushburtonanimation.cpp \
    graphicsconnmenu.cpp \
    devicesmanager.cpp \
    pushdevicesholder.cpp \
    pushburtongenericdevice.cpp \
    npushtickdisposer.cpp \
    npushlogtick.cpp \
    pushn8simulationdevice.cpp \
    npushnormfeettick.cpp \
    npushmagtick.cpp \
    npushimutick.cpp \
    npushhearttick.cpp \
    npushgsrtick.cpp \
    npushgpstick.cpp \
    npushfoottick.cpp \
    npushairtimetick.cpp \
    npushacctick.cpp \
    npushgenreport.cpp \
    pushn8btdevice.cpp \
    pushn8imudevice.cpp \
    pushn8heartdevice.cpp \
    pushn8gsrdevice.cpp \
    pushn8footdevice.cpp \
    pushn8phonemag.cpp \
    pushn8phonegps.cpp \
    pushn8phoneacc.cpp \
    pushn8simplereportsgenerator.cpp \
    pushn8airtimedetector.cpp \
    pushn8absnormfeetdevice.cpp \
    npushreportspeed.cpp \
    npushreportheart.cpp \
    npushreportgsr.cpp \
    npushreportaltitude.cpp \
    npushreportairtime.cpp \
    fuzzydetector.cpp \
    selfshrinkinglist.cpp \
    runcontroldialog.cpp \
    runmanager.cpp \
    ubiqlogsaver.cpp \
    graphicvarplot.cpp \
    graphictextbt.cpp \
    graphicsreportview.cpp \
    graphicslidingdownbts.cpp \
    liveview.cpp \
    graphicssettings.cpp

HEADERS  += mainwindow.h \
    graphicpixmapbt.h \
    graphicsdeviceinfo.h \
    pushburtonanimation.h \
    graphicsconnmenu.h \
    devicesmanager.h \
    pushdevicesholder.h \
    pushburtongenericdevice.h \
    npushtickdisposer.h \
    npushlogtick.h \
    pushn8simulationdevice.h \
    npushnormfeettick.h \
    npushmagtick.h \
    npushimutick.h \
    npushhearttick.h \
    npushgsrtick.h \
    npushgpstick.h \
    npushfoottick.h \
    npushairtimetick.h \
    npushacctick.h \
    AbsNormFeetTypes.h \
    npushgenreport.h \
    pushn8btdevice.h \
    pushn8imudevice.h \
    pushn8heartdevice.h \
    pushn8gsrdevice.h \
    pushn8footdevice.h \
    pushn8phonemag.h \
    pushn8phonegps.h \
    pushn8phoneacc.h \
    pushn8simplereportsgenerator.h \
    pushn8airtimedetector.h \
    pushn8absnormfeetdevice.h \
    npushreportspeed.h \
    npushreportheart.h \
    npushreportgsr.h \
    npushreportaltitude.h \
    npushreportairtime.h \
    fuzzydetector.h \
    selfshrinkinglist.h \
    runcontroldialog.h \
    runmanager.h \
    ubiqlogsaver.h \
    graphicvarplot.h \
    graphictextbt.h \
    graphicsreportview.h \
    graphicslidingdownbts.h \
    graphicslidingdownbts.h \
    liveview.h \
    graphicssettings.h

FORMS    += mainwindow.ui \
    liveview.ui

CONFIG   += mobility
MOBILITY = location sensors

LIBS += -lQBluetooth

symbian {
    TARGET.UID3 = 0xece639d4
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000

    INCLUDEPATH += /epoc32/include/QBlueTooth

    LIBS += -lQBluetooth \
        -leikcoctl \
        -lavkon \
        -leikcore \
        -leiksrv \
        -lcone

    TARGET.CAPABILITY = \
        LocalServices \
        NetworkServices \
        UserEnvironment \
        ReadUserData \
        WriteUserData \
        Location

    addFiles.sources = $(EPOCROOT)Epoc32/release/$(PLATFORM)/$(CFG)/QBluetooth.dll
    addFiles.path = /sys/bin
    DEPLOYMENT += addFiles
}

RESOURCES += \
    push_art.qrc
