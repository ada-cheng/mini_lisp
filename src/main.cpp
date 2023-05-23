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



int main() {
  
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
            EvalEnv env;
            auto result = env.eval(std::move(value));
            std::cout << result->toString() << std::endl;
        } catch (std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}
