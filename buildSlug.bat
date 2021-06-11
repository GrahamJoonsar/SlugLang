@echo off
echo Starting Build
:: Building for Debug, Not including libraries
if %1 == debug g++ src/slug.cpp src/slugInterpreter.cpp -o build/slug.exe
:: Building for release, using libraries
:: for windows, dunno about other os
if %1 == release g++ -static-libgcc -static-libstdc++ src/slug.cpp src/slugInterpreter.cpp -o build/slug.exe
echo Build Complete
:: Paths might be different if you cloned the repository