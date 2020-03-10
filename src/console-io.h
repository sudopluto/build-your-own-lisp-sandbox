#pragma once

// returns true if was able to get input
bool console_getinput(std::string& input, const char* prompt);

#ifdef _WIN32
bool console_getinput(std::string& input, const char* prompt) {
    std::cout << prompt;
    std::string* line = new std::string;
    if (!std::getline(std::cin, *line)) {
        return false;
    }
    return true;
}

#else

#include <editline/readline.h>
bool console_getinput(std::string& input, const char* prompt) {
    char* line = readline(prompt);
    input.clear();
    if (line) {
        add_history(line);
        input.append(line);
    }
    free(line);
    // currently no way to error handle from readline
    return true;
}

#endif
