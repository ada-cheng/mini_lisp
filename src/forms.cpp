

#include <unordered_map>
#include "./value.h"
#include "./forms.h"

#include <vector>

ValuePtr listtt(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size()==0)
    {
        return std::make_shared<PairValue>(nullptr, nullptr);
    }
    if (args.size()==1)
    {
        return std::make_shared<PairValue>(args[0], nullptr);
    }
    else
    {
        return std::make_shared<PairValue>(args[0], listtt(std::vector<ValuePtr>(args.begin()+1, args.end()), env));
    }
}

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
    std::vector<ValuePtr> body(args.begin() + 2, args.end());
    return std::make_shared<LambdaValue>(param_names, body, environment.shared_from_this());
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
          
            std::vector<ValuePtr> body(args.begin() + 2, args.end());


     
           
            environment.defineBinding(*name, std::make_shared<LambdaValue>(param_names, body, environment.shared_from_this()));
            NilValue nil;
            return std::make_shared<NilValue>(nil);
        } else {
            throw LispError("Malformed define.");
        }
    } else
    if (auto name = args[1]->asSymbol()) {
       // add to environment
 
        environment.defineBinding(*name, environment.eval(args[2]));
     
        NilValue nil;
        return std::make_shared<NilValue>(nil);
    }   
    else {
      throw LispError("Malformed define.");
    }
  
}   

ValuePtr condForm(const std::vector<ValuePtr> &args, EvalEnv& env)
{
    ValuePtr res;
    if (args.size() == 1)
    {
        throw LispError("cond clause is empty");
    }
    std::vector<ValuePtr> vec;
    for (int i = 1;i<args.size();i++)
    {
        vec.push_back(args[i]);
    }
    for (auto ele : vec)
    {
        auto vec = static_cast<PairValue&>(*ele).toVector();
        
        if (vec[0]->toString() == "else")
        {
            res = std::make_shared<BooleanValue>(true);
            for (auto i = 1;i<vec.size();i++)
            {
                res = env.eval(vec[i]);
            }   
            return res;
        }
        auto condition = env.eval(vec[0]);
        if (condition->toString() != "#f")
        {
            res = env.eval(vec[0]);
            for (auto i = 1;i<vec.size();i++)
            {
                res = env.eval(vec[i]);
            }
            return res;
        }
       
    }
   
    return res;

}

ValuePtr beginForm(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    ValuePtr res;
    for (auto i = 1;i<args.size();i++)
    {
        res = env.eval(args[i]);
    }
    return res;
}


ValuePtr letForm(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size() < 3)
    {
        throw LispError("Malformed let.");
    }
    if (typeid(*args[1]) != typeid(PairValue))
    {
        throw LispError("Malformed let.");
    }
    auto params = static_cast<PairValue&>(*args[1]).toVector();
    std::vector<std::string> param_names;
    std::vector<ValuePtr> param_values;
    for (auto param : params)
    {
        auto name = static_cast<PairValue&>(*param).toVector()[0];
        auto value = static_cast<PairValue&>(*param).toVector()[1];
        //添加绑定
       // std::cout << "name:" << name->toString() << std::endl;
      //  std::cout << "value:" << value->toString() << std::endl;
        param_names.push_back(*name->asSymbol());
        param_values.push_back(env.eval(value));
    }
    std::vector<ValuePtr> body(args.begin() + 2, args.end());

    auto result = std::make_shared<LambdaValue>(param_names, body, env.shared_from_this());
  
 
    return  result->apply(param_values);
    
}



ValuePtr quasiquoteForm(const std::vector<ValuePtr> & args, EvalEnv& env)
{
   
    std::vector<ValuePtr> ans;
    std::vector<ValuePtr> vecc;
    vecc = static_cast<PairValue&>(*args[1]).toVector();
    for (auto expr : vecc)
    {
        if (typeid(*expr) == typeid(PairValue))
        {
            auto vec = static_cast<PairValue&>(*expr).toVector();
            
            if (vec[0]->toString() == "unquote" || vec[0]->toString()==",")
            {
                ans.push_back(env.eval(vec[1]));
            }
            else
            {
                ans.push_back(expr);
        }
        }
        else
        {
            ans.push_back(expr);
        }
    }
    return listtt(ans, env);
}
    




std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS{
    {"define", defineForm},
    {"quote", quoteForm},
    {"if", ifForm},
    {"and", andForm},
    {"or", orForm},
    {"lambda", lambdaForm},
    {"cond", condForm},
    {"begin", beginForm},
    {"let", letForm},
    {"quasiquote", quasiquoteForm},
    {"`", quasiquoteForm}
   
};