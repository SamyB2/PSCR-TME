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

    bool String::operator< (const String & b) {
        return compare(*this, b) < 0;
    }

    size_t length(const char * str) {
        return strlen(str);
    }

    char * newcpy(const char * str) {
        char * res = new char [length(str) + 1];
        memcpy(res, str, length(str) + 1);
        return res;
    }

    int compare(const String & s1, const String & s2) {
        size_t len1 = length(s1.data);
        size_t len2 = length(s2.data);
        if (len1 < len2) return -1;
        if (len1 > len2) return 1;
        for (size_t i = 0; i < len1; i++) {
            if (s1.data[i] < s2.data[i]) return -1;
            if (s1.data[i] > s2.data[i]) return 1;
        }
        return 0;
    }

    bool operator==(const String &a,const String &b) {
        return compare(a, b) == 0;
    }
}