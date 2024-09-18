#include "String.h"

namespace pr {
    String::String (const char * data) {
        this->data = newcpy(data);
    }

    size_t length(const char * str) {
        return strlen(str);
    }

    char * newcpy(const char * str) {
        char * res = new char [length(str) + 1];
        strcpy(res,str);
        return res;
    }
}