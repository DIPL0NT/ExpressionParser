#include "ExpressionTree_Creation.c"

typedef struct OperandVec{
	OPERAND_VALUE_TYPE *values;
	int count;
} OperandVec;

OperandVec *alloc_OperandVec(int count){
	OperandVec *newVec = (OperandVec*)malloc(sizeof(OperandVec));
	//if (!newVec) ...
	newVec->values = (OPERAND_VALUE_TYPE*)malloc(count*sizeof(OPERAND_VALUE_TYPE));
	//if (!newVec->values) ...
	newVec->count = count;
	return newVec;
}

void release_OperandVec_values(OperandVec *vec){
    //if (!vec) ...
	for (int i=0;i<vec->count;i++){
		release_OperandValue(vec->values[i]);
	}
    return;
}

void free_OperandVec(OperandVec *vec){
	//if (!vec) ...
	//release_OperandVec_values(vec); //must not free actual operand values since they might still be needed
	free(vec->values);
	free(vec);
	return;
}

//frees vec1 and vec2
OperandVec *combine_OperandVec(OperandVec *vec1,OperandVec *vec2){
	//if (!vec1 || !vec2) ...
	OperandVec *newVec = alloc_OperandVec(vec1->count+vec2->count);
	int i;
	for (i=0;i<vec1->count;i++){
		newVec->values[i] = vec1->values[i];
	}
	for (int j=0;j<vec2->count;j++){
		newVec->values[i++] = vec2->values[j];
	}
	free_OperandVec(vec1);
	free_OperandVec(vec2);

	return newVec;
}

typedef struct OperandVec_Wrapper{
    OperandVec *vec;
    int *freeFlag;
} OperandVec_Wrapper;

int is_OperandVec_Wrapper_NULL(OperandVec_Wrapper wrp){
    if (wrp.vec==NULL) return 1;
    if (wrp.freeFlag==NULL) return 1;
    return 0;
}

OperandVec_Wrapper alloc_OperandVec_Wrapper(OperandVec *vec){
    //if (!vec) ...
    OperandVec_Wrapper wrp = {vec,NULL};
    wrp.freeFlag = malloc(vec->count*sizeof(int)); //use calloc ?
    //if (!wrp.freeFlag) ...
    return wrp;
}

void release_OperandVec_Wrapper_values(OperandVec_Wrapper wrp){
    //if (is_OperandVec_Wrapper_NULL(wrp)) ...
    for (int i=0;i<wrp.vec->count;i++){
        if (wrp.freeFlag[i]){
            release_OperandValue(wrp.vec->values[i]);
        }
	}
    return;
}

void free_OperandVec_Wrapper(OperandVec_Wrapper wrp){
    //if (is_OperandVec_Wrapper_NULL(wrp)) ...
    free_OperandVec(wrp.vec);
    free(wrp.freeFlag);
    return;
}

OperandVec_Wrapper evaluate_ExpressionTree(ExpressionTreeNode *tree){
    if (!tree){
        printf("\033[31mERROR\033[0m evaluate_ExpressionTree(NULL)\n");
        return (OperandVec_Wrapper){NULL,NULL};
    }

	OperandVec *result = NULL;
	if (tree->type==OPERAND_NODE){
		result = alloc_OperandVec(1);
		result->values[0] = ((Operand*) tree->token.data)->value ;
        OperandVec_Wrapper wrp = alloc_OperandVec_Wrapper(result);
        //if (is_OperandVec_Wrapper_NULL(wrp)) ...
        wrp.freeFlag[0] = 0;
		return wrp;
	}

	result = alloc_OperandVec(tree->args.count);
    OperandVec_Wrapper result_wrp = alloc_OperandVec_Wrapper(result);
	int i = 0;
	ExpressionTreeNode_ListNode *cur = tree->args.head;
    //OperandVec *subRes = NULL;
    OperandVec_Wrapper subRes_wrp = {NULL,NULL};
	//for (int j=0;j<tree->args.count;j++){
    while (cur){
        //subRes = NULL;
		if (cur->treeNode->type==OPERAND_NODE){
			result_wrp.vec->values[i] = ((Operand*)cur->treeNode->token.data)->value ;
            result_wrp.freeFlag[i++] = 0;
		}
		else if (cur->treeNode->type==OPERATOR_NODE){
            //maybe rewrite with combineVecs
            subRes_wrp = evaluate_ExpressionTree(cur->treeNode); //remember that functions must always return exactely 1 value
			result_wrp.vec->values[i] = subRes_wrp.vec->values[0] ;
            result_wrp.freeFlag[i++] = subRes_wrp.freeFlag[0];
            free_OperandVec_Wrapper(subRes_wrp);
		}
		else if (cur->treeNode->type==LIST_NODE){ //should be the only other case
			result->values = (OPERAND_VALUE_TYPE*)realloc(result->values,(result->count+cur->treeNode->args.count)*sizeof(OPERAND_VALUE_TYPE));
			//if (!result->values) ...
			result->count = result->count + cur->treeNode->args.count ;
			ExpressionTreeNode_ListNode *cur2 = cur->treeNode->args.head ;
			for (int k=0;k<cur->treeNode->args.count;k++){
                //maybe rewrite with combineVecs
                subRes_wrp = evaluate_ExpressionTree(cur2->treeNode); //remember that functions must always return exactely 1 value
				result_wrp.vec->values[i] = subRes_wrp.vec->values[0] ;
                result_wrp.freeFlag[i++] = subRes_wrp.freeFlag[0];
                free_OperandVec_Wrapper(subRes_wrp);
				cur2 = cur2->next;
			}
		}

		cur = cur->next;
	}

	if (tree->type==LIST_NODE){
		return result_wrp;
	}

	if (tree->type==OPERATOR_NODE){ //should be the only remaning case
		const Operator *op = (const Operator*)tree->token.data ;
		if (result->count != op->arity ){
			printf("\033[31mERROR\033[0m while evaluating expression tree beacuse of arity mismatch for an operator \"\033[36m%s033[0m\"\n",op->symbol);
			release_OperandVec_Wrapper_values(result_wrp);
            free_OperandVec_Wrapper(result_wrp);
			return (OperandVec_Wrapper){NULL,NULL};
		}
		OPERAND_VALUE_TYPE res = op->function(result->values) ;
		release_OperandVec_Wrapper_values(result_wrp);
        free_OperandVec_Wrapper(result_wrp);
		result = alloc_OperandVec(1);
        result_wrp = alloc_OperandVec_Wrapper(result);
		result_wrp.vec->values[0] = res ;
        result_wrp.freeFlag[0] = 1;

		return result_wrp;
	}

	return (OperandVec_Wrapper){NULL,NULL}; //should never get here anyway
}


