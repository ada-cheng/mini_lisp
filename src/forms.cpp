

#include <unordered_map>
#include "./value.h"
#include "./forms.h"
#include <vector>



ValuePtr quoteForm(const std::vector<ValuePtr>& args, EvalEnv& environment) 
{
    return args[1];
}

//实现ifForm条件分支特殊形式

ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& environment)
{
    if (environment.eval(args[1])->toString()=="#f")
    {
       
        return environment.eval(args[3]);
    }
    else
    {
     
        return environment.eval(args[2]);
    }
}

ValuePtr andForm(const std::vector<ValuePtr>& args, EvalEnv& environment)
{
    if (args.size()==1 && args[0]->toString()=="and")
    {
        return std::make_shared<BooleanValue>(true);
    }   
    for (int i = 1;i<args.size();i++)
    {
        if (environment.eval(args[i])->toString()=="#f")
        {
            return std::make_shared<BooleanValue>(false);
        }
    }
    return environment.eval(args[args.size()-1]);
}

ValuePtr orForm(const std::vector<ValuePtr>& args, EvalEnv& environment)
{
    if (args.size() == 1 && args[0]->toString() == "or")
    {
        return std::make_shared<BooleanValue>(false);
    }
    for (int i = 1;i<args.size();i++)
    {
        auto res = environment.eval(args[i]);
        if (res->toString()!= "#f")
        {
            return res;
        }

    }
    return std::make_shared<BooleanValue>(false);

}

ValuePtr lambdaForm(const std::vector<ValuePtr>& args, EvalEnv& environment)
{
    if (args.size() < 3)
    {
        throw LispError("Malformed lambda.");
    }
    if (typeid(*args[1]) != typeid(PairValue))
    {
        throw LispError("Malformed lambda.");
    }
    auto params = static_cast<PairValue&>(*args[1]).toVector();
    std::vector<std::string> param_names;
    for (auto param : params)
    {
        if (auto name = param->asSymbol())
        {
            param_names.push_back(*name);
        }
        else
        {
            throw LispError("Malformed lambda.");
        }
    }
    auto body = static_cast<PairValue&>(*args[2]).toVector();
    return std::make_shared<LambdaValue>(param_names, body);
}

ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& environment) 
{
    //修改 define 特殊形式的定义，使得其支持 (define (f x) ...) 形式的变量定义。
    if (args.size() < 3) {
        throw LispError("Malformed define.");
    }
    if (typeid(*args[1]) == typeid(PairValue)) {
        auto params = static_cast<PairValue&>(*args[1]).toVector();
        if (params.size() < 1) {
            throw LispError("Malformed define.");
        }
        if (auto name = params[0]->asSymbol()) {
            std::vector<std::string> param_names;
            for (int i = 1; i < params.size(); i++) {
                if (auto name = params[i]->asSymbol()) {
                    param_names.push_back(*name);
                } else {
                    throw LispError("Malformed define.");
                }
            }
            auto body = static_cast<PairValue&>(*args[2]).toVector();
            environment.env[*name] = std::make_shared<LambdaValue>(param_names, body);
            NilValue nil;
            return std::make_shared<NilValue>(nil);
        } else {
            throw LispError("Malformed define.");
        }
    } else
    if (auto name = args[1]->asSymbol()) {
       // add to environment
       //do not use []
        environment.env[*name] = environment.eval(args[2]);
     
        NilValue nil;
        return std::make_shared<NilValue>(nil);
    }   
    else {
      throw LispError("Malformed define.");
    }
  
}   

std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS{
    {"define", defineForm},
    {"quote", quoteForm},
    {"if", ifForm},
    {"and", andForm},
    {"or", orForm},
    {"lambda", lambdaForm}
};