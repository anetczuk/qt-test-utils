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

#include "CmdParser.h"

#include <set>

namespace testutils {

    static const std::set<std::string> NO_PARAMS_ARGS = {
                                                         /// logging options:
                                                         "-txt", "-cvs", "-xunitxml", "-lightxml", "-teamcity", "-tap",
                                                         /// log details options:
                                                         "-silent", "-v1", "-v2", "-vs",
                                                         /// testing options:
                                                         "-functions", "-datatags", "-nocrashhandler",
                                                         /// benchmarking options:
                                                         "-callgrind", "-perf", "-perfcounterlist", "-tickcounter", "-eventcounter", "-vb",
                                                         /// qmltest related options:
                                                         "-help"};

    static const std::set<std::string> ONE_PARAM_ARGS = {
                                                         /// logging options:
                                                         "-o",
                                                         /// testing options:
                                                         "-eventdelay", "-keydelay", "-mousedelay", "-maxwarnings",
                                                         /// benchmarking options:
                                                         "-perfcounter", "-minimumvalue", "-minimumtotal", "-iterations", "-median",
                                                         /// qmltest related options:
                                                         "-import", "-plugins", "-input"};


    static const std::set<std::string> NO_SUMMARY_OPTIONS = {"-csv", "-xml", "-xunitxml", "-lightxml", "-teamcity"};


    static const QString PARAM_RUNNER_QML( "-runner-qml" );             // flag, no value
    static const QString PARAM_RUNNER_CPP( "-runner-cpp" );             // flag, no value
    static const QString PARAM_RUNNER_UNIT( "-runner-unit" );           // param with value


    bool CmdParser::shouldShowSummary() const {
        for( const auto& item: arguments) {
            const auto pos_iter = NO_SUMMARY_OPTIONS.find( item.toStdString() );
            if (pos_iter != NO_SUMMARY_OPTIONS.end()) {
                return false;
            }
        }
        return true;
    }

    bool CmdParser::isRunnerQml() const {
        return arguments.contains( PARAM_RUNNER_QML );
    }

    bool CmdParser::isRunnerCpp() const {
        return arguments.contains( PARAM_RUNNER_CPP );
    }

    std::string CmdParser::runnerUnit() const {
        const int uIndex = arguments.indexOf( PARAM_RUNNER_UNIT );
        if (uIndex < 0) {
            // not found
            return "";
        }
        if (uIndex >= arguments.size()-1) {
            // last param -- without value
            return "";
        }
        return arguments[uIndex+1].toStdString();
    }

    void CmdParser::appendCppUnit(const std::string& unitId) {
        arguments.append( PARAM_RUNNER_CPP );
        arguments.append( PARAM_RUNNER_UNIT );
        arguments.append( unitId.c_str() );
    }

    void CmdParser::appendQmlRunner() {
        arguments.append( PARAM_RUNNER_QML );
    }

    QStringList CmdParser::extractFunctions() const {
        QStringList functions;

        const int aSize = arguments.size();
        for(int i=1; i<aSize; ++i) {
            const QString& arg = arguments[i];
            const std::string& stdArg = arg.toStdString();
            auto pos = NO_PARAMS_ARGS.find( stdArg );
            if (pos != NO_PARAMS_ARGS.end()) {
                /// known argument that does not have parameters
                continue;
            }
            if (arg.compare(PARAM_RUNNER_QML) == 0) {
                /// known argument that does not have parameters
                continue;
            }
            if (arg.compare(PARAM_RUNNER_CPP) == 0) {
                /// known argument that does not have parameters
                continue;
            }

            pos = ONE_PARAM_ARGS.find( stdArg );
            if (pos != ONE_PARAM_ARGS.end()) {
                /// known argument that does take one parameter
                ++i;
                continue;
            }
            if (arg.compare(PARAM_RUNNER_UNIT) == 0) {
                /// known argument that does take one parameter
                ++i;
                continue;
            }
            if (stdArg.length() < 1) {
                continue;
            }
            if (stdArg[0] == '-') {
                /// unknown argument -- assume no params arg
                continue;
            }

            /// unknown string -- assume function name
            functions.append( stdArg.c_str() );
        }

        return functions;
    }

    QStringList CmdParser::extractQmlParams() const {
        QStringList ret;

        const int aSize = arguments.size();
        for(int i=0; i<aSize; ++i) {
            const QString& arg = arguments[i];
            if (arg.compare(PARAM_RUNNER_UNIT) == 0) {
                /// known argument that does take one parameter
                ++i;
                continue;
            }

            /// unknown string -- assume function name
            const std::string& stdArg = arg.toStdString();
            ret.append( stdArg.c_str() );
        }

        ret.removeAll( PARAM_RUNNER_QML );
        ret.removeAll( PARAM_RUNNER_CPP );

        return ret;
    }
}
