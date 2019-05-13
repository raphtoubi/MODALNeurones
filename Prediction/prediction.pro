TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    model.cpp

unix:!macx: LIBS += -L$$PWD/../Libraries/Tensorflow/lib/ -ltensorflow
unix:!macx: LIBS += -L$$PWD/../Libraries/Tensorflow/lib/ -ltensorflow_framework

INCLUDEPATH += $$PWD/../Libraries/Tensorflow/include
DEPENDPATH += $$PWD/../Libraries/Tensorflow/include

DISTFILES += \
    checkpoints/checkpoint \
    checkpoints/checkpoint.ckpt.data-00000-of-00001 \
    checkpoints/checkpoint.ckpt.index \
    matrices.csv \
    distances_file.csv \
    distances_file.csv \
    single_input_model.pb

macx: LIBS += -L$$PWD/../../../C++/Libraries/Tensorflow/lib/ -ltensorflow
macx: LIBS += -L$$PWD/../../../C++/Libraries/Tensorflow/lib/ -ltensorflow_framework

INCLUDEPATH += $$PWD/../../../C++/Libraries/Tensorflow/include
DEPENDPATH += $$PWD/../../../C++/Libraries/Tensorflow/include

HEADERS += \
    model.hpp
