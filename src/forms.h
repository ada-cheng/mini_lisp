#ifndef FORMS_H
#define FORMS_H

#include <unordered_map>

#include "./eval_env.h"
#include "./value.h"
using SpecialFormType = ValuePtr(const std::vector<ValuePtr>&, EvalEnv&);
extern  std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS;
#endif