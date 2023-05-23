#include "./value.h"


using ValuePtr = std::shared_ptr<Value>;
class EvalEnv {
public:
    ValuePtr eval(ValuePtr expr);
};


ValuePtr EvalEnv::eval(ValuePtr expr) {
    if (typeid(*expr) == typeid(BooleanValue)||typeid(*expr) == typeid(NumericValue)||typeid(*expr) == typeid(StringValue)) {
        return expr;
    } 
    else if (typeid(*expr) == typeid(NilValue)) {
        throw LispError("Evaluating nil is prohibited.");
    } else {
        throw LispError("Unimplemented");
    }
}