#include "Operators_and_Operands_definitions.c"

typedef enum{NULLTERM,OPENPAR,CLOSEPAR,COMMA,OPERATOR,OPERAND,EXPRESSIONTREENODE_LIST} ExpressionTokenType;
//EXPRESSIONTREENODE_LIST is to be used when creating the expression tree
//, its data must be NULL since the actual list is referenced byt the TreeNode

typedef struct ExpressionToken{
	ExpressionTokenType type;
	void *data;
} ExpressionToken;

void release_ExpressionToken(ExpressionToken tok){
	if (tok.type==OPERAND){
		free_Operand((Operand*)tok.data);
	}
	return;
}

void print_ExpressionToken(ExpressionToken tok){
	if (tok.type==NULLTERM){
		printf("\0");
		return;
	}
	if (tok.type==OPENPAR){
		printf("(");
		return;
	}
	if (tok.type==CLOSEPAR){
		printf(")");
		return;
	}
	if (tok.type==COMMA){
		printf(",");
		return;
	}
	if (tok.type==OPERATOR){
		printf("%s",((Operator*)tok.data)->symbol);
		return;
	}
	if (tok.type==OPERAND){
		print_Operand((Operand*)tok.data);
		return;
	}
	return;
}

typedef struct ExpressionString{
	int len;
	int index;
	char *str;
} ExpressionString;

//checks parenthesization and strips whitespace
//wraps the entire expression in parentheses for good measure
ExpressionString create_ExpressionString(char* str){
	ExpressionString es = {2+strlen(str),0,NULL};
	es.str = malloc( (es.len+1)*sizeof(char) );
	//if (!es.str) ...
	int i = 0;
	es.str[i++] = '(';
	int parenthesis_count=0;
	int j=0;
	while (str[j]!='\0'){
		if (isWhiteSpace(str[j])){
			while (isWhiteSpace(str[++j]));
			es.str[i++] = ' '; //all instances of continguos whitespace are stored as a single space char
			continue;
		}
		if (str[j]==')'){
			if (parenthesis_count<1){
				printf("Parenthesization error in expression string: %s (surplus closed parenthesis in position %d)\n",str,j);
				free(es.str);
				es.str=NULL;
				return es;
			}
			parenthesis_count--;
		}
		if (str[j]=='('){
			parenthesis_count++;
		}
		/*
		if (str[j]==','){

		}
		*/
		es.str[i++] = str[j++];
	}

	if (parenthesis_count>0){
		printf("Parenthesization error in expression string: %s (missing closed parenthesis in position %d, the end of the string)\n",str,i-1);
		free(es.str);
		es.str=NULL;
		return es;
	}
	
	es.str[i] = ')';
	es.str[i+1] = '\0';
	es.str;

	return es;
}

/*ASSUMPTIONS: 1) es->str has been stripped of all whitespace.
			   2) chars that are the first char of an operator symbol
			   	  can't appear in the string formats of the operands.
*/
//CUSTOMIZE isOperandChar() BASED ON THE STRING FORMAT FOR THE TYPE OF VALUE OF THE OPERANDS
//WORKS ON ASSUMPTION THAT ANY CHAR THAT ISN'T '\0', '(', ')', AND DOESN'T GET RECOGNIZED BY isWhiteSpace() AND isOperandChar() MUST BE PART OF AN OPERATOR SYMBOL
ExpressionToken get_next_ExpressionToken_from_ExpressionString(ExpressionString *es){
	ExpressionToken tok = {NULLTERM,NULL};

	int i = es->index;
	while (isWhiteSpace(es->str[i])) i++;
	es->index = i;
	
	if (es->str[i]=='\0'){
		tok.type = NULLTERM;
		tok.data = NULL;
		es->index = i+1;
		return tok;
	}
	if (es->str[i]=='('){
		tok.type = OPENPAR;
		tok.data = NULL;
		es->index = i+1;
		return tok;
	}
	if (es->str[i]==')'){
		tok.type = CLOSEPAR;
		tok.data = NULL;
		es->index = i+1;
		return tok;
	}
	if (es->str[i]==','){
		tok.type = COMMA;
		tok.data = NULL;
		es->index = i+1;
		return tok;
	}

	while (isOperandChar(es->str[i])) i++;
	if (i!=es->index){ //the character encountered was the beginning of a operand token
		char tmp = es->str[i];
		es->str[i] = '\0';
		tok.type = OPERAND;
		tok.data = parseOperandStringFormatToVoidPtr(es->str+es->index);
		if (tok.data==NULL){ //the operand string format wasn't recognized
			printf("Parsing error in expression string: %s (unable to identify operand string format in position %d)\n",es->str,es->index);
			tok.type = NULLTERM;
		}
		es->str[i] = tmp;
		es->index = i;
		return tok;
	}

	while ( !isOperandChar(es->str[i]) &&es->str[i]!='(' &&es->str[i]!=')' &&es->str[i]!=',' &&es->str[i]!='\0') i++;
		
	char tmp = es->str[i];
	es->str[i] = '\0';
	for (int j=0;j<NUMofOPERATORS;j++){
		if (!strcmp(es->str+es->index,operators[j]->symbol)){
			tok.type = OPERATOR;
			tok.data = (void*) operators[j];
			es->str[i] = tmp;
			es->index = i;
			return tok;
		}
	}
	
	//if we get here parseOperandStringFormatToVoidPtr wasn't able to parse the token so there must be an error (either operator symbol misspelling or incorrect operand string format)
	printf("Parsing error in expression string: %s (unable to identify token in position %d)\n",es->str,es->index);

	tok.type = NULLTERM;
	tok.data = NULL;
	return tok;
}

