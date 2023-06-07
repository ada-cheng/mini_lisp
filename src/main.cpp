#include <iostream>
#include <string>
#include "./value.h"
#include "./tokenizer.h"
#include "./token.h"
#include "./error.h"
#include "./parser.h"
#include "./eval_env.h"
#include "rjsj_test.hpp"
using ValuePtr = std::shared_ptr<Value>;

struct TestCtx {
    EvalEnv env;
    std::string eval(std::string input) {
        auto tokens = Tokenizer::tokenize(input);
        Parser parser(std::move(tokens));
        auto value = parser.parse();
        auto result = env.eval(std::move(value));
        return result->toString();
    }
};

int main() {
     
        
       RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5);
         EvalEnv env;
    while (true) {
        try {
          
            std::cout << ">>> " ;
            std::string line;
            std::getline(std::cin, line);
            if (std::cin.eof()) {
                std::exit(0);
            }
            auto tokens = Tokenizer::tokenize(line);
            Parser parser(std::move(tokens));
            auto value = parser.parse();
           
            auto result = env.eval(std::move(value));
            std::cout << result->toString() << std::endl;
        } catch (std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}
