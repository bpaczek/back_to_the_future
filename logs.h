#ifndef _LOGS_H_
#define _LOGS_H_

#include <iostream>
#include <string>
#include <sstream>
#include <string>
#include <stdbool.h>
#include <cstdlib>


#define IS_DEBUGPRINT_ENABLE std::getenv("NAVTOR_DEBUG_LOG") == nullptr ? true : false

/* it may be move to the .tpp file */
template<typename... Args>
void debug_print(const std::string& format, Args... args) {
    if (IS_DEBUGPRINT_ENABLE == true) {
        return;
    }
    std::ostringstream stream;
    ((stream << args << " "), ...);
    std::cout << format << ": " << stream.str() << std::endl;
}

#endif