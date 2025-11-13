#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <math.h>


//could create a tree
	/*
	         operation            
	          |     |     
	       ----     -----     
	       |            |
     operation       operation       
      |     |         |     |  
    ---     ---     ---     ---
    |         |     |         |
  numb      numb operation  numb
                   |     |
                 ---     ---
                 |         |
               numb      numb
	*/
	/*
	when creating the tree:
	while reading one parameter at a time:
	when encountering an operator with lower precedence
	 of the current root,
	  the new operator will be the new root,
	  the old root will be the left child,
	  the following expression will be the right child
	when encountering an operator with higher precedence
	 of the current root,
	  the new operator will take the place
	   of the current right child,
	  the previous right child will be the new operator's
	   left child,
	  the following expression will be the new operator's
	   right child
	*/

typedef enum{PREFIX,INFIX,POSTFIX} Fix;

typedef struct Operator{
	char* symbol;
	int arity; //0, 1, 2
	Fix fix; //0: prefix, 1: infix
	int precedence;
	//function pointer?
} Operator;

//reserved chars '\0', '(', ')', ','
//supported arities: 0, 1, 2

const Operator sumOp  = {"+"		,2,INFIX,0};
const Operator subOp  = {"-"		,2,INFIX,0};
const Operator multOp = {"*"		,2,INFIX,1};
const Operator divOp  = {"/"		,2,INFIX,1};
const Operator sqrtOp = {"sqrt"		,1,PREFIX,2};
const Operator powOp  = {"**"		,2,INFIX,2};
#define NUMofOPERATORS 6
Operator *operators[NUMofOPERATORS] = {&sumOp,&subOp,&multOp,&divOp,&sqrtOp,&powOp};

void print_avalaible_Operators(){
	printf("Available operations:\n");
	for (int i=0;i<NUMofOPERATORS;i++){
		printf(" Symbol: \"%s\", Arity: %d, Fix: %s, Precedence: %d\n"
				,operators[i]->symbol
				,operators[i]->arity
				,operators[i]->fix==PREFIX?"PREFIX":(
						operators[i]->fix==INFIX?"INFIX":(
							operators[i]->fix==POSTFIX?"POSTFIX":"ERROR"
						)
					)
				,operators[i]->precedence
			);
	}
	return;
}

int isOperatorChar(char c){
	for (int i=0;i<NUMofOPERATORS;i++){
		for (int j=0;operators[i]->symbol[j]!='\0';j++){
			if (c==operators[i]->symbol[j]) return 1;
		}
	}
	return 0;
}

#define OPERAND_VALUE_TYPE float
typedef struct Operand{
	//type or smth?
	OPERAND_VALUE_TYPE value;
} Operand;

void release_OperandValue(OPERAND_VALUE_TYPE value){
	//do nothing for floats
	return;
}

Operand *alloc_Operand(OPERAND_VALUE_TYPE value){
	Operand *o = malloc(sizeof(Operand));
	//if (!o) ...
	o->value = value;
	return o;
}

void free_Operand(Operand *operand){
	release_OperandValue(operand->value);
	free(operand);
	return;
}

int isWhiteSpace(char c){
	if (c==' '||c=='\t'||c=='\n'||c=='\r') return 1;
	return 0;
}

//reserverd chars: '\0', '(', ')', ',' and all the operators chars
int isOperandChar(char c){
	//for floats
	if (c=='.'|| c>='0'&&c<='9' ) return 1;
	return 0;
}

/*
* Use at the beginning of main() to check that:
* 	1) Operator symbols don't include one of the reserved chars '\0', '(', ')', ','
* 	2) Operand string format doesn't include one the reserved chars '\0', '(', ')', ','
* 	3) No char is both in an Operator symbol and in the Operand string format
*/
int checkCompatibilityOperatorAndOperandChars(){
	if (
		isOperatorChar('\0') || isOperatorChar('(') || isOperatorChar(')') || isOperatorChar(',')
	){
		printf("ERROR in Operator symbols definition: a reserved char ('\\0','(',')',',') is recognized as an Operator char by isOperatorChar()\n");
		return 0;
	}
	if (
		isOperandChar('\0') || isOperandChar('(') || isOperandChar(')') || isOperandChar(',')
	){
		printf("ERROR in Operand string format definition: a reserved char ('\\0','(',')',',') is recognized as an Operand char by isOperandChar()\n");
		return 0;
	}
	
	for (int i=0;i<256;i++){
		if (
			(char)i != '\0' && (char)i != '(' &&(char)i != ')' && (char)i != ','
			&& isOperatorChar((char)i) && isOperandChar((char)i)
		){
			printf("ERROR due to incompatible Operator symbols and Operand string format definitions: char %c is recognized both as an Operator char by isOperatorChar() and as an Operand char by isOperandChar()\n",(char)i);
			return 0;
		}
	}
	printf("CORRECT Operator symbols and Operand string format definitions\n");
	return 1;
}

