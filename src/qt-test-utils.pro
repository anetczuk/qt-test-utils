TEMPLATE = subdirs

SUBDIRS = library testutils tests


library.subdir = alib

tests.subdir = atests
tests.depends = library testutils
