
TEMPLATE = lib
TARGET = utils
DESTDIR = ../lib
INCLUDEPATH += . ..
CONFIG += qt staticlib
QT -= gui

HEADERS += utils.hpp

SOURCES += utils.cpp


libDir.commands = mkdir ../lib

QMAKE_EXTRA_TARGETS += libDir