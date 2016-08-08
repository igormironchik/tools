
TEMPLATE = app
DESTDIR = .
CONFIG += console
QT += testlib
QT -= gui

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../lib -lutils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../lib -lutils
else:symbian: LIBS += -lutils
else:unix: LIBS += -L$$OUT_PWD/../../../lib -lutils

INCLUDEPATH += $$PWD/../../../utils
DEPENDPATH += $$PWD/../../../utils

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../lib/utils.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../lib/utils.lib
else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../../../lib/libutils.a
