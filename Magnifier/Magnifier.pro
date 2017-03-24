
TEMPLATE = app
QT += core gui widgets
TARGET = Magnifier
DESTDIR = ../bin

win32 {
    RC_FILE = app.rc
}

HEADERS = mainwindow.hpp \
    opts_dialog.hpp

SOURCES = main.cpp \
	mainwindow.cpp \
    opts_dialog.cpp
			
RESOURCES = resources.qrc

FORMS += \
    opts_dialog.ui
