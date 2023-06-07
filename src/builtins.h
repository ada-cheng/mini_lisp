#ifndef BUILTINS_H
#define BUILTINS_H
# include "./value.h"
# include "./error.h"
# include <iostream>

using std::string;

ValuePtr add(const std::vector<ValuePtr>& args) {
        int result = 0;
       /*  std::cout<<"add"<<std::endl; */
        for (auto& arg : args) {
            if (typeid(*arg) == typeid(NumericValue)) {
                result += static_cast<NumericValue*>(arg.get())->value;
            } else {
                throw LispError("Cannot add a non-numeric value.");
            }
        }
       // std::cout<<result<<std::endl;
        return std::make_shared<NumericValue>(result);
}

ValuePtr print(const std::vector<ValuePtr>& args) {
    for (auto& arg : args) {
        std::cout << arg->toString() << std::endl;
    }
    return std::make_shared<NilValue>(NilValue());
}
#endif