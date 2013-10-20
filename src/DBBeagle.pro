# -------------------------------------------------
# Project created by QtCreator 2010-04-16T16:05:25
# -------------------------------------------------
QT +=  core gui widgets sql
CONFIG += c++11
TARGET = DBBeagle
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    dbbeagleapplication.cpp \
    dbconnectiondialog.cpp \
    adaptors/mysqldialectadaptor.cpp \
    adaptors/orasqldialectadaptor.cpp \
    appaboutdialog.cpp
HEADERS += mainwindow.h \
    dbbeagleapplication.h \
    dbconnectiondialog.h \
    sqldialectadaptor.h \
    adaptors/mysqldialectadaptor.h \
    adaptors/orasqldialectadaptor.h \
    appaboutdialog.h
FORMS += mainwindow.ui \
    dbconnectiondialog.ui \
    appaboutdialog.ui
RESOURCES += dbbeagle.qrc

# Resource file for application icon for Windows
RC_FILE = dbbeagle_win.rc

# Icon file for Mac OS X
ICON = icons/icon.icns

TRANSLATIONS = dbbeagle_ru.ts

# Generate version information with the help of QMAKE_SUBSTITUTES
GIT_VERSION = $$system($$quote(git describe))
GIT_TIMESTAMP = $$system($$quote(git log -n 1 --format=format:"%at"))

QMAKE_SUBSTITUTES += $$PWD/version.h.in
