TEMPLATE = subdirs

SUBDIRS = library aapp testutils testutilstests tests


library.subdir = alib

aapp.depends = library

testutilstests.depends = testutils

tests.subdir = atests
tests.depends = library testutils
