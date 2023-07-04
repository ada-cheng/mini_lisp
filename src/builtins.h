#ifndef BUILTINS_H
#define BUILTINS_H
# include "./value.h"
# include "./error.h"
# include <iostream>
# include "./eval_env.h"

using std::string;

ValuePtr add(const std::vector<ValuePtr>& args,EvalEnv& env) {
        double result = 0;
   
        for (auto& arg : args) {
            if (typeid(*arg) == typeid(NumericValue)) {
                result += static_cast<NumericValue*>(arg.get())->value;
               
            } else {
                throw LispError("Cannot add a non-numeric value.");
            }
        }
     
        return std::make_shared<NumericValue>(result);
}

ValuePtr print(const std::vector<ValuePtr>& args, EvalEnv& env) {
    for (auto& arg : args) {
        std::cout << arg->toString() << std::endl;
    }
    return std::make_shared<NilValue>(NilValue());
}

ValuePtr multiply(const std::vector<ValuePtr>& args, EvalEnv& env) {
    double result = 1;
    for (auto& arg : args) {
        if (typeid(*arg) == typeid(NumericValue)) {
            result *= static_cast<NumericValue*>(arg.get())->value;
        } else {
            throw LispError("Cannot multiply a non-numeric value.");
        }
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr greater(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size() != 2)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0]) != typeid(NumericValue) || typeid(*args[1]) != typeid(NumericValue))
    {
        throw LispError("Cannot compare non-numeric values.");
    }
    return std::make_shared<BooleanValue>(static_cast<NumericValue*>(args[0].get())->value > static_cast<NumericValue*>(args[1].get())->value);
}

ValuePtr substract(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size()==1)
    {
        if (typeid(*args[0]) != typeid(NumericValue))
        {
            throw LispError("Cannot substract non-numeric values.");
        }
        return std::make_shared<NumericValue>(-static_cast<NumericValue*>(args[0].get())->value);
    }
    if (args.size() != 2)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0]) != typeid(NumericValue) || typeid(*args[1]) != typeid(NumericValue))
    {
        throw LispError("Cannot compare non-numeric values.");
    }
    return std::make_shared<NumericValue>(static_cast<NumericValue*>(args[0].get())->value - static_cast<NumericValue*>(args[1].get())->value);
}

ValuePtr divide(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size() == 1)
    {
        if (typeid(*args[0]) != typeid(NumericValue))
        {
            throw LispError("Cannot divide non-numeric values.");
        }
        if (static_cast<NumericValue*>(args[0].get())->value == 0)
        {
            throw LispError("Cannot divide by zero.");
        }
        return std::make_shared<NumericValue>(1 / static_cast<NumericValue*>(args[0].get())->value);
    }
    if (args.size() != 2)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0]) != typeid(NumericValue) || typeid(*args[1]) != typeid(NumericValue))
    {
        throw LispError("Cannot compare non-numeric values.");
    }

    if (static_cast<NumericValue*>(args[1].get())->value == 0)
    {
        throw LispError("Cannot divide by zero.");
    }
    return std::make_shared<NumericValue>(static_cast<NumericValue*>(args[0].get())->value / static_cast<NumericValue*>(args[1].get())->value);
}


ValuePtr expt(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size() != 2)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0]) != typeid(NumericValue) || typeid(*args[1]) != typeid(NumericValue))
    {
        throw LispError("Cannot compare non-numeric values.");
    }

    if (static_cast<NumericValue*>(args[1].get())->value == 0 && static_cast<NumericValue*>(args[0].get())->value == 0)
    {
        throw LispError("Cannot raise zero to the power of zero.");
    }
    return std::make_shared<NumericValue>(pow(static_cast<NumericValue*>(args[0].get())->value, static_cast<NumericValue*>(args[1].get())->value));
}

