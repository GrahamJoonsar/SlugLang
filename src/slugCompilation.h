#pragma once

#include "slugInterpreter.h"

extern std::unordered_map<std::string, std::string (*)(std::vector<std::string>, Interpreter*)> compedLines;

extern void initCompilation();