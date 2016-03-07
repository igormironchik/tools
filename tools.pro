
TEMPLATE = subdirs

SUBDIRS =	CodeForWeb \
			GitStat

binDir.commands = mkdir bin

QMAKE_EXTRA_TARGETS += binDir

OTHER_FILES =	README.md \
				LICENSE
