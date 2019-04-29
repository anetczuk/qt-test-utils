TEMPLATE = subdirs

SUBDIRS = library testutils testutilstests tests


library.subdir = alib

tests.subdir = atests
tests.depends = library testutils
