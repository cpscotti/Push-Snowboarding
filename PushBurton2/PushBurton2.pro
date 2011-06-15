#-------------------------------------------------
# * Copyright (c) 2011 Nokia Corporation
# *
# * This file is part of the Push Snowboarding Project, More info at:
# * www.pushsnowboading.com
# *
# * Author: Clovis Scotti <scotti@ieee.org>
# *
# * This program is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with this program.  If not, see <http://www.gnu.org/licenses/>.
# *
# * You should have received a copy of the GNU General Public License
# * along with this program; if not, write to the Free Software
# * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
# *
#-------------------------------------------------


QT       += core gui xml phonon network

TARGET = PushBurton2
TEMPLATE = app


SOURCES += \
    main.cpp\
    devicesmanager.cpp \
    pushdevicesholder.cpp \
    pushburtongenericdevice.cpp \
    npushtickdisposer.cpp \
    pushn8simulationdevice.cpp \
    npushlogtick.cpp \
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
    runmanager.cpp \
    ubiqlogsaver.cpp \
    UI/runcontroldialog.cpp \
    UI/graphicvarplot.cpp \
    UI/graphictextbt.cpp \
    UI/graphicsreportview.cpp \
    UI/graphicslidingdownbts.cpp \
    UI/liveview.cpp \
    UI/graphicssettings.cpp \
    UI/mainwindow.cpp \
    UI/graphicpixmapbt.cpp \
    UI/graphicsdeviceinfo.cpp \
    UI/graphicsconnmenu.cpp \
    UI/pushburtonanimation.cpp \
    virtualbrosdevice.cpp \
    rotationcounter.cpp \
    npushrotationstick.cpp \
    broadcasterdevice.cpp

HEADERS  += \
    devicesmanager.h \
    pushdevicesholder.h \
    pushburtongenericdevice.h \
    npushtickdisposer.h \
    pushn8simulationdevice.h \
    npushlogtick.h \
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
    runmanager.h \
    ubiqlogsaver.h \
    FilesystemConventions.h \
    virtualbrosdevice.h \
    rotationcounter.h \
    npushrotationstick.h \
    UI/mainwindow.h \
    UI/graphicpixmapbt.h \
    UI/graphicsdeviceinfo.h \
    UI/graphicsconnmenu.h \
    UI/runcontroldialog.h \
    UI/graphicvarplot.h \
    UI/graphictextbt.h \
    UI/graphicsreportview.h \
    UI/graphicslidingdownbts.h \
    UI/liveview.h \
    UI/graphicssettings.h \
    UI/pushburtonanimation.h \
    broadcasterdevice.h

FORMS    += \
    UI/mainwindow.ui \
    UI/liveview.ui

CONFIG   += mobility
MOBILITY = location sensors

#LIBS += -lQBluetooth
soundFiles.sources = sounds/*.mp3
DEPLOYMENT += soundFiles


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
        -lcone \
        -ltzclient

    TARGET.CAPABILITY = \
        LocalServices \
        NetworkServices \
        UserEnvironment \
        ReadUserData \
        WriteUserData \
        Location

    HEADERS  += \
        pushn8btdevice.h \
        pushn8imudevice.h \
        pushn8heartdevice.h \
        pushn8gsrdevice.h \
        pushn8footdevice.h

    SOURCES  += \
        pushn8btdevice.cpp \
        pushn8imudevice.cpp \
        pushn8heartdevice.cpp \
        pushn8gsrdevice.cpp \
        pushn8footdevice.cpp

    addFiles.sources = $(EPOCROOT)Epoc32/release/$(PLATFORM)/$(CFG)/QBluetooth.dll
    addFiles.path = /sys/bin

    DEPLOYMENT += addFiles

    isEmpty(ICON):ICON = images/burton_logo.svg
}

RESOURCES += \
    push_art.qrc

#OTHER_FILES += \
#    ClassDiagram.txt