ValuePtr quotient(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size() != 2)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0]) != typeid(NumericValue) || typeid(*args[1]) != typeid(NumericValue))
    {
        throw LispError("Cannot compare non-numeric values.");
    }

    if (static_cast<NumericValue*>(args[1].get())->value == 0)
    {
        throw LispError("Cannot divide by zero.");
    }
    return std::make_shared<NumericValue>(static_cast<int>(static_cast<NumericValue*>(args[0].get())->value / static_cast<NumericValue*>(args[1].get())->value));
}

ValuePtr modulo(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size() != 2)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0]) != typeid(NumericValue) || typeid(*args[1]) != typeid(NumericValue))
    {
        throw LispError("Cannot compare non-numeric values.");
    }

    if (static_cast<NumericValue*>(args[1].get())->value == 0)
    {
        throw LispError("Cannot divide by zero.");
    }
    auto result = (static_cast<int>(static_cast<NumericValue*>(args[0].get())->value)) % (static_cast<int>(static_cast<NumericValue*>(args[1].get())->value));
    if (result <0 && static_cast<NumericValue*>(args[1].get())->value > 0)
    {
        result += static_cast<int>(static_cast<NumericValue*>(args[1].get())->value);
    }
    if (result >0 && static_cast<NumericValue*>(args[1].get())->value < 0)
    {
        result += static_cast<int>(static_cast<NumericValue*>(args[1].get())->value);
    }

    return std::make_shared<NumericValue>(result);
}

ValuePtr abss(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size() != 1)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0]) != typeid(NumericValue))
    {
        throw LispError("Cannot compare non-numeric values.");
    }
    return std::make_shared<NumericValue>(abs(static_cast<NumericValue*>(args[0].get())->value));
}

ValuePtr remainderr(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size() != 2)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0])!=typeid(NumericValue) || typeid(*args[1]) != typeid(NumericValue))
    {
        throw LispError("Cannot compare non-numeric values.");
    }
    return std::make_shared<NumericValue>(std::remainder((static_cast<NumericValue*>(args[0].get())->value) , static_cast<int>(static_cast<NumericValue*>(args[1].get())->value)));
}

ValuePtr equal(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size() != 2)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0]) == typeid(NumericValue))
    {
        if (typeid(*args[1])!=typeid(NumericValue))
        {
            return std::make_shared<BooleanValue>(false);
        }
        else
        {
            return std::make_shared<BooleanValue>(static_cast<NumericValue*>(args[0].get())->value == static_cast<NumericValue*>(args[1].get())->value);
        }
    
    }
    if (typeid(*args[0])==typeid(BooleanValue))
    {
        if (typeid(*args[1])!=typeid(BooleanValue))
        {
            return std::make_shared<BooleanValue>(false);
        }
        else
        {
            return std::make_shared<BooleanValue>(static_cast<BooleanValue*>(args[0].get())->value == static_cast<BooleanValue*>(args[1].get())->value);
        }
    }
    if (typeid(*args[0]) == typeid(SymbolValue))
    {
        if (typeid(*args[1])!=typeid(SymbolValue))
        {
            return std::make_shared<BooleanValue>(false);
        }
        else
        {
            return std::make_shared<BooleanValue>(static_cast<SymbolValue*>(args[0].get())->value == static_cast<SymbolValue*>(args[1].get())->value);
        }
    }
    if (typeid(*args[0]) == typeid(NilValue))
    {
        if (typeid(*args[1])!=typeid(NilValue))
        {
            return std::make_shared<BooleanValue>(false);
        }
        else
        {
            return std::make_shared<BooleanValue>(true);
        }
    }
    if (typeid(*args[0])==typeid(StringValue))
    {
        if (typeid(*args[1])!=typeid(StringValue))
        {
            return std::make_shared<BooleanValue>(false);
        }
        else
        {
            return std::make_shared<BooleanValue>(static_cast<StringValue*>(args[0].get())->value == static_cast<StringValue*>(args[1].get())->value);
        }
    }
    if (typeid(*args[0]) == typeid(PairValue))
    {
        if (typeid(*args[1])!=typeid(PairValue))
        {
            return std::make_shared<BooleanValue>(false);
        }
        else
        {
            std::cout<<static_cast<PairValue*>(args[0].get())->toString()<<std::endl;
            return std::make_shared<BooleanValue>(static_cast<PairValue*>(args[0].get())->toString()==static_cast<PairValue*>(args[1].get())->toString());
        }
    }
    if (typeid(*args[0])==typeid(LambdaValue))
    {
        if (typeid(*args[1])!=typeid(LambdaValue))
        {
            return std::make_shared<BooleanValue>(false);
        }
        else
        {
            return std::make_shared<BooleanValue>(static_cast<LambdaValue*>(args[0].get())->getBody() == static_cast<LambdaValue*>(args[1].get())->getBody() && static_cast<LambdaValue*>(args[0].get())->getParams() == static_cast<LambdaValue*>(args[1].get())->getParams() && static_cast<LambdaValue*>(args[0].get())->getEnv() == static_cast<LambdaValue*>(args[1].get())->getEnv());
        }
    }

    if (typeid(*args[0])==typeid(BuiltinProcValue))
    {
        if (typeid(*args[1])!=typeid(BuiltinProcValue))
        {
            return std::make_shared<BooleanValue>(false);
        }
        else
        {
            return std::make_shared<BooleanValue>(static_cast<BuiltinProcValue*>(args[0].get())->getFunc() == static_cast<BuiltinProcValue*>(args[1].get())->getFunc());
        }
    }
}

