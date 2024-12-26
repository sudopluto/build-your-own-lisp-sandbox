#pragma once

#include <sstream>
#include <string>
#include <string.h>
#include <math.h>
#include "mpc/mpc.h"

#include "types.h"

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


class Evaluator {
private:

    LispValue eval_impl_op(char* op, LispValue l, LispValue r) {

        if (l.get_type() == LispValue::ValueType::lerr) {return l;}
        if (r.get_type() == LispValue::ValueType::lerr) {return r;}

        // must be value now

        if (strcmp(op, "+") == 0) {return LispValue(l.get_value() + r.get_value());}
        else if (strcmp(op, "-") == 0) {return l.get_value() - r.get_value();}
        else if (strcmp(op, "*") == 0) {return l.get_value() * r.get_value();}
        else if (strcmp(op, "/") == 0) {
            if (r.get_value() == 0) {
                return LispValue(LispValue::ErrorType::lerr_div_by_zero);
            } else {
                return l.get_value() / r.get_value();
            }
        }
        else if (strcmp(op, "%") == 0) {return l.get_value() % r.get_value();}
        else if (strcmp(op, "^") == 0) {return pow(l.get_value(), r.get_value());}
        else if (strcmp(op, "min") == 0) {return (l.get_value() < r.get_value()) ? l.get_value() : r.get_value();}
        else if (strcmp(op, "max") == 0) {return (l.get_value() > r.get_value()) ? l.get_value() : r.get_value();}
        else {return LispValue(LispValue::ErrorType::lerr_bad_op);}
    }

    LispValue eval_impl(const mpc_ast_t* ast) {
        if (strstr(ast->tag, "number")) {
            std::stringstream ss(ast->contents);
            long ret;
            ss >> ret;
            return !ss.fail() ? LispValue(ret) : LispValue(LispValue::ErrorType::lerr_bad_num);
        }
        else {
            char* op = ast->children[1]->contents;
            LispValue l = eval_impl(ast->children[2]);
            int i = 3;
            while (strstr(ast->children[i]->tag, "expr")) {
                l = eval_impl_op(op, l, eval_impl(ast->children[i]));
                ++i;
            }
            return l;
        }
    }


public:
    void eval(const mpc_ast_t* ast, std::string& output) {
        output.clear();
        if (ast) {
            LispValue eval_final = eval_impl(ast);
            if (eval_final.get_type() == LispValue::ValueType::lval) {
                output.append(std::to_string(eval_final.get_value()));
            } else {
                eval_final.print();
            }
        }
    }
};
