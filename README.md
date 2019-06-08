## qt-test-utils
Extensions to Qt testing framework

### Features:
- running all QtTest tests cases in one executable (usefull e.g. in case of one tests subproject in QtCreator)
- calling QtTest and QtQuickTest tests by name using wildcard(*)
- loading/saving images from/to file directly from Qml
- comparing images and visualising difference between them from Qml


### Example of generated diff image

![Diff image](doc/diff_example.png "Diff image")

Example presents difference of two images. Result consists of four *subimages*:
- on upper left corner: test image
- upper right corner: reference image
- lower right corner: difference between two images
- lower left corner: mask of differences


### Use:
Import *testutils* subproject to projects tree (subdirs template).

For every source file with QtTest tests:
1. write standard Qt test case file
2. include *QTest.h* or *QuickTest.h*

Only once:
- for QtTest: create "main" source file and call *QTEST_RUN_TESTS()* macro from *QtTestRegister.h*
- for QtQuickTest: create "main" source and call *quicktestutils::run_tests()* runner from *QuickTestRegister.h*


### Examples:
- *testutils-qttests* - example subproject containing QtTest tests
- *testutils-quicktests* - example subproject containing QtQuickTest tests


### Macros:
- *QTEST_REGISTER()* -- register test case for tests runner
- *QTEST_MAIN()* -- redefined standard macro to call *QTEST_REGISTER()*
- *QTEST_RUN_TESTS()* - tests runner -- generates *main()* function (to be used only once)


### Other things:
- *common.pri* contains example of function definition for qmake -- *linkStaticLibrary()* -- function demonstrates how to link static libraries
- qmake function *generateResFile* from *common.pri* demonstrates how to dynamically generate resource file
- directory *autosubdirs* contains qmake functions and templates for automatic generation of subdirs projects for calling Qt tests in *executable per test* mode (default for Qt). *testutils-subdirtests* is example of use


### Known bugs:
- under Qt 5.9.5 calling Quick tests by test function name causes tests application to crash


## Appendix

### Requirements for detecting QtTests cases by QtCreator (version 4.9.0)
1. test case have to be implemented in source file (*.cpp)
2. tests file have to be added to *SOURCES* variable in project "pro" file
3. tests project have to include *testlib* library (added to *QT* variable)
4. tests file have to include one of: <QtTest> or "QtTest.h" (Qt standard header can be replaced by custom one)
5. test function have to be implemented as slot
6. test class have to be registered using *QTEST_MAIN* macro (macro can be redefined)
7. only one test class per file allowed (even in case of reimplementing the macro)

### Requirements for detecting QuickTests cases by QtCreator (version 4.9.0)
1. test case have to be implemented in qml file prefixed with "tst_" (tst_*.qml)
2. tests project have to include *qmltestcase* option (added to *CONFIG* variable in project "pro" file)
3. tests file have to be placed in the same directory or one of subfolders of project file
4. tests main file have to include one of: <QtQuickTest/QtQuickTest> or <QtQuickTest/quicktest.h>
5. main file have to use *QUICK_TEST_MAIN* macro (macro can be redefined)
6. only one test case component per file allowed 
