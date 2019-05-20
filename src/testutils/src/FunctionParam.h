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

#ifndef FUNCTIONPARAM_H
#define FUNCTIONPARAM_H

#include <vector>
#include <QStringList>


namespace testutils {

    struct FunctionParam {
        enum Valid {
            V_None,
            V_Func,
            V_Class,
            V_Both
        };

        Valid state;
        QString className;
        QString funcName;

        FunctionParam(): state(V_None), className(""), funcName("") {
        }

        QString getPattern() const {
            switch(state) {
            case V_None:
                return "";       // invalid param
            case V_Class: {
                return className + "::*";
            }
            case V_Func: {
                return "*::" + funcName;
            }
            case V_Both: {
                return className + "::" + funcName;
            }
            }

            return "";      // invalid case
        }

        static FunctionParam createFromClass(const QString& className) {
            FunctionParam ret;
            ret.className = className;
            ret.state = V_Both;
            return ret;
        }
        static FunctionParam createFromFunction(const QString& funcName) {
            FunctionParam ret;
            ret.funcName = funcName;
            ret.state = V_Func;
            return ret;
        }
        static FunctionParam createFromBoth(const QString& className, const QString& funcName) {
            FunctionParam ret;
            ret.className = className;
            ret.funcName = funcName;
            ret.state = V_Both;
            return ret;
        }

        static std::vector<FunctionParam> splitFunctionsForQTest(const QStringList& functionsList) {
            std::vector<FunctionParam> ret;
            ret.reserve( (std::size_t)functionsList.size() );
            for(const QString& item: functionsList) {
                //commonArgs.removeAll(item);
                QStringList parts = item.split("::");
                if (parts.size() == 1) {
                    // no class given
                    FunctionParam param = FunctionParam::createFromFunction( parts[0] );
                    ret.push_back( param );
                } else if (parts.size() == 2) {
                    // with class and method
                    FunctionParam param = FunctionParam::createFromBoth( parts[0], parts[1] );
                    ret.push_back( param );
                }
            }
            return ret;
        }

        static std::vector<FunctionParam> splitFunctionsForQuickTest(const QStringList& functionsList) {
            std::vector<FunctionParam> ret;
            ret.reserve( (std::size_t)functionsList.size() );
            for(const QString& item: functionsList) {
                //commonArgs.removeAll(item);
                QStringList parts = item.split("::");
                if (parts.size() == 1) {
                    // no function given
                    FunctionParam param = FunctionParam::createFromClass( parts[0] );
                    ret.push_back( param );
                } else if (parts.size() == 2) {
                    // with class and method
                    FunctionParam param = FunctionParam::createFromBoth( parts[0], parts[1] );
                    ret.push_back( param );
                }
            }
            return ret;
        }

    };

}

#endif // FUNCTIONPARAM_H