ValuePtr eq(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size() != 2)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0]) == typeid(NumericValue))
    {
        if (typeid(*args[1])!=typeid(NumericValue))
        {
            return std::make_shared<BooleanValue>(false);
        }
        else
        {
            return std::make_shared<BooleanValue>(static_cast<NumericValue*>(args[0].get())->value == static_cast<NumericValue*>(args[1].get())->value);
        }
    
    }
    if (typeid(*args[0])==typeid(BooleanValue))
    {
        if (typeid(*args[1])!=typeid(BooleanValue))
        {
            return std::make_shared<BooleanValue>(false);
        }
        else
        {
            return std::make_shared<BooleanValue>(static_cast<BooleanValue*>(args[0].get())->value == static_cast<BooleanValue*>(args[1].get())->value);
        }
    }
    if (typeid(*args[0]) == typeid(SymbolValue))
    {
        if (typeid(*args[1])!=typeid(SymbolValue))
        {
            return std::make_shared<BooleanValue>(false);
        }
        else
        {
            return std::make_shared<BooleanValue>(static_cast<SymbolValue*>(args[0].get())->value == static_cast<SymbolValue*>(args[1].get())->value);
        }
    }
    if (typeid(*args[0]) == typeid(NilValue))
    {
        if (typeid(*args[1])!=typeid(NilValue))
        {
            return std::make_shared<BooleanValue>(false);
        }
        else
        {
            return std::make_shared<BooleanValue>(true);
        }
    }

    if (typeid(*args[0])==typeid(StringValue) || typeid(*args[0])==typeid(PairValue) || typeid(*args[0])==typeid(LambdaValue) || typeid(*args[0])==typeid(BuiltinProcValue))
    {
        return std::make_shared<BooleanValue>(args[0]==args[1]);
    }

    return std::make_shared<BooleanValue>(false);
   
}

