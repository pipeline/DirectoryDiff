#-------------------------------------------------
#
# Project created by QtCreator 2014-04-07T21:32:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DirectoryDiff
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    textviewwithscrollevent.cpp \
    diff_match_patch.cpp \
    opendirdialog.cpp

HEADERS  += mainwindow.h \
    textviewwithscrollevent.h \
    diff_match_patch.h \
    opendirdialog.h

FORMS    += mainwindow.ui \
    opendirdialog.ui
