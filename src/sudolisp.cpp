#include <iostream>
#include <string>

#include "./console/console.h"
#include "./parser/parser.h"
#include "./evaluator/eval.h"

int main(int argc, char** argv) {
    std::cout << "sudolisp v1" << std::endl;
    std::cout << "press ctl-c or type \"quit\" to exit" << std::endl << std::endl;

    std::string input;
    std::string output;
    Parser p;
    Evaluator e;

    bool run = true;
    while(run) {
        input.clear();
        bool valid_input = console_getinput(input, "sudolisp> ");
        // handle bad input
        if (!valid_input) {
            std::cerr << "got invalid input" << std::endl;
            return 1;
        }
        // handle quiting
        else if (input.compare("quit") == 0) {
            std::cout << std::endl <<  "exiting..." << std::endl;
            return 0;
        }
        // got some input, parse and evaluate
        else {
           p.parse(input);
           e.eval(p.yield_ast(), output);
           std::cout << output << std::endl;
        }
    }
    return 0;
}
