#pragma once

#include <iostream>
#include <cassert>

class LispValue {

public:
    enum class ValueType {lval, lerr};
    enum class ErrorType {lerr_div_by_zero, lerr_bad_op, lerr_bad_num};

private:
    union {
        long value;
        ErrorType err;
    };

   ValueType lval_type;

public:
    LispValue(long num) : value(num), lval_type(ValueType::lval) {};

    LispValue(ErrorType cons_err) : err(cons_err), lval_type(ValueType::lerr) {};

    ValueType get_type() { return lval_type; }

    // need to add panic for this to make sense, bandaid
    long get_value() {
        assert(lval_type == ValueType::lval);
        return value;
    }

    ErrorType get_error() {
        assert(lval_type == ValueType::lerr);
        return err;
    }

    void print() {
        if (lval_type == ValueType::lval) {
           std::cout << "Value: " << value << std::endl;
        } else {
            switch(err) {
                case ErrorType::lerr_div_by_zero: std::cout << "Error: Can't Divide by Zero!" << std::endl; break;
                case ErrorType::lerr_bad_op: std::cout << "Error: Invalid Operator!" << std::endl; break;
                case ErrorType::lerr_bad_num: std::cout << "Error: Invalid Number!" << std::endl; break;
            }
        }
    };
};
