#include <iostream>
#include "./value.h"
#include "./tokenizer.h"
#include "./token.h"
#include "./error.h"
#include <deque>

using ValuePtr = std::shared_ptr<Value>; 

class Parser {
    std::deque<TokenPtr> tokens;
public:
    Parser(std::deque<TokenPtr> tokens): tokens(std::move(tokens)) {};
    ValuePtr parse();
    ValuePtr parseTails();
};

ValuePtr Parser::parse() {
    auto token = std::move(tokens.front());
    // pop front
    tokens.pop_front();
  
    if (token->getType() == TokenType::NUMERIC_LITERAL) {
        auto value = static_cast<NumericLiteralToken&>(*token).getValue();
        return std::make_shared<NumericValue>(value);
    }
    if (token->getType() == TokenType::BOOLEAN_LITERAL) {
        auto value = static_cast<BooleanLiteralToken&>(*token).getValue();
        return std::make_shared<BooleanValue>(value);
    }
    if (token->getType() == TokenType::STRING_LITERAL) {
        auto value = static_cast<StringLiteralToken&>(*token).getValue();
        return std::make_shared<StringValue>(value);
    }
    if (token->getType() == TokenType::IDENTIFIER) {
        auto value = static_cast<IdentifierToken&>(*token).getName();
        return std::make_shared<SymbolValue>(value);
    }
    if (token->getType() == TokenType::LEFT_PAREN) {
        return this->parseTails();
    }
 
    if (token->getType() == TokenType::QUOTE) {
    return std::make_shared<PairValue>(
      std::make_shared<SymbolValue>("quote"),
      std::make_shared<PairValue>(
          this->parse(),
          std::make_shared<NilValue>()
      )
    );
}
    if (token->getType() == TokenType::QUASIQUOTE) {
    return std::make_shared<PairValue>(
      std::make_shared<SymbolValue>("quasiquote"),
      std::make_shared<PairValue>(
          this->parse(),
          std::make_shared<NilValue>()
      )
    );
}
if (token->getType() == TokenType::UNQUOTE) {
    return std::make_shared<PairValue>(
      std::make_shared<SymbolValue>("unquote"),
      std::make_shared<PairValue>(
          this->parse(),
          std::make_shared<NilValue>()
      )
    );
}

    throw SyntaxError("Unimplemented");
}
ValuePtr Parser::parseTails() {

    
    if (tokens.front()->getType() == TokenType::RIGHT_PAREN)
    {
        tokens.pop_front();
        return std::make_shared<NilValue>();
    }
    auto car = this->parse();
   
    if (tokens.front()->getType() == TokenType::DOT) {
        tokens.pop_front();
        auto cdr = this->parse();
        tokens.pop_front();
        return std::make_shared<PairValue>(car, cdr);
    } else {
    
        auto cdr = this->parseTails();
       
      return std::make_shared<PairValue>(car, cdr);
    }
    throw SyntaxError("Unimplemented");
}