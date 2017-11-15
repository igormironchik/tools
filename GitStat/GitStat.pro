
TEMPLATE = app
QT -= gui
CONFIG += console c++14
TARGET = GitStat
DESTDIR = ../bin
DEFINES += ARGS_QSTRING_BUILD

INCLUDEPATH = ../3rdparty/Args

SOURCES = main.cpp

OTHER_FILES = README.md
