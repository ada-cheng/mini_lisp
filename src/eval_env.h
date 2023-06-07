#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include <unordered_map>
#include <vector>
#include <string>
#include "./value.h"

using ValuePtr = std::shared_ptr<Value>;
using namespace std::literals; // 使用 s 后缀

class EvalEnv {
public:
    ValuePtr eval(ValuePtr expr);
    EvalEnv();
    std::vector<ValuePtr> evalList(ValuePtr expr);
    ValuePtr apply(ValuePtr proc, std::vector<ValuePtr> args);
    std::unordered_map<std::string, ValuePtr> env;

   
};


#endif