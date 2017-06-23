
TEMPLATE = app
QT += core gui widgets
TARGET = PhraseGen
DESTDIR = ../bin

win32 {
    RC_FILE = app.rc
}

HEADERS =	mainwindow.hpp

SOURCES =	main.cpp \
			mainwindow.cpp

OTHER_FILES = README.md

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc
