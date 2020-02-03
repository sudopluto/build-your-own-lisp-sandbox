#include <iostream>
#include <string>

#include "mpc/mpc.h"

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

class Parser {
    mpc_parser_t* Number;
    mpc_parser_t* Operator;
    mpc_parser_t* Expr;
    mpc_parser_t* Lispy;

public:
    Parser() {
        Number = mpc_new("number");
        Operator = mpc_new("operator");
        Expr = mpc_new("expr");
        Lispy = mpc_new("lispy");

        mpca_lang(MPCA_LANG_DEFAULT,
            " \
            number   : /-?[0-9]+/ ; \
            operator : '+' | '-' | '*' | '/' ; \
            expr     : <number> | '(' <operator> <expr>+ ')' ; \
            lispy    : /^/ <operator> <expr>+ /$/; \
            ",
            Number, Operator, Expr, Lispy);
    }


    ~Parser() {
        mpc_cleanup(4, Number, Operator, Expr, Lispy);
    }

    void print_parse(const std::string& input) {
        mpc_result_t r;
        if (mpc_parse("<stdin>", input.c_str(), Lispy, &r)) {
            mpc_ast_print((mpc_ast_t*)r.output);
            mpc_ast_delete((mpc_ast_t*)r.output);
        }
        else {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }   

    }
};

int main(int argc, char** argv) {
    std::cout << "sudolisp v1" << std::endl;
    std::cout << "press ctl-c or type \"quit\" to exit" << std::endl << std::endl;

    Parser p{};

    bool run = true;
    while(run) {
        std::string* line = console_getline("sudolisp> ");
        // got nullptr to line or we were requested to quit
        if (!line || line->compare("quit") == 0) {
            run = false;
        }
        // got an actual line, let's attempt to parse
        else {
           p.print_parse(*line); 
        }
        delete line;
    }

    return 0;
}
