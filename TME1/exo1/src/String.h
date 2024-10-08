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
            bool operator<(const String & b);
    };

    size_t length(const char * str);
    char * newcpy(const char * str); 
    int compare(const String & s1, const String & s2);
    bool operator==(const String &a,const String &b); 

}