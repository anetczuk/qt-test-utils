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

#include "QtTest.h"
#include <iostream>

#include "Args.h"


namespace testutils {

    int run_registered_tests(int argc, char *argv[]) {
        QtTestsRegistry& testsRegistry = QtTestsRegistry::get_tests_registry();
        const QStringList arguments = convertArgs(argc, argv);

//        qDebug() << "arguments:" << arguments;
//        QFile ff("/tmp/aaa.log");
//        ff.open( QFile::Append );
//        for( auto& item: arguments) {
//            ff.write( item.toStdString().c_str() );
//            ff.write(" ");
//        }
//        ff.write("\n");
//        ff.close();

        const int numFailedTests = testsRegistry.run_tests(arguments);
        if (testsRegistry.should_show_summary() == false) {
            return numFailedTests;
        }
        std::cout << std::endl;
        std::cout << "********* Tests summary *********" << std::endl;
        std::cout << "      tests failures: " << numFailedTests << std::endl;
        std::cout << "********* Finished summary *********" << std::endl;
        return numFailedTests;
    }

}
