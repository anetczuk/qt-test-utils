/// MIT License
///
/// Copyright (c) 2019 Arkadiusz Netczuk <dev.arnet@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#ifndef SRC_ALIB_SRC_QTTESTREGISTER_H_
#define SRC_ALIB_SRC_QTTESTREGISTER_H_

#include <QApplication>
#include <QtTest/QtTest>
#include <QSharedPointer>


#ifndef EXEC_PER_TESTCASE

    // new behaviour -- run all tests in one executable

    #define QTEST_REGISTER( test_class )    static testutils::RegisterTestCaseInvocation<test_class> PPCAT(TEST_CASE, __LINE__);


    #define QTEST_RUN_TESTS()                                                                   \
                                QT_BEGIN_NAMESPACE                                              \
                                QTEST_ADD_GPU_BLACKLIST_SUPPORT_DEFS                            \
                                QT_END_NAMESPACE                                                \
                                int main(int argc, char *argv[]) {                              \
                                    QApplication app(argc, argv);                               \
                                    QTEST_DISABLE_KEYPAD_NAVIGATION                             \
                                    QTEST_ADD_GPU_BLACKLIST_SUPPORT                             \
                                    QTEST_SET_MAIN_SOURCE_PATH                                  \
                                    return testutils::run_registered_tests(argc, argv);         \
                                }

    // redefine standard macro to keep compatibile with old code
    #undef QTEST_MAIN
    #define QTEST_MAIN( test_class )        QTEST_REGISTER( test_class )

#else

    // old behaviour -- create executable per test case

    #define QTEST_REGISTER( test_class )    QTEST_MAIN( test_class )

    #define QTEST_RUN_TESTS()               // do nothing

#endif


#define PPCAT(A, B) PPCAT_NX(A, B)
#define PPCAT_NX(A, B) A ## B


namespace testutils {

    inline QRegularExpression prepare_regex(const QString& namePattern) {
        QString pattern = namePattern;
        pattern.replace("*", ".*");
        QRegularExpression re( pattern );
        return re;
    }

    void register_test_case(QObject* testCase);

    int run_registered_tests(int argc, char *argv[]);

    QStringList find_methods(const QObject* testCase, const QString& function);

    QStringList find_methods(const QObject* testCase, const QString& className, const QString& functionName);

    QStringList find_methods(const QObject* testCase, const QStringList& functions);

    QStringList find_methods(const QObject* testCase, const QStringList& classes, const QStringList& functions);

    QStringList extract_functions_from_arguments(const QStringList& arguments);

    bool should_show_summary();

    void print_summary(const int cppStatus, const int qmlStatus);

    inline QStringList convertArgs(int argc, char *argv[]) {
        QStringList arguments;
        for(int i=0; i<argc; ++i) {
            arguments.append( argv[i] );
        }
        return arguments;
    }


    class Args {

        std::vector<char*> vec;


    public:

        Args(const int argc, char** argv): vec() {
            for(int i=0; i<argc; ++i) {
                const char* str = argv[i];
                const std::size_t strLen = strlen(str);
                char* target = new char[ strLen+1 ];
                strcpy(target, str);
                target[strLen] = 0;
                vec.push_back( target );
            }
        }

        Args(const QStringList& args): vec() {
            for(int i=0; i<args.size(); ++i) {
                const std::string& str = args[i].toStdString();
                const std::size_t strLen = str.length();
                char* target = new char[ strLen+1 ];
                str.copy(target, strLen);
                target[strLen] = 0;
                vec.push_back( target );
            }
        }

        ~Args() {
            for(std::size_t i=0; i<vec.size(); ++i) {
                delete[] vec[i];
                vec.clear();
            }
        }

        int argc() const {
            return vec.size();
        }

        char** argv() {
            return &(vec[0]);
        }

    };

    // ======================================================


    template <class TC>
    class RegisterTestCaseInvocation {
        QSharedPointer<TC> testCase;

    public:

        RegisterTestCaseInvocation(): testCase( new TC() ) {
            register_test_case( testCase.data() );
        }

    };


    class TestsRegistry {

        std::vector<QObject*> casesList;
        bool showSummaryMode;


    public:

        TestsRegistry();

        virtual ~TestsRegistry() {
        }

        const QObject* operator [](const std::size_t index) const {
            return casesList[index];
        }

        QObject* operator [](const std::size_t index) {
            return casesList[index];
        }

        std::size_t size() const {
            return casesList.size();
        }

        bool should_show_summary() const {
            return showSummaryMode;
        }

        const QObject* get(const std::size_t index) const {
            return casesList[index];
        }

        QObject* get(const std::size_t index) {
            return casesList[index];
        }

        void push_back(QObject* testCase) {
            casesList.push_back(testCase);
        }

        int run_tests(const QStringList& arguments);


    protected:

        virtual int execute_test_case(QObject* testCase, const QStringList& arguments);

    };

}


#endif /* SRC_ALIB_SRC_QTTESTREGISTER_H_ */
