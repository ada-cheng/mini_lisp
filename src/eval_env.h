#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include <unordered_map>
#include <vector>
#include <string>
#include "./value.h"

using ValuePtr = std::shared_ptr<Value>;
using namespace std::literals; // 使用 s 后缀

class EvalEnv:public std::enable_shared_from_this<EvalEnv> {
    std::unordered_map<std::string, ValuePtr> env;
    
public:
    EvalEnv();
 

    ValuePtr eval(ValuePtr expr);
    
    std::vector<ValuePtr> evalList(ValuePtr expr);
    ValuePtr apply(ValuePtr proc, std::vector<ValuePtr> args, EvalEnv& env);
   
    std::shared_ptr<EvalEnv> parent=nullptr;
    // 定义成员函数 EvalEnv::lookupBinding
    ValuePtr lookupBinding(std::string name) {
        if (env.find(name) != env.end()) {

           // std::cout<<"find"<<name<<env[name]->toString()<<std::endl;
            return env[name];
        } else if (parent) {
          //  std::cout<<"parent"<<std::endl;
          
            return parent->lookupBinding(name);
        } else {
            return nullptr;
        }
    }
    // EvalEnv::defineBinding
    void defineBinding(std::string name, ValuePtr value) {
        env[name] = value;
    }
    std::shared_ptr<EvalEnv> createChild(const std::vector<std::string>& params, const std::vector<ValuePtr>& args) {
        auto child = std::shared_ptr<EvalEnv>(new EvalEnv());
        child->parent = shared_from_this();
        for (size_t i = 0; i < params.size(); i++) {
            child->defineBinding(params[i], args[i]);
           
        }
        return child;
    }
    
    void removeBinding(std::string name) {
        env.erase(name);
    }
   
};


#endif