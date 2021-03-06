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
#ifndef SRC_ALIB_SRC_QMLUNITRUNNER_H_
#define SRC_ALIB_SRC_QMLUNITRUNNER_H_

#include "CmdParser.h"


namespace quicktestutils {

    QStringList get_functions(const QStringList& args);

    QStringList find_methods(const QStringList& testCaseFunctions, const QString& runFunction);

    QStringList find_methods(const QStringList& testCaseFunctions, const QStringList& functionPatterns);


    // ====================================================================


    class QmlUnitRunner {
    public:

        std::string testsName;


        QmlUnitRunner(): testsName("qmltests") {
        }

        QmlUnitRunner(const std::string& name): testsName(name) {
        }

        virtual ~QmlUnitRunner() {
        }

        int run(const QStringList& arguments, const char *sourceDir = nullptr, QObject* setup = nullptr);

        int run(const testutils::CmdParser& arguments, const char *sourceDir = nullptr, QObject* setup = nullptr);


    protected:

        virtual int executeQml(const testutils::CmdParser& arguments, const char *sourceDir, QObject* setup);

        virtual QStringList getFunctionsList(const testutils::CmdParser& arguments) const;

    };

}


#endif /* SRC_ALIB_SRC_QMLUNITRUNNER_H_ */
