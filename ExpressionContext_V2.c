#include "Operators_and_Operands_definitions.c"

typedef struct SymbolTreeNode SymbolTreeNode;
SymbolTreeNode *alloc_SymbolTreeNode(char c,const Operator *op);
void free_SymbolTreeNode(SymbolTreeNode *tree);
int addSymbolTo_SymbolTree(SymbolTreeNode *tree,const Operator *op);

typedef struct ExpressionContext{
    //operand type

    /*Operators*/

    const Operator **operators;
    int operatorsCount;

    SymbolTreeNode *symbolTree;

    /*Operands*/
    
    Operand* (*alloc_Operand)(OPERAND_VALUE_TYPE);

    void (*release_OperandValue)(OPERAND_VALUE_TYPE);

    void (*free_Operand)(Operand*);

    int (*isOperandChar)(char);
    
    void* (*parseOperandStringFormatToVoidPtr)(char*);
    
    void (*print_OperandValue)(OPERAND_VALUE_TYPE);

    void (*print_Operand)(Operand*);

    /*Tests ?*/

} ExpressionContext;


ExpressionContext *alloc_ExpressionContext(
                        const Operator **operators,
                        int operatorsCount,
                        Operand* (*alloc_Operand)(OPERAND_VALUE_TYPE),
                        void (*release_OperandValue)(OPERAND_VALUE_TYPE),
                        void (*free_Operand)(Operand*),
                        int (*isOperandChar)(char),
                        void* (*parseOperandStringFormatToVoidPtr)(char*),
                        void (*print_OperandValue)(OPERAND_VALUE_TYPE),
                        void (*print_Operand)(Operand*)
                    )
{
    ExpressionContext *newContext = (ExpressionContext*)malloc(sizeof(ExpressionContext));
    //if (!newContext) ...
    newContext->operators = operators;
    newContext->operatorsCount = operatorsCount;

    newContext->symbolTree = alloc_SymbolTreeNode('\0',NULL);
    for (int i=0;i<operatorsCount;i++){
        addSymbolTo_SymbolTree(newContext->symbolTree,operators[i]);
    }

    newContext->alloc_Operand = alloc_Operand;
    newContext->release_OperandValue = release_OperandValue;
    newContext->free_Operand = free_Operand;
    newContext->isOperandChar = isOperandChar;
    newContext->parseOperandStringFormatToVoidPtr = parseOperandStringFormatToVoidPtr;
    newContext->print_OperandValue = print_OperandValue;
    newContext->print_Operand = print_Operand;

    return newContext;
}

void free_ExpressionContext(ExpressionContext *context){
	//if (!context) ...
	
	free_SymbolTreeNode(context->symbolTree);
	free(context);

	return;
}

int isOperatorChar(ExpressionContext *context,char c){ //could be implemented as table, blah blah blah
	for (int i=0;i<context->operatorsCount;i++){
		for (int j=0;context->operators[i]->symbol[j]!='\0';j++){
			if (c==context->operators[i]->symbol[j]) return 1;
		}
	}
	return 0;
}

void print_avalaible_Operators(ExpressionContext *context){
	printf("Available operations:\n");
	for (int i=0;i<context->operatorsCount;i++){
		printf(" Symbol: \"\033[36m%s\033[0m\", Arity: \033[36m%d\033[0m, Fix: \033[36m%s\033[0m, Precedence: \033[36m%d\033[0m\n"
				,context->operators[i]->symbol
				,context->operators[i]->arity
				,context->operators[i]->fix==PREFIX?"PREFIX":(
						context->operators[i]->fix==INFIX?"INFIX":(
							context->operators[i]->fix==POSTFIX?"POSTFIX":"ERROR"
						)
					)
				,context->operators[i]->precedence
			);
	}
	return;
}




