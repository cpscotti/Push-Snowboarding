/*
 * Copyright (c) 2011 Nokia Corporation
 *
 * This file is part of the Push Snowboarding Project, More info at:
 * www.pushsnowboading.com
 *
 * Author: Clovis Scotti <scotti@ieee.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */

#include <QtGui/QApplication>
#include "mainwindow.h"

#include <QDebug>

#ifdef Q_OS_SYMBIAN
#include <eikenv.h>
#include <eikappui.h>
#include <aknenv.h>
#include <aknappui.h>
#endif



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef Q_OS_SYMBIAN
    CAknAppUi * appUi = dynamic_cast<CAknAppUi*> (CEikonEnv::Static()->AppUi());
    if(appUi) {
        TRAP_IGNORE(appUi->SetOrientationL((CAknAppUi::TAppUiOrientation)CAknAppUi::EAppOrientationPortrait);)
    }
#endif

    MainWindow w;
#if defined(Q_WS_S60)
    w.showMaximized();
//    w.showFullScreen();
#else
    w.show();
#endif

    return a.exec();
}
