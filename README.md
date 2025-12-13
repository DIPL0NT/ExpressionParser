### **Quick implementation of algebraic expression parsing and evaluation with custom defined Operators and Operands**

Maybe look at examples first (build scripts or prebuilt binaries included).
```bash
/examples/float       # primitive type values as operands
/examples/fractions   # dynamically allocated objects as operands
```

### Important! :
Read comments in **Operators_and_Operands_definitions.h**.
```Cpp
Operators_and_Operands_definitions.h:

[...]

/***********************************************************************************************************************************************
 Include ExpressionParser.c in a file where everything that is marked as TODEFINE in this file must be defined.
 Define the operators' functions, which must receive an array of void* and return a void* (your operands must be cast to void*), in that file.
 Before using the parser must set up with:
operatorsSymbolTree = create_SymbolTree(); //necessary for everything else to work
checkOperatorAndOperandCharsDefinitions(); //to check there's no errors in the definitions
 Run tests with:
runExpressionTests(your_tests,your_testsCount);
***********************************************************************************************************************************************/

[...]
```

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
    1. **Operator** symbols don't include one of the reserved chars '\0'  '('  ')'  ','  
    2. **Operand** string format doesn't include one the reserved chars '\0'  '('  ')'  ','  
    3. No two **Operators** have the same symbol (gets checked in **create_SymbolTree()** too)  
    4. **Operand** string format doesn't include any char that is also the first char of an **Operator** symbol  
