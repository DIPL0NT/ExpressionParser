#include "ExpressionTree_V2_Logic.c"

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

void free_OperandVec(OperandVec *vec){
	//if (!vec) ...
	/*for (int i=0;i<vec->count;i++){
		releaseOperandValue(vec->values[i]); //must not free actual operand values since they might still be needed 
	}*/
	free(vec->values);
	free(vec);
	return;
}

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


OperandVec *evaluate_ExpressionTree(ExpressionTreeNode *tree){
	OperandVec *result = NULL;
	if (tree->type==OPERAND_NODE){
		result = alloc_OperandVec(1);
		result->values[0] = ((Operand*) tree->token.data)->value ;
		return result;
	}

	result = alloc_OperandVec(tree->args.count);
	int i = 0;
	ExpressionTreeNode_ListNode *cur = tree->args.head;
	for (int j=0;j<tree->args.count;j++){
		if (cur->treeNode->type==OPERAND_NODE){
			result->values[i++] = ((Operand*)cur->treeNode->token.data)->value ;
		}
		else if (cur->treeNode->type==OPERATOR_NODE){
			result->values[i++] = evaluate_ExpressionTree(cur->treeNode)->values[0] ; //remember that functions must always return exactely 1 value
		}
		else if (cur->treeNode->type==LIST_NODE){ //should be the only other case
			result->values = (OPERAND_VALUE_TYPE*)realloc(result->values,(result->count+cur->treeNode->args.count)*sizeof(OPERAND_VALUE_TYPE));
			//if (!result->values) ...
			result->count = result->count + cur->treeNode->args.count ;
			ExpressionTreeNode_ListNode *cur2 = cur->treeNode->args.head ;
			for (int k=0;k<cur->treeNode->args.count;k++){
				result->values[i++] = evaluate_ExpressionTree(cur2->treeNode)->values[0] ; //remember that functions must always return exactely 1 value
				cur2 = cur2->next;
			}
		}

		cur = cur->next;
	}

	if (tree->type==LIST_NODE){
		return result;
	}

	if (tree->type==OPERATOR_NODE){ //should be the only remaning case
		const Operator *op = (const Operator*)tree->token.data ;
		if (result->count != op->arity ){
			printf("ERROR while evaluating expression tree beacuse of arity mismatch for an operator \"\033[36m%s033[0m\"\n",op->symbol);
			free_OperandVec(result);
			return NULL;
		}
		OPERAND_VALUE_TYPE res = op->function(result->values) ;
		free_OperandVec(result);
		result = alloc_OperandVec(1);
		result->values[0] = res ;

		return result;
	}

	return NULL; //should never get here anyway
}



