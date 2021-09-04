@echo off
echo Starting Build
:: Building for Debug, Not including libraries
if %1 == debug g++ -Wall src/slug.cpp src/slugInterpreter.cpp src/evaluations.cpp src/interpFunctions.cpp src/VariableStack.cpp src/loops.cpp -o bin/slug.exe
:: Building for release, using libraries
:: for windows, dunno about other os
if %1 == release g++ -03 -static-libgcc -static-libstdc++ src/slug.cpp src/VariableStack.cpp src/loops.cpp src/slugInterpreter.cpp -o bin/slug.exe
echo Build Complete