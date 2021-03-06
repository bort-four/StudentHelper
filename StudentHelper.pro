#-------------------------------------------------
#
# Project created by QtCreator 2015-09-17T10:37:51
#
#-------------------------------------------------

QT       += core gui
QT 		 += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StudentHelper
TEMPLATE = app

CONFIG += c++11


SOURCES += main.cpp\
    studenthelper.cpp \
    filetreeitem.cpp \
    studenthelperwiget.cpp \
    filebrowserwidget.cpp \
    searcherwidget.cpp \
    printerwidget.cpp \
    filedialog.cpp \
    printcompositor.cpp

HEADERS  += \
    studenthelper.h \
    filetreeitem.h \
    studenthelperwiget.h \
    filebrowserwidget.h \
    searcherwidget.h \
    printerwidget.h \
    filedialog.h \
    printcompositor.h

FORMS    += \
    studenthelperwiget.ui \
    filebrowserwidget.ui \
    searcherwidget.ui \
    filewidget.ui   \
    printerwidget.ui \
    folderwidget.ui \
    filedialog.ui

RESOURCES += \
    icons.qrc
