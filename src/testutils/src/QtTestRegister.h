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

#include <QRegularExpression>

#include "ObjectRegistry.h"
#include "CmdParser.h"


namespace testutils {

    QStringList find_methods(const QObject* testCase, const QString& function);

    QStringList find_methods(const QObject* testCase, const QString& className, const QString& functionName);

    QStringList find_methods(const QObject* testCase, const QStringList& functions);

    QStringList find_methods(const QObject* testCase, const QStringList& classes, const QStringList& functions);


    // ======================================================


    class QtTestsRegistry: public ObjectRegistry<QObject> {

        bool showSummaryMode;


    public:

        QtTestsRegistry();

        virtual ~QtTestsRegistry() {
        }

        bool should_show_summary() const {
            return showSummaryMode;
        }

        int run_tests(const QStringList& arguments);

        int run_tests(const testutils::CmdParser& arguments);

        static QtTestsRegistry& get_tests_registry();


    protected:

        virtual int execute_test_case(Type* testCase, const testutils::CmdParser& arguments);

    };


    template <class ObjType>
    class RegisterQtTest {
        QSharedPointer<ObjType> testCase;

    public:

        RegisterQtTest(): testCase( new ObjType() ) {
            QtTestsRegistry& testsRegistry = QtTestsRegistry::get_tests_registry();
            ObjType* ptr = testCase.data();
            testsRegistry.push_back(ptr);
        }

    };

}


#endif /* SRC_ALIB_SRC_QTTESTREGISTER_H_ */
