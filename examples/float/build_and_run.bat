:: remember to edit Operators_and_Operands_definitions.h before building
:: write #define OPERAND_VALUE_TYPE float

@echo off

gcc .\float_Operators_and_Operands_definitions.c .\float_main_ExpressionParser.c -o float_main_ExpressionParser.exe
if %errorlevel% neq 0 (
    echo Build failed
    exit %errorlevel%
) else (
    echo Build completed
)
.\float_main_ExpressionParser.exe

