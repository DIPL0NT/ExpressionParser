#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


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


typedef struct Operator{
	char* symbol;
	int arity;
	int precedence;
} Operator;

const Operator sumOp  = {"+"		,2,0};
const Operator subOp  = {"-"		,2,0};
const Operator multOp = {"*"		,2,1};
const Operator divOp  = {"/"		,2,1};
const Operator sqrtOp = {"sqrt"		,1,2};
const Operator powOp  = {"**"		,2,2};
#define NUMofOPERATORS 6
Operator *operators[NUMofOPERATORS] = {&sumOp,&subOp,&multOp,&divOp,&sqrtOp,&powOp};

typedef struct Operand{
	//type or smth?
	float value;
} Operand;

Operand *alloc_Operand(float value){
	Operand *o = malloc(sizeof(Operand));
	//if (!o) ...
	o->value = value;
	return o;
}

void free_Operand(Operand *operand){
	free(operand);
	return;
}

int isWhiteSpace(char c){
	if (c==' '||c=='\t'||c=='\n') return 1;
	return 0;
}

int isOperandChar(char c){
	if (c=='.'|| c>='0'&&c<='9' ) return 1;
	return 0;
}

typedef enum{NULLTERM,OPENPAR,CLOSEPAR,OPERATOR,OPERAND} ExpressionElementType;

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
	if (el.type==OPERATOR){
		printf("%s",((Operator*)el.data)->symbol);
		return;
	}
	if (el.type==OPERAND){
		printf("%f",((Operand*)el.data)->value);
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

ExpressionString create_ExpressionString(char* str){
	//wraps the entire expression in parentheses for good measure
	ExpressionString es = {2+strlen(str),0,NULL,OPENPAR};
	es.str = malloc( (es.len+1)*sizeof(char) );
	//if (!es.str) ...
	es.str[0] = '(';
	es.str++;
	int parenthesis_count=0;
	int i=0;
	while (str[i]!='\0'){
		if (str[i]==')'){
			if (parenthesis_count<1){
				printf("Parenthesization error in expression string: %s (surplus closed parenthesis in position %d)\n",str,i);
				free(--es.str);
				es.str=NULL;
				return es;
			}
			parenthesis_count--;
		}
		if (str[i]=='('){
			parenthesis_count++;
		}
		es.str[i] = str[i];
		i++;
	}

	if (parenthesis_count>0){
		printf("Parenthesization error in expression string: %s (missing closed parenthesis in position %d, the end of the string)\n",str,i-1);
		free(--es.str);
		es.str=NULL;
		return es;
	}
	
	es.str[i] = ')';
	es.str[i+1] = '\0';
	es.str--;

	return es;
}



//WORKS ON ASSUMPTION THAT OPERANDS AND OPERATORS 
ExpressionElement get_next_ExpressionElement_from_ExpressionString(ExpressionString *es){
	ExpressionElement el = {NULLTERM,NULL};

	int i = es->index;
	while (isWhiteSpace(es->str[i])) i++;
	es->index = i;
	
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

	while (isOperandChar(es->str[i])) i++;
	if (i!=es->index){ //the character encountered was the beginning of a operand token
		char tmp = es->str[i];
		es->str[i] = '\0';
		el.type = OPERAND;
		float f;
		sscanf(es->str+es->index,"%f",&f);
		el.data = (void*) alloc_Operand(f);
		es->str[i] = tmp;
		es->index = i;
		return el;
	}

	while (!isWhiteSpace(es->str[i]) && !isOperandChar(es->str[i]) && es->str[i]!='('
			&&es->str[i]!=')' &&es->str[i]!='\0') i++;
	
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
	struct ExpressionTreeNode *left;
	struct ExpressionTreeNode *right;
} ExpressionTreeNode;

void free_ExpressionTree(ExpressionTreeNode *root){
	if (!root) return;
	if (!root->left && !root->right){ //the node is a leaf so the element it contains is a fraction (allocated on heap)
		if (root->element.type==OPERAND) free_Operand(root->element.data);
		return; 
	}
	free_ExpressionTree(root->left);
	free_ExpressionTree(root->right);
	free(root);
	return;
}

ExpressionTreeNode *create_tree_from_expression_string(char* expression){
	
}


int main(){

	printf("Available operations:\n");
	for (int i=0;i<NUMofOPERATORS;i++){
		printf(" Symbol: %s Arity: %d Precedence: %d\n",operators[i]->symbol,operators[i]->arity,operators[i]->precedence);
	}

	char input[] = "(0.1/(4+.2)+5.7)";
	printf("Input: %s\n",input);
	ExpressionString es = create_ExpressionString(input);
	printf("ExpressionString: %s\n",es.str);
	ExpressionElement el = {NULLTERM,NULL};
	printf("Parsed tokens: ");
	while ( (el = get_next_ExpressionElement_from_ExpressionString(&es)).type!=NULLTERM ){
		print_ExpressionElement(el);
		printf(" ");
	}
	printf("\n");

	return 0;
}
