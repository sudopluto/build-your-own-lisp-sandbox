#pragma once

#include <string>

#include "mpc/mpc.h"

class Parser {
    mpc_parser_t* Number;
    mpc_parser_t* Operator;
    mpc_parser_t* Expr;
    mpc_parser_t* Lispy;

    mpc_ast_t* Ast;

public:
    Parser() : Ast(nullptr) {
        Number = mpc_new("number");
        Operator = mpc_new("operator");
        Expr = mpc_new("expr");
        Lispy = mpc_new("lispy");

        mpca_lang(MPCA_LANG_DEFAULT,
            " \
            number   : /-?[0-9]+/ ; \
            operator : '+' | '-' | '*' | '/' | '%'| '^' | \"min\" | \"max\"; \
            expr     : <number> | '(' <operator> <expr>+ ')' ; \
            lispy    : <number> | /^/ <operator> <expr>+ /$/; \
            ",
            Number, Operator, Expr, Lispy);
    }


    ~Parser() {
        mpc_cleanup(4, Number, Operator, Expr, Lispy);
        if (Ast) {
            mpc_ast_delete((Ast));
        }
    }

    void parse(const std::string& input) {
        mpc_result_t r;
        if (Ast) {
            mpc_ast_delete((Ast));
            Ast = nullptr;
        }

        if (mpc_parse("<stdin>", input.c_str(), Lispy, &r)) {
            Ast = (mpc_ast_t*)r.output;
        }
        else {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }
    }

    // temporary, as I don't really feel like writing a wrapper around ast
    // parser owns the memory for this, do not call delete / modify
    const mpc_ast_t* yield_ast() {
        return Ast;
    }
};
