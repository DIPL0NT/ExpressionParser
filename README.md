IMPORTANT!: Read comments in Operators_and_Operands_definitions.h
            and edit Operators_and_Operands_definitions.h accordingly

IMPORTANT!:
Use at the beginning of main():
operatorsSymbolTree = create_SymbolTree(); //necessary for everything else to work

IMPORTANT!:
Use at the beginning of main() (can only be used after operatorsSymbolTree has been created with create_SymbolTree() ):
checkOperatorAndOperandCharsDefinitions();
to check that:
    1) Operator symbols don't include one of the reserved chars '\0', '(', ')', ','
    2) Operand string format doesn't include one the reserved chars '\0', '(', ')', ','
    3) No two Operators have the same symbol (gets checked in create_SymbolTree too)
    4) Operand string format doesn't include any char that is also the first char of an Operator symbol




