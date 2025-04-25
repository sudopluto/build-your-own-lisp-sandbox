#pragma once

#include <sstream>
#include <string>
#include <string.h>
#include <math.h>

#include "../types/types.h"
// TODO: typedef away mpc types so evaluator doesnt' know about mpc
#include "../parser/mpc/mpc.h"

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
