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

LIBS += -ltinyxml2
LIBS += -lopencv_highgui -lopencv_core -lopencv_imgcodecs -lopencv_imgproc
LIBS += -L"/usr/lib/x86_64-linux-gnu" -lboost_system


SOURCES += main.cpp\
    mainwindow.cpp \
    node/nodeid.cpp \
    node/node.cpp \
    node/terminalset.cpp \
    node/terminalnode.cpp \
    node/functionset.cpp \
    node/functionnode.cpp \
    node/morphonode.cpp \
    generator/nodegenerator.cpp \
    application/tree.cpp \
    application/population.cpp \
    application/application.cpp \
    node/threshnode.cpp \
    parser/parser.cpp \

HEADERS  += mainwindow.h \
    node/nodes.h \
    node/nodeid.h \
    node/node.h \
    node/terminalset.h \
    node/terminalnode.h \
    node/functionset.h \
    node/functionnode.h \
    node/morphonode.h \
    generator/generator.h \
    generator/nodegenerator.h \
    application/tree.h \
    application/population.h \
    application/application.h \
    node/threshnode.h \
    parser/parser.h \

FORMS    += mainwindow.ui \
    settings.ui

    prog.path = $$_PRO_FILE_PWD_/../GP
    prog.files = $$OUT_PWD/gpAnalizer
INSTALLS += prog

message($$OUT_PWD)
message($$_PRO_FILE_PWD_)
