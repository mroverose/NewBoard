#ifndef WBGLOBALS_H
#define WBGLOBALS_H

#include <QString>
#include <QFile>

#define DELETEPTR(ptr) if(NULL != ptr){ \
    delete ptr; \
    ptr = NULL; \
    }

#define SET_STYLE_SHEET() {\
    setAttribute(Qt::WA_StyledBackground, true);\
    QString style;\
    QFile f(":/style.qss");\
    if(f.exists() && f.open(QIODevice::ReadOnly))\
            style = QString(f.readAll());\
     setStyleSheet(style);\
}

#ifdef Q_OS_WIN

#define WARNINGS_DISABLE __pragma(warning(push, 0));
#define WARNINGS_ENABLE __pragma(warning(pop));

#ifdef NO_THIRD_PARTY_WARNINGS
// disabling warning level to 0 and save old state
#define THIRD_PARTY_WARNINGS_DISABLE WARNINGS_DISABLE
#else
// just save old state (needs for not empty define)
#define THIRD_PARTY_WARNINGS_DISABLE __pragma(warning(push));
#endif //#ifdef NO_THIRD_PARTY_WARNINGS

#else //#ifdef Q_OS_WIN

#define WARNINGS_DISABLE _Pragma("GCC diagnostic push"); \
_Pragma("GCC diagnostic ignored \"-Wunused-parameter\""); \
_Pragma("GCC diagnostic ignored \"-Wunused-variable\""); \
_Pragma("GCC diagnostic ignored \"-Wsign-compare\"");

#define WARNINGS_ENABLE _Pragma("GCC diagnostic pop");

#ifdef NO_THIRD_PARTY_WARNINGS
//disabling some warnings
#define THIRD_PARTY_WARNINGS_DISABLE WARNINGS_DISABLE
#else
// just save old state (needs for not empty define)
#define THIRD_PARTY_WARNINGS_DISABLE _Pragma("GCC diagnostic push");

#endif //#ifdef NO_THIRD_PARTY_WARNINGS

#endif //#ifdef Q_OS_WIN

// anyway on any OS
#define THIRD_PARTY_WARNINGS_ENABLE WARNINGS_ENABLE

#endif // WBGLOBALS_H

