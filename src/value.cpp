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
#include "./value.h"
#include "./eval_env.h"
using std::string;
using std::to_string;

using ValuePtr = std::shared_ptr<Value>;


std::optional<std::string> Value::asSymbol()
{
    if (typeid(*this) == typeid(SymbolValue))
    {
            return this->toString();
    }
    return std::nullopt;
}
LambdaValue::LambdaValue(std::vector<std::string> params, std::vector<ValuePtr> body, std::shared_ptr<EvalEnv> env) : params(params), body(body), env(env) {}
std::string LambdaValue::toString()
{
    return "#<procedure>";
}
std::vector<std::string> LambdaValue::getParams() const
{
    return params;
}
std::vector<ValuePtr> LambdaValue::getBody() const
{
    return body;
}
std::shared_ptr<EvalEnv> LambdaValue::getEnv() const
{
    return env;
}

ValuePtr LambdaValue::apply(const std::vector<ValuePtr>& args)
{

    std::shared_ptr<EvalEnv> newEnv = this->env->createChild(this->params, args);
   
    std::vector<ValuePtr> body = this->body;
    ValuePtr result;
    for (auto i = body.begin(); i != body.end(); i++)
    {
        
        result = newEnv->eval(*i);
      
    }
    return result;
    
}

