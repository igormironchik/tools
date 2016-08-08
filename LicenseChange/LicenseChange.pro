
TEMPLATE = subdirs

SUBDIRS = LicenseChange \
	tests \
	utils

OTHER_FILES = README.md
	
LicenseChange.depends = utils
tests.depends = utils
