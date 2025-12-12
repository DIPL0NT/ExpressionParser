Maybe look at examples first (build scripts or prebuilt binaries included).
```bash
/examples/float
/examples/fractions
```

### Important! :
Read comments in **Operators_and_Operands_definitions.h**
```Cpp
/***************************************************************************************************************************************************
 Include this file in a file where you will define everything , unless indicated otherwise by a comment, that is declared as extern in this file.
 Include ExpressionParser.c in the file where you will actually use the parser; before using the parser must set up with:
operatorsSymbolTree = create_SymbolTree(); //necessary for everything else to work
checkOperatorAndOperandCharsDefinitions(); //to check there's no errors in the definitions
 Customize the tests in runExpressionTests() defined in ExpressionParser.c .
***************************************************************************************************************************************************/

/*******************************************************************************************
 IMPORTANT!
 The OPERAND_VALUE_TYPE define is the only thing to edit in this file.
 Before it you can include the header file of the actual implementation of the operations. 
*******************************************************************************************/

[...]
```
and edit **Operators_and_Operands_definitions.h** accordingly.

### Important! :
Use at the beginning of **main()**:  
```Cpp
operatorsSymbolTree = create_SymbolTree(); //necessary for everything else to work
```

### Important! :
Use at the beginning of **main()** (can only be used after **operatorsSymbolTree** has been created with **create_SymbolTree()** ):  
```Cpp
checkOperatorAndOperandCharsDefinitions();
```   
to check that:  
    1. **Operator** symbols don't include one of the reserved chars '\0', '(', ')', ','  
    2. **Operand** string format doesn't include one the reserved chars '\0', '(', ')', ','  
    3. No two **Operators** have the same symbol (gets checked in **create_SymbolTree()** too)  
    4. **Operand** string format doesn't include any char that is also the first char of an **Operator** symbol  

