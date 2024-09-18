#include <cstring>  
#pragma once

namespace pr {
    class String {

        public :
        const char * data;
            String (const char * data="");
            String (const String & s);
            ~String() {
                delete [] data;
            }
            String& operator=(const String & s);    
    };

    size_t length(const char * str);
    char * newcpy(const char * str); 
}