//SHOULD THIS BE USED?
//function made so that operators and operands can contain the same chars (overlap between operandChars and operator symbol chars)
//the problem is that this requires that there's always a space between operands and operators
//that way cases where there's no overlap (say the operands are numbers and the expressions look like 2+2) would be restricted (it would force 2+2 to be written as "2 + 2")
//CURRENT IMPLEMENTATION:
//an operand token can:
// have all chars that are also operatorChars
// have all chars that are isOperandChars
// begin with operatorChar and end with isOperandChars
//cant have operatorChar*isOperandChar*operatorChar
//cant have isOperandChar*operatorChar
ExpressionToken get_next_ExpressionToken_from_ExpressionString_OperatorAndOperandsHaveOverlappingChars(ExpressionString *es){
	ExpressionToken tok = {NULLTERM,NULL};

	char tmp = 0;
	int i = es->index;
	while (isWhiteSpace(es->str[i])) i++;
	es->index = i;
	
	if (es->str[i]=='\0'){
		tok.type = NULLTERM;
		tok.data = NULL;
		es->index = i+1;
		return tok;
	}
	if (es->str[i]=='('){
		tok.type = OPENPAR;
		tok.data = NULL;
		es->index = i+1;
		return tok;
	}
	if (es->str[i]==')'){
		tok.type = CLOSEPAR;
		tok.data = NULL;
		es->index = i+1;
		return tok;
	}
	if (es->str[i]==','){
		tok.type = COMMA;
		tok.data = NULL;
		es->index = i+1;
		return tok;
	}
	if (isOperandChar(es->str[i])){ //this way the entire token must be of operandChars
		while (isOperandChar(es->str[i])) i++; //continue scanning until end of token
		tmp = es->str[i];
		es->str[i] = '\0';
		tok.type = OPERAND;
		tok.data = parseOperandStringFormatToVoidPtr(es->str+es->index);
		if (tok.data != NULL){ //operand string format was recognized and correctely parsed
			es->str[i] = tmp;
			es->index = i;
			return tok;
		}
		else{
			printf("Parsing error in expression string: %s (unable to identify token in position %d)\n",es->str,es->index);
			tok.type = NULLTERM;
			es->str[i] = tmp;
			es->index = i;
			return tok;
		}
	}

	while (!isReservedChar(es->str[i]) && !isWhiteSpace(es->str[i]) && !isOperandChar(es->str[i])) i++;
	tmp = es->str[i];
	es->str[i] = '\0';
	for (int j=0;j<NUMofOPERATORS;j++){ //check if the token is an operator symbol
		if (!strcmp(es->str+es->index,operators[j]->symbol)){
			tok.type = OPERATOR;
			tok.data = (void*) operators[j];
			es->str[i] = tmp;
			es->index = i;
			return tok;
		}
	}
	//if we get here it means the token wasn't an operator symbol, so it must be an operand
	es->str[i] = tmp;
	//while (!isReservedChar(es->str[i]) && !isWhiteSpace(es->str[i])) i++;
	while (isOperandChar(es->str[i])) i++; //continue scanning until end of token
	                                       //this way an operand token can:
										   // have all chars that are also operatorChars
										   // have all chars that are isOperandChars
										   // begin with operatorChar and end with isOperandChars
										   //cant have operatorChar*isOperandChar*operatorChar
	tmp = es->str[i];
	es->str[i] = '\0';
	tok.type = OPERAND;
	tok.data = parseOperandStringFormatToVoidPtr(es->str+es->index);
	if (tok.data != NULL){ //operand string format was recognized and correctely parsed
		es->str[i] = tmp;
		es->index = i;
		return tok;
	}
	
	//if we get here parseOperandStringFormatToVoidPtr wasn't able to parse the token so there must be an error (either operator symbol misspelling or incorrect operand string format)
	printf("Parsing error in expression string: %s (unable to identify token in position %d)\n",es->str,es->index);

	tok.type = NULLTERM;
	tok.data = NULL;
	return tok;
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
	newVec->index = 0;

	return newVec;
}

void free_ExpressionToken_Vector(ExpressionToken_Vector *vec){
	//if (!vec->array) ...
	//if (vec->count>vec->size) ...

	for(int i=0;i<vec->count;i++){
		release_ExpressionToken(vec->array[i]);
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
		vec->capacity *= 2; //magic number
	}
	
	vec->array[vec->count] = tok;
	vec->count++;
	return;
}

ExpressionToken_Vector *create_ExpressionToken_Vector_from_ExpressionString(ExpressionString *es){
	ExpressionToken_Vector *newVec = create_ExpressionToken_Vector(es->len/2); //magic number
	//if (!newVec) ...

	while (newVec->array[newVec->count-1].type!=NULLTERM){
		addTo_ExpressionToken_Vector(newVec,get_next_ExpressionToken_from_ExpressionString(es));
	}

	return newVec;
}

ExpressionToken get_next_ExpressionToken_from_ExpressionToken_Vector(ExpressionToken_Vector *vec){
	if (vec->count==0) return (ExpressionToken){NULLTERM,NULL};
	if (vec->index>=vec->count){
		//...
		return (ExpressionToken){NULLTERM,NULL};
	}

	return vec->array[vec->index++];
}

