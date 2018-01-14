#-------------------------------------------------
#
# Project created by QtCreator 2017-06-22T14:12:43
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoSellSoft
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


SOURCES += main.cpp\
        mainwindow.cpp \
    clientcard.cpp \
    mailaccountsdialog.cpp \
    smtpsettings.cpp \
    treeedit.cpp \
    graphicsview.cpp \
    scene.cpp \
    graphicsblock.cpp \
    databasedialog.cpp \
    databasesettings.cpp \
    datetimepickerdialog.cpp \
    linkgenerator.cpp \
    vksettingsdialog.cpp \
    vksettings.cpp \
    vkprocessor.cpp \
    vkadduserdialog.cpp

HEADERS  += mainwindow.h \
    clientcard.h \
    mailaccountsdialog.h \
    smtpsettings.h \
    treeedit.h \
    graphicsview.h \
    scene.h \
    graphicsblock.h \
    databasedialog.h \
    databasesettings.h \
    datetimepickerdialog.h \
    linkgenerator.h \
    vksettingsdialog.h \
    vksettings.h \
    vkprocessor.h \
    vkadduserdialog.h

FORMS    += mainwindow.ui \
    mailaccountsdialog.ui \
    treeedit.ui \
    databasedialog.ui \
    datetimepickerdialog.ui \
    linkgenerator.ui \
    vksettingsdialog.ui \
    vkadduserdialog.ui

DISTFILES +=

RESOURCES += \
    images.qrc
