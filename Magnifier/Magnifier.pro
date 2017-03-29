
TEMPLATE = app
QT += core gui widgets
TARGET = Magnifier
DESTDIR = ../bin

win32 {
    RC_FILE = app.rc
}

HEADERS = mainwindow.hpp \
    zoomwindow.hpp \
    constants.hpp

SOURCES = main.cpp \
	mainwindow.cpp \
    zoomwindow.cpp
			
RESOURCES = resources.qrc
