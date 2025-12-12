#include "Operators_and_Operands_definitions.c"

typedef enum{NULLTERM,OPENPAR,CLOSEPAR,COMMA,OPERATOR,OPERAND} ExpressionTokenType;

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
		printf("\033[31m\\0\033[0m");
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

//checks parenthesization and commas and condenses contiguos whitespace into a single space char
//wraps the entire expression in parentheses for good measure
ExpressionString create_ExpressionString(char* str){
	ExpressionString es = {2+strlen(str),0,NULL};
	es.str = malloc( (es.len+1)*sizeof(char) );
	//if (!es.str) ...
	int i = 0;
	int parenthesis_count=0;
	es.str[i++] = '(';
	parenthesis_count++;
	int j=0;
	while (str[j]!='\0'){
		if (isWhiteSpace(str[j])){
			while (isWhiteSpace(str[++j]));
			es.str[i++] = ' '; //all instances of continguos whitespace are stored as a single space char
			continue;
		}
		if (str[j]==')'){
			if (parenthesis_count<1){
				printf("\033[31mERROR\033[0m while parsing expression string: %s (surplus closed parenthesis in position %d)\n",str,j);
				free(es.str);
				es.str=NULL;
				return es;
			}
			parenthesis_count--;
		}
		if (str[j]=='('){
			parenthesis_count++;
		}
		if (str[j]==','){
			if ( es.str[i-1]=='(' || es.str[i-1]==' '&&es.str[i-2]=='(' ){
				printf("\033[31mERROR\033[0m while parsing expression string: %s (opening parenthesis followed by comma in position %d)\n",str,j);
				free(es.str);
				es.str=NULL;
				return es;
			}
			if ( es.str[i-1]==',' || es.str[i-1]==' '&&es.str[i-2]==',' ){
				printf("\033[31mERROR\033[0m while parsing expression string: %s (comma followed by comma in position %d)\n",str,j);
				es.str[i]=0;
				printf("\n%s",es.str);
				free(es.str);
				es.str=NULL;
				return es;
			}
		}

		es.str[i++] = str[j++];
	}
	es.str[i++] = ')';
	parenthesis_count--;
	es.str[i] = '\0';
	//es.str;

	if (parenthesis_count>0){
		printf("\033[31mERROR\033[0m while parsing expression string: %s (missing closed parenthesis)\n",str);
		free(es.str);
		es.str=NULL;
		return es;
	}
	
	
	return es;
}

/*Symbol recognition decision tree*/
typedef struct SymbolTreeNode{
    char c;
    const Operator *op;
    int branchCount;
    struct SymbolTreeNode **branches;
} SymbolTreeNode;

SymbolTreeNode *alloc_SymbolTreeNode(char c,const Operator *op){
    SymbolTreeNode *newNode = (SymbolTreeNode*)malloc(sizeof(SymbolTreeNode));
    //if (!newNode) ...
    newNode->c = c;
    newNode->op = op;
    newNode->branchCount = 0;
    newNode->branches = NULL;
    
    return newNode;
}

void free_SymbolTreeNode(SymbolTreeNode *tree){
    if (!tree) return;
    for (int i=0;i<tree->branchCount;i++){
        free_SymbolTreeNode(tree->branches[i]);
    }
	free(tree->branches);
    free(tree);

    return;
}

int addSymbolTo_SymbolTree(SymbolTreeNode *tree,const Operator *op){
    if (!tree){
        printf("\033[31mERROR\033[0m addSymbolTo_SymbolTree(NULL,op)\n");
        return 1;
    }

    int len = strlen(op->symbol);
    int i,j;

    int lo; int hi;
    int mid;
    for (i=0;i<len;i++){
		lo = 0;
		hi = tree->branchCount;
        while (lo!=hi){ //search symbol char in the branches
            if (tree->branches[lo]->c==op->symbol[i]){
                if (i==len-1){
                    printf("\033[31mERROR\033[0m two operators have been defined with the same symbol \"\033[36m%s\033[0m\"\n",op->symbol);
                    return 2;
                }
                //
                tree = tree->branches[lo];
                break;
            }
            mid = (lo+hi) /2;
            if (tree->branches[mid]->c==op->symbol[i]){
                if (i==len-1){
                    printf("\033[31mERROR\033[0m two operators have been defined with the same symbol \"\033[36m%s\033[0m\"\n",op->symbol);
                    return 2;
                }
                //
                tree = tree->branches[mid];
                break;
            }
			if (lo==mid){
				hi = mid;
				break;
			}
            else if (tree->branches[mid]->c > op->symbol[i]){
                hi = mid;
            }
            else{
                lo = mid;
            }
        }
        if (lo==hi){
            //char not found, must add branch
            tree->branches = realloc( tree->branches, (tree->branchCount+1)*sizeof(SymbolTreeNode*) );
            //if (!tree->branches) ...
            tree->branchCount++;
            tree->branches[tree->branchCount-1] = alloc_SymbolTreeNode(op->symbol[i], i==len-1 ? op : NULL );
			//if (!tree->branches[tree->branchCount-1]) ...
            SymbolTreeNode *tmp = NULL;
            for (j=tree->branchCount-1;j>=0;j--){
                if (j && op->symbol[i] < tree->branches[j-1]->c){
                    tmp = tree->branches[j-1];
                    tree->branches[j-1] = tree->branches[j];
                    tree->branches[j] = tmp;
                }
                else{
                    //j is the right place for the symbol char
                    tree = tree->branches[j];
                    while (i!=len-1){
						i++;
                        tree->branches = malloc(sizeof(SymbolTreeNode*));
                        tree->branchCount = 1;
                        tree->branches[0] = alloc_SymbolTreeNode(op->symbol[i],NULL);
                        tree = tree->branches[0];
                    }
					tree->op = op;
                    return 0;
                }
            }
        }
        else{
            //char found, continue on the branch by searching next symbol char
            i++;
        }

    }

    return -1; //should never get here
}

