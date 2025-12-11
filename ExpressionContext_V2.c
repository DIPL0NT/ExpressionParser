#include "ExpressionToken_V2.c"

typedef struct ExpressionContext{
    //operand type

    /*Operators*/

    const Operator **operators;
    int operatorsCount;

    int (*isOperatorChar)(char);

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


ExpressionContext *initialise_ExpressionContext(
                        const Operator **operators,
                        int operatorsCount,
                        int (*isOperatorChar)(char),
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
    newContext->isOperatorChar = isOperatorChar;

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





typedef struct{
	ExpressionToken *array;
	int capacity;
	int count;
	int index;
} ExpressionToken_Vector;

ExpressionToken_Vector *create_ExpressionToken_Vector(int initialCapacity){
	ExpressionToken_Vector *newVec = (ExpressionToken_Vector*)malloc(sizeof(ExpressionToken_Vector));
	newVec->array = (ExpressionToken*)malloc(initialCapacity*sizeof(ExpressionToken));
	//if (!newVec->array) ...
	newVec->capacity = initialCapacity;
	newVec->count = 0;
	newVec->index = 0;

	return newVec;
}

void free_ExpressionToken_Vector(ExpressionToken_Vector *vec){
	//if (!vec->array) ...
	//if (vec->count>vec->size) ...

	for(int i=0;i<vec->count;i++){
		/*
		TODO:
		decide where to release the tokens, here or in free_ExpressionTreeNode ?
		*/
		//release_ExpressionToken(vec->array[i]);
	}

	free(vec->array);
	free(vec);
	return;
}

void addTo_ExpressionToken_Vector(ExpressionToken_Vector *vec,ExpressionToken tok){
	//if (!vec) ...
	if (vec->count==vec->capacity){
		ExpressionToken *newArray = (ExpressionToken*)malloc(vec->capacity*2*sizeof(ExpressionToken)); //magic number
		for (int i=0;i<vec->count;i++){
			newArray[i] = vec->array[i];
		}
		free(vec->array);
		vec->array = newArray;
		vec->capacity *= 2; //magic number
	}
	
	vec->array[vec->count] = tok;
	vec->count++;
	return;
}

ExpressionToken_Vector *create_ExpressionToken_Vector_from_ExpressionString(SymbolTreeNode *tree,ExpressionString *es){
	ExpressionToken_Vector *newVec = create_ExpressionToken_Vector(10); //magic number
	//if (!newVec) ...

	do {
		addTo_ExpressionToken_Vector(newVec,get_next_ExpressionToken_from_ExpressionString(tree,es));
	} while (newVec->array[newVec->count-1].type!=NULLTERM);
	/*
	if (newVec->array[newVec->count].type!=NULLTERM){
		printf("ERROR while creating token vector: the last token is not a NULLTERM\n");
		free_ExpressionToken_Vector(newVec);
		return NULL;
	}
	*/

	return newVec;
}

ExpressionToken get_next_ExpressionToken_from_ExpressionToken_Vector(ExpressionToken_Vector *vec){
	if (vec->count==0){
		//printf("Ciao\n");
		return (ExpressionToken){NULLTERM,NULL};
	}
	if (vec->index >= vec->count){
		//...
		//printf("MIAO\n");
		return (ExpressionToken){NULLTERM,NULL};
	}

	return vec->array[vec->index++];
}


