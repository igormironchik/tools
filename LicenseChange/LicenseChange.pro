
TEMPLATE = app
QT += core gui widgets
TARGET = LicenseChange
DESTDIR = ../bin

win32 {
    RC_FILE = app.rc
}

HEADERS = mainwindow.hpp \
    textedit.hpp \
    proxy.hpp \
	delayedexecutiontimer.hpp

SOURCES = main.cpp \
	mainwindow.cpp \
    textedit.cpp \
    proxy.cpp \
	delayedexecutiontimer.cpp
			
RESOURCES = resources.qrc

OTHER_FILES = README.md
