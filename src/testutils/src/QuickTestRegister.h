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

#ifndef SRC_ALIB_SRC_QUICKTESTREGISTER_H_
#define SRC_ALIB_SRC_QUICKTESTREGISTER_H_

#include <QStringList>
#include <QSharedPointer>

#include "ObjectRegistry.h"


namespace quicktestutils {

    class QmlSingleTestUnit {
    protected:

        std::string unit;       // path
        QObject* setupObj;


    public:

        QmlSingleTestUnit(const std::string& unitPath): unit(unitPath), setupObj(nullptr) {
        }

        QmlSingleTestUnit(const std::string& rootPath, const std::string& subPath): unit(rootPath), setupObj(nullptr) {
            unit.append(subPath);
        }

        virtual ~QmlSingleTestUnit() {
        }

        const std::string& unitId() const {
            return unit;
        }

        void attachSetupObject(QObject& setup) {
            setupObj = &setup;
        }

        void attachSetupObject(QObject* setup) {
            setupObj = setup;
        }

        virtual int run(const QStringList& arguments) {
            return executeUnit(arguments);
        }

        int executeUnit(const QStringList& arguments);

    };


    // ======================================================


    class QuickTestsRegistry: public testutils::ObjectRegistry<QmlSingleTestUnit> {
    public:

        QuickTestsRegistry();

        virtual ~QuickTestsRegistry() {
        }

        static QuickTestsRegistry& get_tests_registry();

    };


    template <class ObjType>
    class RegisterTestUnit {
        QSharedPointer<ObjType> testCase;

    public:

        RegisterTestUnit(): testCase( new ObjType() ) {
            QuickTestsRegistry& testsRegistry = QuickTestsRegistry::get_tests_registry();
            ObjType* ptr = testCase.data();
            testsRegistry.push_back(ptr);
        }

    };

}


#endif /* SRC_ALIB_SRC_QUICKTESTREGISTER_H_ */
