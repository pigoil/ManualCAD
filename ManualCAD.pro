#-------------------------------------------------
#
# Project created by QtCreator 2018-06-06T14:42:55
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ManualCAD
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += Core \
               Utils \

SOURCES += main.cpp \
           mainwindow.cpp \
    mainwidget.cpp \
    Core/paintengine.cpp \
    Utils/mcad_utils.cpp \
    Core/usercommand.cpp \
    parameterdialog.cpp

HEADERS  += mainwindow.h \
    mainwidget.h \
    Core/paintengine.h \
    Utils/mcad_utils.h \
    Core/usercommand.h \
    parameterdialog.h

FORMS    += mainwindow.ui \
    parameterdialog.ui

RESOURCES += \
    resource.qrc
