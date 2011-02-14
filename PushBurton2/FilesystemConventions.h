#ifndef FILESYSTEMCONVENTIONS_H
#define FILESYSTEMCONVENTIONS_H

#define FSC_FSYS_SLASH "/"

#ifdef Q_OS_SYMBIAN
    #define FSC_RUNS_FOLDERS_ROOT "E:/"
    #define FSC_FSYS_SLASH "/"
    #define FSC_SETTINGS_FOLDER ""
#endif

#ifdef Q_OS_LINUX
    #define FSC_RUNS_FOLDERS_ROOT "/home/cpscotti/"
    #define FSC_FSYS_SLASH "/"
    #define FSC_SETTINGS_FOLDER "NPBSettings/"
#endif

#ifdef Q_OS_WIN32
    #define FSC_RUNS_FOLDERS_ROOT "C:\\"
    #define FSC_FSYS_SLASH "\\"
    #define FSC_SETTINGS_FOLDER "NPBSettings\\"
#endif

#define FSC_RUNS_PREFIX "NPBRun"
#define FSC_RUNS_FILTER "NPBRun*"

#endif // FILESYSTEMCONVENTIONS_H
