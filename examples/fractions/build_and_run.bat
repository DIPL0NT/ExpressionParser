:: remember to edit Operators_and_Operands_definitions.h before building
:: add #include "examples/fractions/fractions_implementation/fractions.h"
:: and write #define OPERAND_VALUE_TYPE fraction

@echo off

gcc .\fractions_implementation\fractions.c .\fractions_Operators_and_Operands_definitions.c .\fractions_main.c -o fractions_main.exe 
if %errorlevel% neq 0 (
    echo Build failed
    exit %errorlevel%
) else (
    echo Build completed
)
.\fractions_main.exe
