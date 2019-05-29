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

#include "QuickTestRunner.h"

#include "TestUtils.h"
#include "QmlUnitRunner.h"
#include "QuickTestRegister.h"
#include "Args.h"

#include <iostream>
#include <QProcess>
#include <QDebug>


using namespace testutils;


namespace quicktestutils {

    int run_tests(int argc, char *argv[], const char *sourceDir) {
        const QStringList arguments = convertArgs( argc, argv );
        return run_tests(arguments, sourceDir);
    }

    int run_tests(const QStringList& arguments, const char *sourceDir) {
        QuickTestsRunner runner;
        return runner.run_tests(arguments, sourceDir);
    }


    // ====================================================


    int QuickTestsRunner::run_tests(const QStringList& arguments, const char *sourceDir) {
        CmdParser cmdArgs(arguments);
        return run_tests(cmdArgs, sourceDir);
    }

    int QuickTestsRunner::run_tests(const CmdParser& arguments, const char *sourceDir) {
        bool runnerExecuted = false;
        int failures = 0;
        if (arguments.isRunnerQml()) {
            runnerExecuted = true;
            failures += execute_qml_tests(arguments, sourceDir);
        }

        if (arguments.isRunnerCpp()) {
            runnerExecuted = true;
            failures += execute_registered_tests(arguments);
        }

        if (runnerExecuted == false) {
            // spawn new processes with runners
            failures += spawn_qml_tests(arguments);

            if (arguments.isTextMode()) {
                std::cout << std::endl;
            }

            QuickTestsRegistry& testsRegistry = getTestsRegistry();
            const std::size_t regSize = testsRegistry.size();
            for(std::size_t i=0; i<regSize; ++i) {
                QmlSingleTestUnit* unit = testsRegistry.get(i);
                const std::string& unitId = unit->unitId();
                failures += spawn_registered_tests(arguments, unitId);
            }

            if (arguments.shouldShowSummary()) {
                std::cout << std::endl;
                std::cout << "********* Tests summary *********" << std::endl;
                std::cout << "      tests failures: " << failures << std::endl;
                std::cout << "********* Finished summary *********" << std::endl;
            }
        }
        return std::min(127, failures);
    }

    int QuickTestsRunner::spawn_qml_tests(const CmdParser& arguments) {
//        qDebug() << "spawn_qml_tests";
        CmdParser params( arguments );
        params.appendQmlRunner();
        QProcess qmlProcess;
        QStringList paramsList = params.list();
        const int failures = executeProcess(qmlProcess, paramsList, false);
        std::cout << qmlProcess.readAll().toStdString();
        return failures;
    }

    int QuickTestsRunner::execute_qml_tests(const CmdParser& arguments, const char *sourceDir) {
//        qDebug() << "execute_qml_tests";
        QmlUnitRunner runner;
        return runner.run(arguments, sourceDir);
    }

    int QuickTestsRunner::spawn_registered_tests(const CmdParser& arguments, const std::string& unit) {
//        qDebug() << "spawn_registered_tests";
        CmdParser params( arguments );
        params.appendCppUnit( unit.c_str() );
        QProcess qmlProcess;
        const QStringList& paramsList = params.list();
        const int failures = executeProcess(qmlProcess, paramsList, false);
        std::cout << qmlProcess.readAll().toStdString();
        return failures;
    }

    int QuickTestsRunner::execute_registered_tests(const CmdParser& arguments) {
        const std::string unitId = arguments.runnerUnit();
        if (unitId.empty()) {
            qDebug() << "invalid state - missing runner_unit parameter value";
            return 1;
        }
//        qDebug() << "execute_registered_tests:" << unitId.c_str();

        const QStringList params = arguments.list();

        // find and run unit
        int failures = 0;
        QuickTestsRegistry& testsRegistry = getTestsRegistry();
        const std::size_t regSize = testsRegistry.size();
        for(std::size_t i=0; i<regSize; ++i) {
            QmlSingleTestUnit* unit = testsRegistry.get(i);
            const std::string& uId = unit->unitId();
            if (unitId.compare(uId) == 0) {
//                qDebug() << "executing unit:" << uId.c_str();
                failures += unit->run(params);
            }
        }
        return failures;
    }

    QuickTestsRegistry& QuickTestsRunner::getTestsRegistry() {
        QuickTestsRegistry& testsRegistry = QuickTestsRegistry::get_tests_registry();
        return testsRegistry;
    }

}
