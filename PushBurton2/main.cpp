#include <QtGui/QApplication>
#include "mainwindow.h"

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
#else
    w.show();
#endif

    return a.exec();
}
