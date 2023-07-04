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
 
    std::ranges::transform(p->toVector(),
                           std::back_inserter(result),
                           [this](ValuePtr v) { return this->eval(v); });
    
    return result;
}

ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr> args, EvalEnv& env) {

  
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        // 调用内置过程

   
        return static_cast<BuiltinProcValue*>(proc.get())->proc(args, env);
    } 
    else if (typeid(*proc) == typeid(LambdaValue) )
    {
      
       LambdaValue* l = static_cast<LambdaValue*>(proc.get());
       return l->apply(args);
         
    }
    else {
       
       throw LispError("Unimplemented");
    }
}


EvalEnv::EvalEnv()
{
    env["+"] = std::make_shared<BuiltinProcValue>(&add);
    env["print"] = std::make_shared<BuiltinProcValue>(&print);
    env["*"] = std::make_shared<BuiltinProcValue>(&multiply);
    env[">"] = std::make_shared<BuiltinProcValue>(&greater);
    env["-"] = std::make_shared<BuiltinProcValue>(&substract);
    env["/"] = std::make_shared<BuiltinProcValue>(&divide);
    env["abs"] = std::make_shared<BuiltinProcValue>(&abss);
    env["expt"] = std::make_shared<BuiltinProcValue>(&expt);
    env["quotient"] = std::make_shared<BuiltinProcValue>(&quotient);
    env["modulo"] = std::make_shared<BuiltinProcValue>(&modulo);
    env["remainder"] = std::make_shared<BuiltinProcValue>(&remainderr);
    env["eq?"] = std::make_shared<BuiltinProcValue>(&eq);
    env["equal?"] = std::make_shared<BuiltinProcValue>(&equal);
    env["not"] = std::make_shared<BuiltinProcValue>(&nott);
    env["="] = std::make_shared<BuiltinProcValue>(&numericequal);
    env["<"] = std::make_shared<BuiltinProcValue>(&less);
    env["<="] = std::make_shared<BuiltinProcValue>(&lessequal);
    env[">="] = std::make_shared<BuiltinProcValue>(&greaterequal);
    env["even?"] = std::make_shared<BuiltinProcValue>(&even);
    env["odd?"] = std::make_shared<BuiltinProcValue>(&odd);
    env["zero?"] = std::make_shared<BuiltinProcValue>(&zero);
    env["atom?"] = std::make_shared<BuiltinProcValue>(&atom);
    env["boolean?"] = std::make_shared<BuiltinProcValue>(&boolean);
    env["integer?"] = std::make_shared<BuiltinProcValue>(&integer);
    env["list?"] = std::make_shared<BuiltinProcValue>(&list);
    env["number?"] = std::make_shared<BuiltinProcValue>(&number);
    env["null?"] = std::make_shared<BuiltinProcValue>(&null);
    env["pair?"] = std::make_shared<BuiltinProcValue>(&pair);
    env["procedure?"] = std::make_shared<BuiltinProcValue>(&procedure);
    env["string?"] = std::make_shared<BuiltinProcValue>(&sstring);
    env["symbol?"] = std::make_shared<BuiltinProcValue>(&symbol);
    
    env["apply"] = std::make_shared<BuiltinProcValue>(&applyy);
    
    env["display"] = std::make_shared<BuiltinProcValue>(&display);
    env["displayln"] = std::make_shared<BuiltinProcValue>(&displayln);
    
    env["error"] = std::make_shared<BuiltinProcValue>(&errorr);
    env["list"] = std::make_shared<BuiltinProcValue>(&listt);
    env["append"] = std::make_shared<BuiltinProcValue>(&append);
    env["car"] = std::make_shared<BuiltinProcValue>(&car);
    env["cdr"] = std::make_shared<BuiltinProcValue>(&cdr);
    env["cons"] = std::make_shared<BuiltinProcValue>(&cons);
    env["length"] = std::make_shared<BuiltinProcValue>(&length);
    env["map"] = std::make_shared<BuiltinProcValue>(&mapp);
    env["filter"] = std::make_shared<BuiltinProcValue>(&filter);
    env["reduce"] = std::make_shared<BuiltinProcValue>(&reduce);
    env["eval"] = std::make_shared<BuiltinProcValue>(&evall);
    env["exit"] = std::make_shared<BuiltinProcValue>(&exitt);
    env["newline"] = std::make_shared<BuiltinProcValue>(&newlinee);
    //env["newline"] = std::make_shared<BuiltinProcValue>(&newlinee);
    /*
   
    
    env["newline"] = std::make_shared<BuiltinProcValue>(&newline);
    env["print"] = std::make_shared<BuiltinProcValue>(&printt);
  
    */
    
   
   





}
ValuePtr EvalEnv::eval(ValuePtr expr) {

    if (typeid(*expr) == typeid(BooleanValue)||typeid(*expr) == typeid(NumericValue)||typeid(*expr) == typeid(StringValue)) {
        return expr;
    } 
   
    if (typeid(*expr) == typeid(PairValue) )
    {

    std::vector<ValuePtr> v = static_cast<PairValue&>(*expr).toVector();
    
    if (v[0]->asSymbol())
    {
        if (SPECIAL_FORMS.find(*v[0]->asSymbol()) != SPECIAL_FORMS.end()) {
      // make sure this is a shared ptr
      


        return SPECIAL_FORMS.at(*v[0]->asSymbol())(v, *this);
    }
     else
    {
       
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
       
    

      return this->apply(proc, args, *this);
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
    }

    
    else
    {

        ValuePtr proc = this->eval(v[0]);
        if (proc->asSymbol())
        {
            if (SPECIAL_FORMS.find(*proc->asSymbol()) != SPECIAL_FORMS.end()) {
        // make sure this is a shared ptr
        return SPECIAL_FORMS.at(*proc->asSymbol())(v, *this);
    }
        }
        else
        {

   
        std::vector<ValuePtr> args;
        if (v.size() > 1)
        {
        args = this->evalList(static_cast<PairValue&>(*expr).cdr);
        //std::cout<<"size"<<args.size()<<std::endl;
        }
        else
        {
            args = {};
        }
       // std::cout<<"proc"<<proc->toString()<<std::endl;

      return this->apply(proc, args, *this);
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

    //using lookupBinding 
    if (this->lookupBinding(*name)) {
        return this->lookupBinding(*name);
    }
    else {
        throw LispError("Variable " + *name + " not defined.");
    }

    /*

    if (env.find(*name) != env.end()) {
        return env[*name];
    }
    else {
        throw LispError("Variable " + *name + " not defined.");
    }
    */
}


        throw LispError("Unimplemented");
    
}
