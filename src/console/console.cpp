#include <iostream>

#include "console.h"

bool console_getinput(std::string& input, const char* prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
        return false;
    }
    input.append(line);
    return true;
}
