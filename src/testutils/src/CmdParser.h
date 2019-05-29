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

#ifndef CMD_PARSER_H_
#define CMD_PARSER_H_

#include <QStringList>
#include <QDebug>


namespace testutils {

    class CmdParser {
    protected:

        QStringList arguments;


    public:

        CmdParser() {
        }

        explicit CmdParser(const QStringList& args): arguments(args) {
        }

        CmdParser(const CmdParser& args): arguments(args.arguments) {
        }

        const QStringList& list() const {
            return arguments;
        }

        bool contains(const QString& arg) const {
            return arguments.contains( arg );
        }

        bool shouldShowSummary() const;

        bool isTextMode() const {
            return shouldShowSummary();
        }

        bool isRunnerQml() const;

        bool isRunnerCpp() const;

        std::string runnerUnit() const;

        void push_back(const QString& arg) {
            arguments.append( arg );
        }

        void append(const QString& arg) {
            arguments.append( arg );
        }

        void append(const QStringList& list) {
            arguments.append( list );
        }

        void appendCppUnit(const std::string& unitId);

        void appendQmlRunner();

        int removeAll(const QString& arg) {
            return arguments.removeAll(arg);
        }

        int removeAll(const QStringList& argList) {
            int ret = 0;
            for(const QString& item: argList) {
                ret += removeAll(item);
            }
            return ret;
        }

        QStringList extractFunctions() const;

        QStringList extractQmlParams() const;

        QString toQString() const {
            if (arguments.length() < 1)
                return "";
            QString ret;
            ret.append( arguments[0] );
            const int aSize = arguments.size();
            for(int i = 1; i<aSize; ++i) {
                const QString& item = arguments[i];
                ret.append(" ");
                ret.append(item);
            }
            return ret;
        }
    };


    inline QDebug operator<< (QDebug d, const CmdParser& data) {
        d << data.list();
        return d;
    }

}


#endif /* CMD_PARSER_H_ */
