#!/bin/sh

# remember to edit Operators_and_Operands_definitions.h before building
# write #define OPERAND_VALUE_TYPE float

gcc float_Operators_and_Operands_definitions.c float_main_ExpressionParser.c -lm -o float_main_ExpressionParser
if [ $? -ne 0 ]; then
    echo "Build failed\n"
    exit $?
else
    echo "Build completed\n"
fi
./float_main_ExpressionParser

