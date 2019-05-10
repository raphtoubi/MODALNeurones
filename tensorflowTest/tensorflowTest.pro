TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

unix:!macx: LIBS += -L$$PWD/../Libraries/Tensorflow/lib/ -ltensorflow
unix:!macx: LIBS += -L$$PWD/../Libraries/Tensorflow/lib/ -ltensorflow_framework

INCLUDEPATH += $$PWD/../Libraries/Tensorflow/include
DEPENDPATH += $$PWD/../Libraries/Tensorflow/include

DISTFILES += \
    matrices.csv \
    distances_file.csv \
    distances_file.csv
