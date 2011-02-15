#ifndef FILESYSTEMCONVENTIONS_H
#define FILESYSTEMCONVENTIONS_H

//#define FSC_FSYS_SLASH "/"

//#warning FSC loaded

#ifdef Q_OS_SYMBIAN
    #define FSC_RUNS_FOLDERS_ROOT "E:/"
    #define FSC_FSYS_SLASH "/"
    #define FSC_SETTINGS_FOLDER ""
//    #warning Q_OS_SYMBIAN Selected
#else
    #ifdef Q_OS_LINUX
        #define FSC_RUNS_FOLDERS_ROOT "/home/cpscotti/"
        #define FSC_FSYS_SLASH "/"
        #define FSC_SETTINGS_FOLDER "NPBSettings/"
//        #warning Q_OS_LINUX Selected
    #else
        #ifdef Q_OS_WIN32
            #define FSC_RUNS_FOLDERS_ROOT "C:\\"
            #define FSC_FSYS_SLASH "\\"
            #define FSC_SETTINGS_FOLDER "NPBSettings\\"
//            #warning Q_OS_WIN32 Selected
        #else
            #error NO PLATFORM Selected
        #endif
#endif

#endif

#define FSC_RUNS_PREFIX "NPBRun"
#define FSC_RUNS_FILTER "NPBRun*"

#endif // FILESYSTEMCONVENTIONS_H
