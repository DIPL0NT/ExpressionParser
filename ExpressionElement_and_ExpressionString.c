#include "Operators_and_Operands_definitions.c"

typedef enum{NULLTERM,OPENPAR,CLOSEPAR,COMMA,OPERATOR,OPERAND,EXPRESSIONTREENODE_LIST} ExpressionElementType;
//EXPRESSIONTREENODE_LIST is to be used when creating the expression tree
//, its data must be NULL since the actual list is referenced byt the TreeNode

typedef struct ExpressionElement{
	ExpressionElementType type;
	void *data;
} ExpressionElement;

void release_ExpressionElement(ExpressionElement el){
	if (el.type==OPERAND){
		free_Operand((Operand*)el.data);
	}
	return;
}

void print_ExpressionElement(ExpressionElement el){
	if (el.type==NULLTERM){
		printf("\0");
		return;
	}
	if (el.type==OPENPAR){
		printf("(");
		return;
	}
	if (el.type==CLOSEPAR){
		printf(")");
		return;
	}
	if (el.type==COMMA){
		printf(",");
		return;
	}
	if (el.type==OPERATOR){
		printf("%s",((Operator*)el.data)->symbol);
		return;
	}
	if (el.type==OPERAND){
		print_Operand((Operand*)el.data);
		return;
	}
	return;
}

typedef struct ExpressionString{
	int len;
	int index;
	char *str;
	ExpressionElementType last_type;
} ExpressionString;

//checks parenthesization and strips whitespace
//wraps the entire expression in parentheses for good measure
ExpressionString create_ExpressionString(char* str){
	ExpressionString es = {2+strlen(str),0,NULL,OPENPAR};
	es.str = malloc( (es.len+1)*sizeof(char) );
	//if (!es.str) ...
	int i = 0;
	es.str[i++] = '(';
	int parenthesis_count=0;
	int j=0;
	while (str[j]!='\0'){
		if (isWhiteSpace(str[j])){ //skip whitespace to strip whitespace
			j++;
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
ExpressionElement get_next_ExpressionElement_from_ExpressionString(ExpressionString *es){
	ExpressionElement el = {NULLTERM,NULL};

	int i = es->index;
	
	if (es->str[i]=='\0'){
		el.type = NULLTERM;
		el.data = NULL;
		es->index = i+1;
		return el;
	}
	if (es->str[i]=='('){
		el.type = OPENPAR;
		el.data = NULL;
		es->index = i+1;
		return el;
	}
	if (es->str[i]==')'){
		el.type = CLOSEPAR;
		el.data = NULL;
		es->index = i+1;
		return el;
	}
	if (es->str[i]==','){
		el.type = COMMA;
		el.data = NULL;
		es->index = i+1;
		return el;
	}

	while (isOperandChar(es->str[i])) i++;
	if (i!=es->index){ //the character encountered was the beginning of a operand token
		char tmp = es->str[i];
		es->str[i] = '\0';
		el.type = OPERAND;
		el.data = parseOperandStringFormatToVoidPtr(es->str+es->index);
		es->str[i] = tmp;
		es->index = i;
		return el;
	}

	while ( !isOperandChar(es->str[i]) &&es->str[i]!='('
			&&es->str[i]!=')' &&es->str[i]!=',' &&es->str[i]!='\0') i++;
	
	char tmp = es->str[i];
	es->str[i] = '\0';
	for (int j=0;j<NUMofOPERATORS;j++){
		if (!strcmp(es->str+es->index,operators[j]->symbol)){
			el.type = OPERATOR;
			el.data = (void*) operators[j];
			es->str[i] = tmp;
			es->index = i;
			return el;
		}
	}
	
	//if we get here it wasn't \0 or a parenthesis or an operator or an operand => there's smth wrong
	printf("Parsing error in expression string: %s (unable to identify token in position %d)\n",es->str,es->index);

	el.type = NULLTERM;
	el.data = NULL;
	return el;
}



