
TEMPLATE = subdirs

SUBDIRS =	CodeForWeb \
		GitStat \
		LicenseChange \
		Magnifier \
		3rdparty

binDir.commands = mkdir bin

QMAKE_EXTRA_TARGETS += binDir

OTHER_FILES =	README.md \
				LICENSE

Magnifier.depends = 3rdparty
