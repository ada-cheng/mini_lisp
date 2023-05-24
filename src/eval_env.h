#include "./value.h"
#include "./error.h"
#include <unordered_map>
using ValuePtr = std::shared_ptr<Value>;
using namespace std::literals; // 使用 s 后缀
std::unordered_map<std::string, ValuePtr> env;
class EvalEnv {
public:
    ValuePtr eval(ValuePtr expr);
};


ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (typeid(*expr) == typeid(BooleanValue)||typeid(*expr) == typeid(NumericValue)||typeid(*expr) == typeid(StringValue)) {
        return expr;
    } 
    if (typeid(*expr) == typeid(NilValue)) {
        throw LispError("Evaluating nil is prohibited.");
    }
    if (typeid(*expr) == typeid(PairValue))
    {
    std::vector<ValuePtr> v = static_cast<PairValue&>(*expr).toVector();
    if (v[0]->asSymbol() == "define"s) {
        if (auto name = v[1]->asSymbol()) {
        env[*name] = eval(v[2]);
        NilValue nil;
        return std::make_shared<NilValue>(nil);
}
else {
      throw LispError("Malformed define.");
    }


    }

   
}
if (auto name = expr->asSymbol())
{
    if (env.find(*name) != env.end()) {
        return env[*name];
    }
    else {
        throw LispError("Variable " + *name + " not defined.");
    }
}
 else {
        throw LispError("Unimplemented");
    }
}