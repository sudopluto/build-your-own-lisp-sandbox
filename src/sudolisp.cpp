#include <iostream>
#include <string>

// will return string allocated on heap, need to delete
std::string* console_getline(const std::string& prompt);

#ifdef _WIN32
std::string* console_getline(const std::string& prompt) {
    std::cout << prompt;
    std::string* line = new std::string;
    std::getline(std::cin, *line);
    return line;
}

#else

#include <editline/readline.h>
std::string* console_getline(const std::string& prompt) {
    char* line = readline(prompt.c_str());
    if (line) {
        add_history(line);
        std::string* ret = new std::string(line);
        free(line);
        return ret;
    }
    else {
        return new std::string("");
    }
}

#endif

int main(int argc, char** argv) {

    bool run = true;
    while(run) {
        std::string* line = console_getline("sudolisp> ");
        if (line->compare("quit") == 0) {
            run = false;
        }
        else {
            std::cout << " command not implemented: " << "\"" << *line << "\"" << std::endl;
        }
        delete line;
    }

    return 0;
}