int terminal_colors[] = {31,32,33,35,36};
int terminal_colors_count = sizeof(terminal_colors)/sizeof(int);
unsigned int terminal_colors_index = 0;

void print_ExpressionTree(ExpressionTreeNode *tree){
    if (tree==NULL){
        printf("print_ExpressionTree(NULL)");
        return;
    }

    //printf("\n");

    if (tree->type==OPERAND_NODE){
        print_Operand(((Operand*)tree->token.data));
    }

    else if (tree->type==OPERATOR_NODE){
        if (((const Operator*)tree->token.data)->fix==PREFIX){
            printf("\033[%dm%s(\033[0m",terminal_colors[terminal_colors_index%terminal_colors_count],((const Operator*)tree->token.data)->symbol);
            ExpressionTreeNode_ListNode *cur = tree->args.head;
            while (cur){
                terminal_colors_index++;
                print_ExpressionTree(cur->treeNode);
                terminal_colors_index--;
                cur = cur->next;
                if (cur) printf("\033[%dm,\033[0m",terminal_colors[terminal_colors_index%terminal_colors_count]);
            }
            printf("\033[%dm)\033[0m",terminal_colors[terminal_colors_index%terminal_colors_count]);
        }
        else if (((const Operator*)tree->token.data)->fix==INFIX){
            printf("\033[%dm(\033[0m",terminal_colors[terminal_colors_index%terminal_colors_count]);
            terminal_colors_index++;
            print_ExpressionTree(tree->args.head->treeNode);
            terminal_colors_index--;
            printf("\033[%dm%s\033[0m",terminal_colors[terminal_colors_index%terminal_colors_count],((const Operator*)tree->token.data)->symbol);
            terminal_colors_index++;
            print_ExpressionTree(tree->args.tail->treeNode);
            terminal_colors_index--;
            printf("\033[%dm)\033[0m",terminal_colors[terminal_colors_index%terminal_colors_count]);
        }
        else if (((const Operator*)tree->token.data)->fix==POSTFIX){
            printf("\033[%dm(\033[0m",terminal_colors[terminal_colors_index%terminal_colors_count]);
            ExpressionTreeNode_ListNode *cur = tree->args.head;
            while (cur){
                terminal_colors_index++;
                print_ExpressionTree(cur->treeNode);
                terminal_colors_index--;
                cur = cur->next;
                if (cur) printf("\033[%dm,\033[0m",terminal_colors[terminal_colors_index%terminal_colors_count]);
            }
            printf("\033[%dm)",terminal_colors[terminal_colors_index%terminal_colors_count]);
            printf("%s\033[0m",((const Operator*)tree->token.data)->symbol);
        }
    }

    else if (tree->type==LIST_NODE){
        printf("\033[%dmL(\033[0m",terminal_colors[terminal_colors_index%terminal_colors_count]);
        ExpressionTreeNode_ListNode *cur = tree->args.head;
        while (cur){
            terminal_colors_index++;
            print_ExpressionTree(cur->treeNode);
            terminal_colors_index--;
            cur = cur->next;
            if (cur) printf("\033[%dm,\033[0m",terminal_colors[terminal_colors_index%terminal_colors_count]);
        }
        printf("\033[%dm)\033[0m",terminal_colors[terminal_colors_index%terminal_colors_count]);
    }

    else{
        //printf("print_ExpressionTree(INCOMPLETE)");
        printf("\033[%dmINCOMPLETE(\033[0m",terminal_colors[terminal_colors_index%terminal_colors_count]);
        ExpressionTreeNode_ListNode *cur = tree->args.head;
        while (cur){
            terminal_colors_index++;
            print_ExpressionTree(cur->treeNode);
            terminal_colors_index--;
            cur = cur->next;
            if (cur) printf(",");
        }
        printf("\033[%dm)\033[0m",terminal_colors[terminal_colors_index%terminal_colors_count]);
    }

    return;
}




