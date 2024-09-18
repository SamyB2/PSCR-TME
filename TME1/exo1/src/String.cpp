#include "String.h"

namespace pr {
    String::String (const char * data) {
        this->data = newcpy(data);
    }

    String::String (const String & s) {
        this->data = newcpy(s.data);
    }

    String& String::operator= (const String & s) {
        if (this == &s) return *this;
        delete [] this->data;
        this->data = newcpy(s.data);
        return *this;
    }

    size_t length(const char * str) {
        return strlen(str);
    }

    char * newcpy(const char * str) {
        char * res = new char [length(str) + 1];
        memcpy(res, str, length(str) + 1);
        return res;
    }
}