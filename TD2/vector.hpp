#include <assert.h>

namespace pr {
    template <typename T>
    class Vector {
        T * data;
        size_t sz;
        size_t capacity;

        /*
            -> push front : O(n)
            -> push front : for m push : m*ln(m)
            -> operator[] : O(1)
        */

        public:
            Vector(size_t cap=10) : sz(0), capacity(sz) {
                data = new T[capacity];
            }

            Vector(const Vector& v) {
                // TODO
            }

            ~Vector() {
                delete [] data;
            }

            Vector& operator= (const Vector& v) {
                // TODO
            }

            T& operator[](size_t index) {
                assert(sz < index);
                return data[index];
            }

            const T& operator[](size_t index) const {
                assert(sz < index)
                return data[index];
            }

            void push_back (const T& val) {
                if (sz == capacity) {
                    T * new_data = new T[capacity + 10];
                    for (int i = 0; i<capacity; ++i) {
                        new_data[i] = data[i];
                    }
                    delete [] data;
                    capacity += 10;
                    data = new_data;
                }
                data[sz++] = val;
            }

            size_t size() const {
                return sz;
            }

            bool empty() const {
                return sz == 0;
            }
    };
};