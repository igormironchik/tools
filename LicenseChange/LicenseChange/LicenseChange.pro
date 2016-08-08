
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
	delayedexecutiontimer.hpp \
    worker.hpp

SOURCES = main.cpp \
	mainwindow.cpp \
    textedit.cpp \
    proxy.cpp \
	delayedexecutiontimer.cpp \
    worker.cpp
			
RESOURCES = resources.qrc

unix|win32: LIBS += -L$$OUT_PWD/../lib -lutils

INCLUDEPATH += $$PWD/../utils
DEPENDPATH += $$PWD/../utils

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../lib/utils.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../lib/libutils.a
