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

#include "QTestRegister.h"


namespace testutils {

    typedef std::vector<QObject*> TestsRegistry;


    TestsRegistry& registered_tests() {
        static std::vector<QObject*> list;
        return list;
    }

    // ======================================================

    void register_test_case(QObject* testCase) {
        TestsRegistry& tests = registered_tests();
        tests.push_back(testCase);
    }

    int run_registered_tests(int argc, char *argv[]) {
        TestsRegistry& tests = registered_tests();
        if (tests.size() < 1) {
            qWarning() << "no registered tests found";
        }

        int status = 0;
        for (auto it = tests.begin(); it != tests.end(); ++it) {
            status += QTest::qExec(*it, argc, argv);
        }
        return std::min(status, 127);
    }

}
