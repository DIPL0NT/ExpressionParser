#!/bin/bash

# remember to edit Operators_and_Operands_definitions.h before building
# add #include "examples/fractions/fractions_implementation/fractions.h"
# and write #define OPERAND_VALUE_TYPE fraction

gcc fractions_implementation/fractions.c fractions_main.c -o ./bin/fractions_main
if [ $? -ne 0 ]; then
    echo "Build failed"
    exit $?
else
    echo "Build completed"
fi
./bin/fractions_main
