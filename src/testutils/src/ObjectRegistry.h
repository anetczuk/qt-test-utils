#ifndef TYPEREGISTRY_H
#define TYPEREGISTRY_H

#include <vector>
#include <QSharedPointer>


namespace testutils {

    template <class ObjType>
    class ObjectRegistry {

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
