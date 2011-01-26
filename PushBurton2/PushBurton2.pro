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
        -lcone \
        -ltzclient

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

    isEmpty(ICON):ICON = images/burton_logo.svg
}

RESOURCES += \
    push_art.qrc