ValuePtr nott(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size() != 1)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0]) == typeid(BooleanValue))
    {
        if (static_cast<BooleanValue*>(args[0].get())->toString() == "#f")
        {
            return std::make_shared<BooleanValue>(true);
        }
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr numericequal(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size() != 2)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0]) == typeid(NumericValue))
    {
        if (typeid(*args[1])!=typeid(NumericValue))
        {
            return std::make_shared<BooleanValue>(false);
        }
        else
        {
            return std::make_shared<BooleanValue>(static_cast<NumericValue*>(args[0].get())->value == static_cast<NumericValue*>(args[1].get())->value);
        }
    
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr less(const std::vector<ValuePtr> &args, EvalEnv &env)
{
    if (args.size() != 2)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0]) == typeid(NumericValue))
    {
        if (typeid(*args[1])!=typeid(NumericValue))
        {
            return std::make_shared<BooleanValue>(false);
        }
        else
        {
            return std::make_shared<BooleanValue>(static_cast<NumericValue*>(args[0].get())->value < static_cast<NumericValue*>(args[1].get())->value);
        }
    
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr lessequal(const std::vector<ValuePtr>& args, EvalEnv & env)
{
    if (args.size() != 2)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0]) == typeid(NumericValue))
    {
        if (typeid(*args[1])!=typeid(NumericValue))
        {
            return std::make_shared<BooleanValue>(false);
        }
        else
        {
            return std::make_shared<BooleanValue>(static_cast<NumericValue*>(args[0].get())->value <= static_cast<NumericValue*>(args[1].get())->value);
        }
    
    }
}

ValuePtr greaterequal(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size() != 2)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0]) == typeid(NumericValue))
    {
        if (typeid(*args[1])!=typeid(NumericValue))
        {
            return std::make_shared<BooleanValue>(false);
        }
        else
        {
            return std::make_shared<BooleanValue>(static_cast<NumericValue*>(args[0].get())->value >= static_cast<NumericValue*>(args[1].get())->value);
        }
    
    }
}

ValuePtr even(const std::vector<ValuePtr> & args, EvalEnv& env)
{
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0])!=typeid(NumericValue))
    {
        throw LispError("Wrong type of arguments.");
    }
    if (static_cast<NumericValue*>(args[0].get())->value == static_cast<int>(static_cast<NumericValue*>(args[0].get())->value) )
    {
        if (static_cast<int>(static_cast<NumericValue*>(args[0].get())->value)%2==0)
        {
            return std::make_shared<BooleanValue>(true);
        }
        else
        {
            return std::make_shared<BooleanValue>(false);
        }
        
    }
    else
    {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr odd(const std::vector<ValuePtr> &args, EvalEnv& env)
{
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0])!=typeid(NumericValue))
    {
        throw LispError("Wrong type of arguments.");
    }
    if (static_cast<NumericValue*>(args[0].get())->value == static_cast<int>(static_cast<NumericValue*>(args[0].get())->value) )
    {
        if (static_cast<int>(static_cast<NumericValue*>(args[0].get())->value)%2!=0)
        {
            return std::make_shared<BooleanValue>(true);
        }
        else
        {
            return std::make_shared<BooleanValue>(false);
        }
        
    }
    else
    {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr zero(const std::vector<ValuePtr> &args, EvalEnv& env)
{
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0])!=typeid(NumericValue))
    {
        throw LispError("Wrong type of arguments.");
    }
    if (static_cast<NumericValue*>(args[0].get())->value == 0)
    {
        return std::make_shared<BooleanValue>(true);
    }
    else
    {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr atom(const std::vector<ValuePtr> &args, EvalEnv& env)
{
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }
    
    if (typeid(*args[0])==typeid(BooleanValue) || typeid(*args[0])==typeid(NumericValue) || typeid(*args[0])==typeid(StringValue) || typeid(*args[0])==typeid(SymbolValue) || typeid(*args[0])==typeid(NilValue))
    {
      
        return std::make_shared<BooleanValue>(true);
    }
    else
    {
        return std::make_shared<BooleanValue>(false);
    }
    
}

ValuePtr boolean(const std::vector<ValuePtr> & args, EvalEnv& env)
{
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0])!=typeid(BooleanValue))
    {
        return std::make_shared<BooleanValue>(false);
    }
    else
    {
        return std::make_shared<BooleanValue>(true);
    }
}

