#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <iterator>
#include <optional>
#include "./error.h"
using std::string;
using std::to_string;

class Value {
    public:
    virtual string toString() = 0;
    std::optional<std::string> asSymbol();

};
using ValuePtr = std::shared_ptr<Value>;

class StringValue : public Value {
public:
    string value;
    StringValue(string value) : value(value) {}
    string toString() {
        //return value with quotation marks
        std::ostringstream oss;
        oss << std::quoted(value);
        return oss.str();
    }
};
class BooleanValue : public Value {
public:
    bool value;
    BooleanValue(bool value) : value(value) {}
    string toString() {
        return value ? "#t" : "#f";
    }
};

class NumericValue : public Value {
public:
    double value;
    NumericValue(double value) : value(value) {}
    string toString() {
        if (value == (int)value)
        {
            return to_string((int)value);
        
        //保留6位小数
        }else{
            std::ostringstream oss;
            oss << std::setprecision(6) << value;
            return oss.str();
    }
}
};

class NilValue : public Value {
public:
    string toString() {
        return "()";
    }
};

class SymbolValue : public Value {
public:
    string value;

    SymbolValue(string value) : value(value) {}
    string toString() {
        return value;
    }
};

class PairValue : public Value {
public:
    std::shared_ptr<Value> car;
    std::shared_ptr<Value> cdr;
    PairValue(std::shared_ptr<Value> car, std::shared_ptr<Value> cdr) : car(car), cdr(cdr) {}
    /*
    PairValue(std::vector<ValuePtr> values) {
        if (values.size() == 0) {
            this->car = nullptr;
            this->cdr = nullptr;
        } else {
            this->car = values[0];
            if (values.size() == 1) {
                this->cdr = nullptr;
            } else {
                this->cdr = std::make_shared<PairValue>(std::vector<ValuePtr>(values.begin() + 1, values.end()));
            }
        }
    }
    */
    std::vector<ValuePtr> toVector() {
        std::vector<ValuePtr> result;
        PairValue *current = this;
        while (current != nullptr) {
            if (current->car != nullptr) {
                result.push_back(current->car);
            }
            if (current->cdr != nullptr && typeid(*current->cdr) == typeid(PairValue)) {
                current = static_cast<PairValue*>(current->cdr.get());
            } else {
                if (current->cdr != nullptr && typeid(*current->cdr) != typeid(NilValue)) {
                    result.push_back(current->cdr);
                }
                break;
            }
        }
        return result;
    }
  
    string toString() {
        string result = "(";
        PairValue *current = this;
        if (typeid(*current) == typeid(PairValue)) {
            auto& pair = static_cast<const PairValue&>(*current);
        // pair 是绑定到 const PairValue 的引用
        }
        while (current != nullptr) {
            if (current->car != nullptr) {
                result += current->car->toString();
              
            }
            if (current->cdr != nullptr && typeid(*current->cdr) == typeid(PairValue)) {
                 
                if (current->cdr->toString() != "()") {
                    result += " ";
                }
               
                current = static_cast<PairValue*>(current->cdr.get());
            
            } else {
                if (current->cdr != nullptr && current->cdr->toString() != "()")
                {
                    result += " . ";
                    result += current->cdr->toString();
                }
             
              
                break;
            }
        }
        result += ")";
        return result;
    }
   
};
class EvalEnv ;
class LambdaValue : public Value {
private:
    std::vector<std::string> params;
    std::vector<ValuePtr> body;
    std::shared_ptr<EvalEnv> env;
    // [...]
public:
    LambdaValue(std::vector<std::string> params, std::vector<ValuePtr> body, std::shared_ptr<EvalEnv> env) ;
    std::string toString() ;
    std::vector<std::string> getParams() const ;
    std::vector<ValuePtr> getBody() const ;
    std::shared_ptr<EvalEnv> getEnv() const ;
    ValuePtr apply(const std::vector<ValuePtr>& args);
    

   
};


using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr>&, EvalEnv&);
class BuiltinProcValue : public Value {

public:
    BuiltinFuncType* func;
    BuiltinProcValue(BuiltinFuncType* func) : func(func) {}
    std::string toString()  
    {
        return "#<procedure>";
    }
    ValuePtr proc (const std::vector<ValuePtr>& args, EvalEnv& env)
    {
 
        
        return func(args,env);
    }

    BuiltinFuncType* getFunc() const {
        return func;
    }


   
   
};

#endif