void print_OperandValue(OPERAND_VALUE_TYPE val){
	//for floats
	printf("%f",val);
	return;
}

void print_Operand(Operand *o){
	print_OperandValue(o->value);
	return;
}

typedef enum{NULLTERM,OPENPAR,CLOSEPAR,COMMA,OPERATOR,OPERAND} ExpressionElementType;

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
		//printf("%f",((Operand*)el.data)->value);
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

//takes as input the string format of an operand and returns pointer to newly allocated operand
//eg if operands are float the input will be "%f" and sscanf() will be used to parse
void *parseOperandToVoidPtr(char *s){
	//for floats
	float f;
	sscanf(s,"%f",&f);
	return (void*) alloc_Operand(f);
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
	//while (isWhiteSpace(es->str[i])) i++;
	//es->index = i;
	
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
		/*
		float f;
		sscanf(es->str+es->index,"%f",&f);
		el.data = (void*) alloc_Operand(f);
		*/
		el.data = parseOperandToVoidPtr(es->str+es->index);
		es->str[i] = tmp;
		es->index = i;
		return el;
	}

	while (/*!isWhiteSpace(es->str[i]) &&*/ !isOperandChar(es->str[i]) &&es->str[i]!='('
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


typedef struct ExpressionTreeNode{
	struct ExpressionTreeNode *root;
	ExpressionElement element;
	struct ExpressionTreeNode **args;
} ExpressionTreeNode;

ExpressionTreeNode *alloc_ExpressionTreeNode(ExpressionTreeNode *root,ExpressionElement el){
	ExpressionTreeNode *newNode = (ExpressionTreeNode*) malloc(sizeof(ExpressionTreeNode));
	//if (!newNode) ...
	newNode->root = root;
	newNode->element = el;
	if (el.type==OPERATOR ){
		if (((Operator*)el.data)->arity>0){
			newNode->args = (ExpressionTreeNode**) malloc( ((Operator*)el.data)->arity * sizeof(ExpressionTreeNode*) );
			//if (!newNode->args) ...
			for (int i=0;i<((Operator*)el.data)->arity;i++){
				newNode->args[i] = NULL;
			}
		}
		else{
			newNode->args = NULL;
		}
	}
	else if (el.type==OPERAND){
		newNode->args = NULL;
	}
	//it should never happen that el is of type NULLTERM, OPENPAR, CLOSEPAR

	return newNode;
}

void free_ExpressionTreeNode(ExpressionTreeNode* node){
	if (node->element.type==OPERAND){
		release_ExpressionElement(node->element);
		//node->args should be NULL
		free(node);
		return;
	}
	else if (node->element.type==OPERATOR){
		for (int i=0;i<((Operator*)node->element.data)->arity;i++){
			free_ExpressionTreeNode(node->args[i]);
		}
		release_ExpressionElement(node->element);
		if (node->args) free(node->args); //need to check that it isn't NULL for operators with arity 0
		free(node);
		return;
	}
	//it should never happen that el is of type NULLTERM, OPENPAR, CLOSEPAR

	return;
}

/*
void free_ExpressionTree(ExpressionTreeNode *root){
	if (!root) return;
	if (!root->left && !root->right){ //the node is a leaf so the element it contains is an operand (allocated on heap)
		release_ExpressionElement(root->element);
		return; 
	}
	free_ExpressionTree(root->left);
	free_ExpressionTree(root->right);
	free(root);
	return;
}
*/

ExpressionTreeNode *create_ExpressionTree_from_ExpressionString(ExpressionString es){

}

