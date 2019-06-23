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

#ifndef TYPEREGISTRY_H
#define TYPEREGISTRY_H

#include <vector>


namespace testutils {

    template <class ObjType>
    class ObjectRegistry {
    protected:

        std::vector<ObjType*> objectsList;


    public:

        typedef ObjType Type;


        ObjectRegistry() {
        }

        const ObjType* operator [](const std::size_t index) const {
            return objectsList[index];
        }

        ObjType* operator [](const std::size_t index) {
            return objectsList[index];
        }

        std::size_t size() const {
            return objectsList.size();
        }

        const ObjType* get(const std::size_t index) const {
            return objectsList[index];
        }

        ObjType* get(const std::size_t index) {
            return objectsList[index];
        }

        void push_back(ObjType* testCase) {
            objectsList.push_back(testCase);
        }

    };

}

#endif // TYPEREGISTRY_H
