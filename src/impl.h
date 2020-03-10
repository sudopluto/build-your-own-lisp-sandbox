#pragma once

#include <string>
#include <string.h>
#include <math.h>
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

    void parse(const std::string& input, bool print = false) {
        mpc_result_t r;
        if (mpc_parse("<stdin>", input.c_str(), Lispy, &r)) {
            if (print) { 
                mpc_ast_print((mpc_ast_t*)r.output);
            }
            Ast = (mpc_ast_t*)r.output;
        }
        else {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
            Ast = nullptr;
        }   
    }

    // temporary, as I don't really feel like writing a wrapper around ast
    // parser owns the memory for this, do not call delete / modify
    const mpc_ast_t* yield_ast() {
        return Ast;         
    }
};


class Evaluator {
private:

    long eval_impl_op(char* op, long x, long y) {
        if (strcmp(op, "+") == 0) {return x + y;}
        else if (strcmp(op, "-") == 0) {return x - y;}
        else if (strcmp(op, "*") == 0) {return x * y;}
        else if (strcmp(op, "/") == 0) {return x / y;}
        else if (strcmp(op, "%") == 0) {return x % y;}
        else if (strcmp(op, "^") == 0) {return pow(x, y);}
        else if (strcmp(op, "min") == 0) {return (x < y) ? x : y;}
        else if (strcmp(op, "max") == 0) {return (x > y) ? x : y;}
        else {return x;}
    }

    long eval_impl(const mpc_ast_t* ast) {
        if (strstr(ast->tag, "number")) {
            return atol(ast->contents);
        }
        else {
            char* op = ast->children[1]->contents;
            long x = eval_impl(ast->children[2]);
            int i = 3;
            while (strstr(ast->children[i]->tag, "expr")) {
                x = eval_impl_op(op, x, eval_impl(ast->children[i]));
                ++i;
            }
            return x;
        }
    }


public:
    void eval(const mpc_ast_t* ast, std::string& output) {
        output.clear();
        if (ast) {
            output.append(std::to_string(eval_impl(ast)));
        }
    }
};
