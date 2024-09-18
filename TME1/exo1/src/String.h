#include <cstring>  
#pragma once

namespace pr {
    class String {
        const char * data;

        public :
            String (const char * data);
            ~String() {
                delete [] data;
            }
    };

    size_t length(const char * str);
    char * newcpy(const char * str); 
}