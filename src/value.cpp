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