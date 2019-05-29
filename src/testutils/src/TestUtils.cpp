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

#include "TestUtils.h"

#include <set>
#include <sstream>
#include <QDebug>


namespace testutils {

    int executeProcess(QProcess& process, const QStringList& arguments, const bool failOnNonZeroExitCode) {
        QStringList params( arguments );
        const QString program = params[0];
        params.pop_front();
        return executeProcess( process, program, params, failOnNonZeroExitCode );
    }

    int executeProcess(QProcess& process, const QString& program, const QStringList& arguments, const bool failOnNonZeroExitCode) {
//        qDebug() << "Starting new process:" << program << arguments << failOnNonZeroExitCode;
        process.setProcessChannelMode(QProcess::MergedChannels);
        process.start(program, arguments);

        if (process.waitForFinished() == false) {
            std::stringstream message;
            message << "function subprocess could not finish: " << process.errorString().toStdString();
            throw std::runtime_error( message.str() );
        }

        const QProcess::ExitStatus exitStatus = process.exitStatus();
        if ( exitStatus != QProcess::NormalExit ) {
            std::stringstream message;
            message << "function subprocess finished with exit status: " << exitStatus;
            message << "\nsubprocess arguments:\n";
            for(const auto& arg: arguments) {
                message << arg.toStdString() << "\n";
            }
            message << "\nsubprocess output:\n" << process.readAll().toStdString();
            throw std::runtime_error( message.str() );
        }

        const int exitCode = process.exitCode();
        if ( failOnNonZeroExitCode && exitCode != 0 ) {
            std::stringstream message;
            message << "function subprocess finished with exit code: " << exitCode;
            message << "\nsubprocess arguments:\n";
            for(const auto& arg: arguments) {
                message << arg.toStdString() << "\n";
            }
            message << "\nsubprocess output:\n" << process.readAll().toStdString();
            throw std::runtime_error( message.str() );
        }
        return exitCode;
    }

}
