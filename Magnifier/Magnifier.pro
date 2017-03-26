
TEMPLATE = app
QT += core gui widgets xml
TARGET = Magnifier
DESTDIR = ../bin

win32 {
    RC_FILE = app.rc
}

HEADERS = mainwindow.hpp \
    opts_dialog.hpp \
    $$GENERATED

SOURCES = main.cpp \
	mainwindow.cpp \
    opts_dialog.cpp
			
RESOURCES = resources.qrc

FORMS += \
    opts_dialog.ui

OTHER_FILES = cfg.qtconf

TO_GENERATE = cfg.qtconf

GENERATED =

QMAKE_EXTRA_COMPILERS += generate_cfg
generate_cfg.name = CONF_GEN
generate_cfg.input = TO_GENERATE
generate_cfg.output = ${QMAKE_FILE_BASE}.hpp
generate_cfg.CONFIG = no_link
generate_cfg.variable_out = GENERATED

generate_cfg.commands = $$shell_path( $$absolute_path( $${OUT_PWD}/../3rdparty/QtConfFile/qtconffile.generator ) ) \
-i ${QMAKE_FILE_IN} \
-o $${OUT_PWD}/${QMAKE_FILE_BASE}.hpp

PRE_TARGETDEPS += compiler_generate_cfg_make_all

unix|win32: LIBS += -L$$OUT_PWD/../3rdparty/QtConfFile/lib/ -lQtConfFile

INCLUDEPATH += $$PWD/../3rdparty/QtConfFile
DEPENDPATH += $$PWD/../3rdparty/QtConfFile

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/QtConfFile/lib/QtConfFile.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../3rdparty/QtConfFile/lib/libQtConfFile.a


