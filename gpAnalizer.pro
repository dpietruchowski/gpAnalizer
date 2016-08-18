#-------------------------------------------------
#
# Project created by QtCreator 2015-09-21T20:46:31
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gpAnalizer
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc


SOURCES += main.cpp\
        mainwindow.cpp \
    application/node/funcnode.cpp \
    application/node/morphonode.cpp \
    application/node/node.cpp \
    application/node/nullnode.cpp \
    application/node/termnode.cpp \
    application/application.cpp \
    application/functionset.cpp \
    application/population.cpp \
    application/terminalset.cpp \
    application/tree.cpp

HEADERS  += mainwindow.h \
    application/node/funcnode.h \
    application/node/morphonode.h \
    application/node/node.h \
    application/node/nullnode.h \
    application/node/termnode.h \
    application/application.h \
    application/functions.h \
    application/functionset.h \
    application/population.h \
    application/terminalset.h \
    application/tree.h \
    application/typeFunc.h

FORMS    += mainwindow.ui
