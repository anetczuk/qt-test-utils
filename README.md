## qt-test-utils
Extensions to Qt testing framework

### Features:
- run all QtTest tests cases in one executable (usefull e.g. in case of one tests subproject in QtCreator)
- switch to old behaviour if needed (define EXEC_PER_TESTCASE)
- calling QtTest and QtQuickTest tests by name supports wildcard(*)


### Use:
Import *testutils* subproject to projects tree (subdirs template).

For every source file with QtTest tests:
1. write standard Qt test case file
2. include *QTestRegister.h*

Only once:
- for QtTest: create "main" source file and call *QTEST_RUN_TESTS()* macro from *QtTestRegister.h*
- for QtQuickTest: create "main" source and call *quicktestutils::run_tests()* runner from *QuickTestRegister.h*


### Macros:
- *QTEST_REGISTER()* -- register test case for tests runner
- *QTEST_MAIN()* -- redefined standard macro to call *QTEST_REGISTER()*
- *QTEST_RUN_TESTS()* - tests runner -- generates *main()* function (to be used only once)


### Other things:
- *common.pri* contains example of function definition for qmake -- *linkStaticLibrary()* -- function demonstrates how to link static libraries