ValuePtr integer(const std::vector<ValuePtr> & args, EvalEnv& env)
{
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0])!=typeid(NumericValue))
    {
        return std::make_shared<BooleanValue>(false);
    }
    else
    {
        if (static_cast<NumericValue*>(args[0].get())->value == static_cast<int>(static_cast<NumericValue*>(args[0].get())->value))
        {
            return std::make_shared<BooleanValue>(true);
        }
        else
        {
            return std::make_shared<BooleanValue>(false);
        }
        
    }
}

ValuePtr list(const std::vector<ValuePtr> & args, EvalEnv& env)
{
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0])==typeid(NilValue))
    {
        return std::make_shared<BooleanValue>(true);
    }
    if (typeid(*args[0])!=typeid(PairValue))
    {
        return std::make_shared<BooleanValue>(false);
    }
    else
    {
       
        if (static_cast<PairValue*>(args[0].get())->toVector().size()<2)
        {
            return std::make_shared<BooleanValue>(true);
        }
        if (typeid(*static_cast<PairValue*>(args[0].get())->cdr)==typeid(PairValue))
        {
            return std::make_shared<BooleanValue>(true);
        }
        return std::make_shared<BooleanValue>(false);
    }
    
}

ValuePtr number(const std::vector<ValuePtr> & args, EvalEnv& env)
{
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0])!=typeid(NumericValue))
    {
        return std::make_shared<BooleanValue>(false);
    }
    else
    {
        return std::make_shared<BooleanValue>(true);
    }
}

ValuePtr null(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (args[0]->toString()=="()")
    {
        return std::make_shared<BooleanValue>(true);
    }
    else
    {
        return std::make_shared<BooleanValue>(false);
    }
}

ValuePtr pair(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0])!=typeid(PairValue))
    {
        return std::make_shared<BooleanValue>(false);
    }
    else
    {
        return std::make_shared<BooleanValue>(true);
    }
}

ValuePtr procedure(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0])!=typeid(LambdaValue) && typeid(*args[0])!=typeid(BuiltinProcValue))
    {
        return std::make_shared<BooleanValue>(false);
    }
    else
    {
        return std::make_shared<BooleanValue>(true);
    }
    
}

ValuePtr sstring(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0])!=typeid(StringValue))
    {
        return std::make_shared<BooleanValue>(false);
    }
    else
    {
        return std::make_shared<BooleanValue>(true);
    }
}

ValuePtr symbol(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0])!=typeid(SymbolValue))
    {
        return std::make_shared<BooleanValue>(false);
    }
    else
    {
        return std::make_shared<BooleanValue>(true);
    }
}

ValuePtr applyy(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size()!=2)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0])!=typeid(LambdaValue) && typeid(*args[0])!=typeid(BuiltinProcValue))
    {
        throw LispError("Wrong type of arguments.");
    }
    if (typeid(*args[1])!=typeid(PairValue))
    {
        throw LispError("Wrong type of arguments.");
    }
    else
    {
        std::vector<ValuePtr> newArgs;
        for (auto i:static_cast<PairValue*>(args[1].get())->toVector())
        {
            newArgs.push_back(i);
        }
        if (typeid(*args[0])==typeid(BuiltinProcValue))
        {
            return static_cast<BuiltinProcValue*>(args[0].get())->proc(newArgs, env);
        }
        return static_cast<LambdaValue*>(args[0].get())->apply(newArgs);
    }

}

ValuePtr display(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0])==typeid(StringValue))
    {
        std::cout<<static_cast<StringValue*>(args[0].get())->value;
    }
    else
    {       
        std::cout<<"'"<<args[0]->toString();

    }
    std::cout<<std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr displayln(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0])==typeid(StringValue))
    {
        std::cout<<static_cast<StringValue*>(args[0].get())->value<<std::endl;
    }
    else
    {       
        std::cout<<"'"<<args[0]->toString()<<std::endl;
    }
    return std::make_shared<NilValue>();
}



