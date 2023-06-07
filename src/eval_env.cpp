#include "./eval_env.h"
#include "./value.h"
#include "./error.h"
#include "./builtins.h"
#include <unordered_map>
#include <algorithm>
#include <iterator>
#include <ranges>
#include "./forms.h"

using ValuePtr = std::shared_ptr<Value>;
using namespace std::literals; // 使用 s 后缀

std::vector<ValuePtr> EvalEnv::evalList(ValuePtr expr) {
    std::vector<ValuePtr> result;
    PairValue* p = static_cast<PairValue*>(expr.get());
  /*   std::cout<<p->toString()<<std::endl;
    std::cout<<p->toVector().size()<<std::endl; */
    std::ranges::transform(p->toVector(),
                           std::back_inserter(result),
                           [this](ValuePtr v) { return this->eval(v); });
    
    return result;
}

ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr> args) {
 /*    std::cout<<proc->toString()<<std::endl;
    std::cout<<typeid(*proc).name()<<std::endl; */
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        // 调用内置过程

    //   std::cout<<"builtin"<<std::endl;
        return static_cast<BuiltinProcValue*>(proc.get())->proc(args);
    } else {
      //  std::cout<<args[0]->toString()<<std::endl;
        throw LispError("Unimplemented");
    }
}


EvalEnv::EvalEnv()
{
    env["+"] = std::make_shared<BuiltinProcValue>(&add);
    env["print"] = std::make_shared<BuiltinProcValue>(&print);
}
ValuePtr EvalEnv::eval(ValuePtr expr) {
 /*    std::cout<<expr->toString()<<std::endl; */
    if (typeid(*expr) == typeid(BooleanValue)||typeid(*expr) == typeid(NumericValue)||typeid(*expr) == typeid(StringValue)) {
        return expr;
    } 
   
    if (typeid(*expr) == typeid(PairValue) )
    {
    /*     std::cout << "pair" << std::endl; */
    std::vector<ValuePtr> v = static_cast<PairValue&>(*expr).toVector();
    
    if (v[0]->asSymbol())
    {
        if (SPECIAL_FORMS.find(*v[0]->asSymbol()) != SPECIAL_FORMS.end()) {
      
        return SPECIAL_FORMS.at(*v[0]->asSymbol())(v, *this);
    }

    /*
    if (v[0]->asSymbol() == "define"s) {
        if (auto name = v[1]->asSymbol()) {
        env[*name] = eval(v[2]);
   
        NilValue nil;
        return std::make_shared<NilValue>(nil);
    }   
    
    else {
      throw LispError("Malformed define.");
    }
    */

    
    else
    {
      /*   std::cout<<"else"<<std::endl; */
        ValuePtr proc = this->eval(v[0]);
        std::vector<ValuePtr> args;
        if (v.size() > 1)
        {
        args = this->evalList(static_cast<PairValue&>(*expr).cdr);
        }
        else
        {
            args = {};
        }
      /*  std::cout<<args.size()<<std::endl; */
      return this->apply(proc, args);
    }
  
    }
   
    }
    if (typeid(*expr) == typeid(BuiltinProcValue)) {

        return expr;
    }
    if (typeid(*expr) == typeid(NilValue)) {
        throw LispError("Evaluating nil is prohibited.");
    }
if (auto name = expr->asSymbol())
{
    //std::cout<<env.size()<<std::endl;
    if (env.find(*name) != env.end()) {
        return env[*name];
    }
    else {
        throw LispError("Variable " + *name + " not defined.");
    }
}
// std::cout<<expr->toString()<<std::endl;

        throw LispError("Unimplemented");
    
}
