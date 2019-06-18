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

#ifndef ARGS_H_
#define ARGS_H_

#include <vector>
#include <QStringList>


namespace testutils {

    inline QStringList convertArgs(int argc, char *argv[]) {
        QStringList arguments;
        for(int i=0; i<argc; ++i) {
            arguments.append( argv[i] );
        }
        return arguments;
    }


    /**
     * @brief The Args class
     */
    class Args {

        std::vector<char*> vec;


    public:

        Args(const int argc, char** argv): vec() {
            for(int i=0; i<argc; ++i) {
                const char* str = argv[i];
                const std::size_t strLen = strlen(str);
                char* target = new char[ strLen+1 ];
                strcpy(target, str);
                target[strLen] = 0;
                vec.push_back( target );
            }
        }

        Args(const QStringList& args): vec() {
            for(int i=0; i<args.size(); ++i) {
                const std::string& str = args[i].toStdString();
                add(str);
            }
        }

        ~Args() {
            for(std::size_t i=0; i<vec.size(); ++i) {
                delete[] vec[i];
                vec.clear();
            }
        }

        int argc() const {
            return (int)vec.size();
        }

        char** argv() {
            return &(vec[0]);
        }

        void add(const std::string& str) {
            const std::size_t strLen = str.length();
            char* target = new char[ strLen+1 ];
            str.copy(target, strLen);
            target[strLen] = 0;
            vec.push_back( target );
        }

    };

}

#endif /* ARGS_H_ */
