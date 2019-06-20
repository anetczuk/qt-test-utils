
## copy data dir
copydata.commands = $(COPY_DIR) $$PWD/data $$BUILD_PROJECT_SUBDIR
QMAKE_EXTRA_TARGETS += copydata
POST_TARGETDEPS += copydata


## create tests results dir
#resultsdir.target = $$OUT_PWD/tests
resultsdir.commands = $(MKDIR) $$BUILD_PROJECT_SUBDIR/tests/screens; $(MKDIR) $$BUILD_PROJECT_SUBDIR/tests/diff
QMAKE_EXTRA_TARGETS += resultsdir
POST_TARGETDEPS += resultsdir
