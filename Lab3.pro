TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -pthread -std=c++0x
LIBS += -pthread

SOURCES += main.cpp