ValuePtr errorr(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size()==0)
    {
        throw LispError("Error.");
    }
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }
    throw LispError(static_cast<StringValue*>(args[0].get())->value);
}

ValuePtr evall(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }
    return env.eval(args[0]);
}

ValuePtr exitt(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size()==0)
    {
        exit(0);
    }
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }
    exit(static_cast<NumericValue*>(args[0].get())->value);
}

ValuePtr newlinee(const std::vector<ValuePtr>& args,EvalEnv& env)
{
    std::cout<<std::endl;
    return std::make_shared<NilValue>(NilValue());
}

/*
ValuePtr printt(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    std::cout<<"'";
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }
    std::cout<<args[0]->toString();
    std::cout<<std::endl;
    return std::make_shared<NilValue>();
}
*/

ValuePtr listt(const std::vector<ValuePtr>& args, EvalEnv& env)
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
        return std::make_shared<PairValue>(args[0], listt(std::vector<ValuePtr>(args.begin()+1, args.end()), env));
    }
}

ValuePtr append(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    
    std::vector<ValuePtr> newArgs;
    for (auto i:args)
    {
        if (typeid(*i)!=typeid(PairValue) && typeid(*i)!=typeid(NilValue))
        {
            throw LispError("Wrong type of arguments.");
        }
        if (typeid(*i)==typeid(NilValue))
        {
            continue;
        }
        for (auto j:static_cast<PairValue*>(i.get())->toVector())
        {
            newArgs.push_back(j);
         
        }
    }
    
    return listt(newArgs, env);
}
ValuePtr car(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }

    if (typeid(*args[0])!=typeid(PairValue))
    {
        throw LispError("Wrong type of arguments.");
    }
    return static_cast<PairValue*>(args[0].get())->car;
}

ValuePtr cdr(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }

    if (typeid(*args[0])!=typeid(PairValue))
    {
        throw LispError("Wrong type of arguments.");
    }
    if (static_cast<PairValue*>(args[0].get())->cdr==nullptr)
    {
        return std::make_shared<NilValue>();
    }
    return static_cast<PairValue*>(args[0].get())->cdr;
}

ValuePtr cons(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size()!=2)
    {
        throw LispError("Wrong number of arguments.");
    }
    return std::make_shared<PairValue>(args[0], args[1]);
}

ValuePtr length(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    if (args.size()!=1)
    {
        throw LispError("Wrong number of arguments.");
    }
    if (typeid(*args[0]) == typeid(NilValue))
    {
        return std::make_shared<NumericValue>(0);
    }

    if (typeid(*args[0])!=typeid(PairValue))
    {
        throw LispError("Wrong type of arguments.");
    }
    auto llist = static_cast<PairValue*>(args[0].get());
    return std::make_shared<NumericValue>((llist)->toVector().size());
}