SymbolTreeNode *operatorsSymbolTree = NULL;
/*
Must be ran before everything else
operatorsSymbolTree = create_SymbolTree();
*/
SymbolTreeNode *create_SymbolTree(){
	SymbolTreeNode *newSymbolTree = alloc_SymbolTreeNode('\0',NULL);
	//if (!newSymbolTree) ...
	for (int i=0;i<operatorsCount;i++){
		if ( addSymbolTo_SymbolTree(newSymbolTree,operators[i]) ){
			printf("\033[31mERROR\033[0m while trying to add operator symbol \"\033[36m%s\033[0m\" to SymbolTree\n",operators[i]->symbol);
			free_SymbolTreeNode(newSymbolTree);
			return NULL;
		}
	}
	return newSymbolTree;
}

int isOperatorFirstChar(char c){
	//if (!operatorsSymbolTree) ...
	for (int i=0;i<operatorsSymbolTree->branchCount;i++){
		if (operatorsSymbolTree->branches[i]->c == c){
			return 1;
		}
	}
	return 0;
}

//in the symbol tree the leaf nodes are those with branchCount = 0
ExpressionToken get_next_ExpressionToken_from_ExpressionString(SymbolTreeNode *tree,ExpressionString *es){
    ExpressionToken tok = {NULLTERM,NULL};

	char tmp = 0;
	int i = es->index;
	if (es->str[i]==' '){
		es->index = ++i;
	}
	
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

	while (es->str[i]!=' ' && !isReservedChar(es->str[i]) && !isOperatorFirstChar(es->str[i])) i++ ;
	if (i!=es->index){
		tmp = es->str[i];
		es->str[i] = '\0';
		Operand *operand = parseOperandStringFormatToVoidPtr(es->str+es->index);
		if (!operand){
			printf("Parsing error in expression string: %s (unable to identify token in position %d: %s)\n",es->str,es->index,es->str+es->index);
			tok.type = NULLTERM;
			tok.data = NULL;
			es->str[i] = tmp;
			es->index = i;
			return tok;
		}
		tok.type = OPERAND;
		tok.data = (void*) operand;
		es->str[i] = tmp;
		es->index = i;
		return tok;
	}

    //the char in the root of the tree is not to be considered
	int lo; int hi;
	int mid;
    for ( ; es->str[i]!=' ' && !isReservedChar(es->str[i]) ; i++){

        lo = 0;
		hi = tree->branchCount;
        while (lo!=hi){ //search es char in the branches
            if (tree->branches[lo]->c == es->str[i]){
                tree = tree->branches[lo];
                if (tree->op!=NULL){
                    tok.type = OPERATOR;
                    tok.data = (void*) tree->op;
                    es->index = i+1;
                    return tok;
                }
                break;
            }
            mid = (lo+hi) /2;
            if (tree->branches[mid]->c == es->str[i]){
                tree = tree->branches[mid];
                if (tree->op!=NULL){
                    tok.type = OPERATOR;
                    tok.data = (void*) tree->op;
                    es->index = i+1;
                    return tok;
                }
                break;
            }
			if (lo==mid){
				hi = mid;
				break;
			}
            else if (tree->branches[mid]->c > es->str[i]){
                hi = mid;
            }
            else{
                lo = mid;
            }
        }
        if (lo==hi){ //es char not found, so it's not an Operator => must be an Operand
            break;
        }
        else{
            //es char found, search for the next down the tree
        }

    }
    
    //it's an Operand
    while (es->str[i]!=' ' && !isReservedChar(es->str[i]) && !isOperatorFirstChar(es->str[i])) i++ ;
    tmp = es->str[i];
    es->str[i] = '\0';
    Operand *operand = parseOperandStringFormatToVoidPtr(es->str+es->index);
    if (!operand){
        printf("Parsing error in expression string: %s (unable to identify token in position %d: %s)\n",es->str,es->index,es->str+es->index);
        tok.type = NULLTERM;
        tok.data = NULL;
        es->str[i] = tmp;
        es->index = i;
        return tok;
    }
    tok.type = OPERAND;
    tok.data = (void*) operand;
    es->str[i] = tmp;
    es->index = i;

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

void print_ExpressionTokenVector(ExpressionToken_Vector *vec){
	for (int i=0;i<vec->count;i++){
		print_ExpressionToken(vec->array[i]);
		printf("_");
	}
	printf("\n");
	return;
}


