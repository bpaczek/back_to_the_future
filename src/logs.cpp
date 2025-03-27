#include <iostream>
#include <sstream>
#include <string>
#include <stdbool.h>

#include <cstdlib>

static bool is_debug_print_enable(){
    return std::getenv("NAVTOR_DEBUG_LOG") == nullptr ? false : true;
}

template<typename... Args>
void debug_print(const std::string& format, Args... args) {
    if (!is_debug_print_enable()) {
        return;
    }
    std::ostringstream stream;
    ((stream << args << " "), ...);
    std::cout << format << ": " << stream.str() << std::endl;
}