ValuePtr mapp(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    auto proc = args[0];
    if (typeid(*proc)==typeid(BuiltinProcValue))
    {
    auto newproc = static_cast<BuiltinProcValue*>(proc.get());
    std::vector<ValuePtr> newArgs;
    for (int i=1; i<args.size(); i++)
    {
        if (typeid(*args[i])!=typeid(PairValue))
        {
            throw LispError("Wrong type of arguments.");
        }
        for (auto j:static_cast<PairValue*>(args[i].get())->toVector())
        {
            newArgs.push_back(j);
        }
    }
    std::vector<ValuePtr> result;
    for (auto i:newArgs)
    {
        result.push_back(newproc->proc({i}, env));
    }
    return listt(result,env);
    }
    
    else if (typeid(*proc)==typeid(LambdaValue))
    {
        auto newproc = static_cast<LambdaValue*>(proc.get());
        std::vector<ValuePtr> newArgs;
        for (int i=1; i<args.size(); i++)
        {
            if (typeid(*args[i])!=typeid(PairValue))
            {
                throw LispError("Wrong type of arguments.");
            }
            for (auto j:static_cast<PairValue*>(args[i].get())->toVector())
            {
                newArgs.push_back(j);
            }
        }
        std::vector<ValuePtr> result;
        for (auto i:newArgs)
        {
            result.push_back(newproc->apply({i}));
        }
        return listt(result,env);
    }
    else
    {
        throw LispError("Wrong type of arguments.");
    }
    
}
ValuePtr filter(const std::vector<ValuePtr> & args, EvalEnv & env)
{
    auto proc = args[0];
    if (typeid(*proc)==typeid(BuiltinProcValue))
    {
    auto newproc = static_cast<BuiltinProcValue*>(proc.get());
    std::vector<ValuePtr> newArgs;
    for (int i=1; i<args.size(); i++)
    {
        if (typeid(*args[i])==typeid(NilValue))
        {
            continue;
        }
        if (typeid(*args[i])!=typeid(PairValue))
        {
            throw LispError("Wrong type of arguments.");
        }
        for (auto j:static_cast<PairValue*>(args[i].get())->toVector())
        {
            newArgs.push_back(j);
        }
    }
    std::vector<ValuePtr> result;
    for (auto i:newArgs)
    {
        if (static_cast<BooleanValue*>(newproc->proc({i}, env).get())->value)
        {
            result.push_back(i);
        }
    }
    return listt(result,env);
    }
    else if (typeid(*proc)==typeid(LambdaValue))
    {
        auto newproc = static_cast<LambdaValue*>(proc.get());
        std::vector<ValuePtr> newArgs;
        for (int i=1; i<args.size(); i++)
        {
            if (typeid(*args[i])==typeid(NilValue))
            {
                continue;
            }
            if (typeid(*args[i])!=typeid(PairValue))
            {
                throw LispError("Wrong type of arguments.");
            }
            for (auto j:static_cast<PairValue*>(args[i].get())->toVector())
            {
                newArgs.push_back(j);
            }
        }
        std::vector<ValuePtr> result;
        for (auto i:newArgs)
        {
            if (static_cast<BooleanValue*>(newproc->apply({i}).get())->value)
            {
                result.push_back(i);
            }
        }
        return listt(result,env);
    }
    else
    {
        throw LispError("Wrong type of arguments.");
    }

}


ValuePtr reduce(const std::vector<ValuePtr>& args, EvalEnv& env)
{
    auto proc = args[0];
    if (typeid(*proc)==typeid(BuiltinProcValue))
    {
    auto newproc = static_cast<BuiltinProcValue*>(proc.get());
    std::vector<ValuePtr> newArgs;
    for (int i=1; i<args.size(); i++)
    {
        if (typeid(*args[i])!=typeid(PairValue))
        {
            throw LispError("Wrong type of arguments.");
        }
        for (auto j:static_cast<PairValue*>(args[i].get())->toVector())
        {
            newArgs.push_back(j);
        }
    }
    ValuePtr result = newArgs[0];
    if (newArgs.size()==1)
    {
        return newproc->proc({result}, env);
    }

    for (int i=1; i<newArgs.size(); i++)
    {
        result = newproc->proc({result, newArgs[i]}, env);
    }
    return result;
    }
    else if (typeid(*proc)==typeid(LambdaValue))
    {
        auto newproc = static_cast<LambdaValue*>(proc.get());
        std::vector<ValuePtr> newArgs;
        
        for (int i=1; i<args.size(); i++)
        {
            if (typeid(*args[i])!=typeid(PairValue))
            {
                throw LispError("Wrong type of arguments.");
            }
            for (auto j:static_cast<PairValue*>(args[i].get())->toVector())
            {
                newArgs.push_back(j);
            }
        }
        ValuePtr result = newArgs[0];
        if (newArgs.size()==1)
        {
            return newproc->apply({result});
        }
        for (int i=1; i<newArgs.size(); i++)
        {
            result = newproc->apply({result, newArgs[i]});
        }
        return result;
    }
    else
    {
        throw LispError("Wrong type of arguments.");
    }
}


#endif