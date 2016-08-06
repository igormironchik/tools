
TEMPLATE = app
QT += core gui widgets
TARGET = LicenseChange
DESTDIR = ../bin

win32 {
    RC_FILE = app.rc
}

HEADERS =	mainwindow.hpp

SOURCES =	main.cpp \
			mainwindow.cpp
			
RESOURCES = resources.qrc

OTHER_FILES = README.md
