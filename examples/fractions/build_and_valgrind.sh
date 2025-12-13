#!/bin/sh

# remember to edit Operators_and_Operands_definitions.h before building
# write #define OPERAND_VALUE_TYPE float

gcc fractions_implementation/fractions.c fractions_main.c -g3 -o ./bin/fractions_main
if [ $? -ne 0 ]; then
    echo "Build failed\n"
    exit $?
else
    echo "Build completed\n"
fi
valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./bin/fractions_main

