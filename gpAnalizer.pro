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

LIBS += -L"$$OUT_PWD/libs" -ltinyxml2
LIBS += -L"$$OUT_PWD/libs" -lopencv_highgui -lopencv_core -lopencv_imgcodecs -lopencv_imgproc
LIBS += -L"$$OUT_PWD/libs" -lboost_system


SOURCES += main.cpp\
    ui/settings.cpp \
    ui/myvalidator.cpp \
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
    fitness/fitness.cpp \
    fitness/fitnessgenerator.cpp \
    fitness/hamming.cpp \
    fitness/hausdorff.cpp \
    fitness/hausdorffcanny.cpp \
    fitness/hausdorffsmall.cpp \
    selection/rouletteselection.cpp \
    selection/selection.cpp \
    selection/selectiontypes.cpp \
    selection/tournamentselection.cpp \
    operation/mutation.cpp \
    operation/hoistmutation.cpp \
    operation/nodemutation.cpp \
    operation/subtreemutation.cpp \
    operation/collapsemutation.cpp \
    operation/geneticoperation.cpp \
    operation/crossover.cpp \
    operation/subtreecrossover.cpp \
    operation/arity2crossover.cpp \
    operation/geneticoperationgenerator.cpp \
    myapplication.cpp \
    operation/copy.cpp \
    application/stats.cpp \
    fitness/hausdorffmodified.cpp \
    ui/applicationtab.cpp \
    ui/mainwindow.cpp

HEADERS  += \
    ui/settings.h \
    ui/myvalidator.h \
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
    fitness/fitness.h \
    fitness/fitnessgenerator.h \
    fitness/hamming.h \
    fitness/hausdorff.h \
    fitness/hausdorffcanny.h \
    fitness/hausdorffsmall.h \
    selection/rouletteselection.h \
    selection/selection.h \
    selection/selection_h.h \
    selection/selectiontypes.h \
    selection/tournamentselection.h \
    fitness/fitness_h.h \
    operation/mutation.h \
    operation/geneticoperation.h \
    operation/hoistmutation.h \
    operation/nodemutation.h \
    operation/subtreemutation.h \
    operation/collapsemutation.h \
    operation/crossover.h \
    operation/subtreecrossover.h \
    operation/arity2crossover.h \
    operation/geneticoperation_h.h \
    operation/geneticoperationgenerator.h \
    myapplication.h \
    operation/copy.h \
    application/stats.h \
    fitness/hausdorffmodified.h \
    ui/applicationtab.h \
    ui/mainwindow.h

FORMS    += \
    ui/settings.ui \
    ui/applicationtab.ui \
    ui/mainwindow.ui

    prog.path = $$_PRO_FILE_PWD_/../GP
    prog.files = $$OUT_PWD/gpAnalizer
INSTALLS += prog

message($$OUT_PWD)
message($$_PRO_FILE_PWD_)
