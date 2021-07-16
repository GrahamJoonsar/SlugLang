@echo off
echo Starting Build
:: Building for Debug, Not including libraries
if %1 == debug g++ src/slug.cpp src/slugInterpreter.cpp -o slug.exe
:: Building for release, using libraries
:: for windows, dunno about other os
if %1 == release g++ -03 -static-libgcc -static-libstdc++ src/slug.cpp src/slugInterpreter.cpp -o slug.exe
echo Build Complete