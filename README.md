## qt-test-utils
Extensions to Qt testing framework

### Features:
- run all test cases in one executable (usefull e.g. in case of one tests subproject QtCreator)
- switch to old behaviour if needed


### Use:
For every source file with tests:
1. write standard Qt test case file
2. include *QTestRegister.h*

Only once:
- create "main" source file and call *QTEST_RUN_TESTS()* macro from *QTestRegister.h*


### Macros:
- *QTEST_REGISTER()* -- register test case for tests runner
- *QTEST_MAIN()* -- redefined standard macro to call *QTEST_REGISTER()*
- *QTEST_RUN_TESTS()* - tests runner -- generates *main()* function (to be used only once)


### Other things:
- *common.pri* contains example of function definition for qmake -- *linkStaticLibrary()* -- function demonstrates how to link static libraries
