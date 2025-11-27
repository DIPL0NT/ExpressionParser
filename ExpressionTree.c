#include "ExpressionToken_dataStructures.c"

ExpressionTreeNode *create_ExpressionTree_from_ExpressionToken_Vector(ExpressionToken_Vector *vec,ExpressionTreeNode *root);

int continue_creating_ExpressionTree_from_ExpressionToken_Vector(ExpressionToken_Vector *vec,ExpressionTreeNode *currentTreeNode,ExpressionTreeNode *root);

//Could create a tree
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

//Maybe implement ExpressionTreeNode.args as a list of nodes that keeps track of its size
// so that args can be added before encountering the operator in case of INFIX or POSTFIX operators,
// when encountering the operator compare operator arity and current args list size

//IMPORTANT
// args added to the head of the list, so when evaluating must start from tail

//the practical arity of a PREFIX operator is always 1 eg Sum(a,b) takes the only arg (a,b) ,an expression list

int continue_creating_ExpressionTree_from_ExpressionToken_Vector(ExpressionToken_Vector *vec,ExpressionTreeNode *currentTreeNode,ExpressionTreeNode *root){

	//Local state variables
	const Operator *currentOperator = NULL;
	int error = 0;

	ExpressionToken currentToken = (ExpressionToken){NULLTERM,NULL};

	while (!error){

	currentToken = get_next_ExpressionToken_from_ExpressionToken_Vector(vec);

    //if I encounter '('
	// if last token is operand, ERROR
	// if (arg list count>=arity), ERROR
	// else, add to the arg list the result of a recursive call and then continue parsing
	if (currentToken.type==OPENPAR){
		if (vec->array[vec->index-2].type==OPERAND){
			printf("ERROR while parsing token number %d (can't have opening parenthesis after an operand)\n",vec->index-1);
			error = 1;
			return error;
		}
		if (currentOperator && currentTreeNode->args.count>=currentOperator->arity){
			printf("ERROR while parsing token number %d (argument count >= operator \"\033[36m%s\033[0m\"'s arity, \033[36m%d\033[0m)\n",vec->index-1,currentOperator->symbol,currentOperator->arity);
			error = 2;
			return error;
		}
		else{ //recursive call
			addToTail_ExpressionTreeNode_List(
				&currentTreeNode->args,
				create_ExpressionTree_from_ExpressionToken_Vector(vec,currentTreeNode)
			);
		}
	}

	//if I encounter ','                                                                               |
	// if it's the first token since the leading '(', ERROR   
	// if last token is ',', ERROR                                         v
	// if I have already encountered an operator, it means I am actually parsing an arg list eg. (2 2 +,2 2 *)
												// ? if last token is operator that isn't POSTFIX, ERROR
												// if I had already encountered an operator and (arg list count!=arity), ERROR
	//                                            else the currentTreeNode must become an EXPRESSIONTREENODE_LIST,
	//                                               copy the old currentTreeNode in a new TreeNode and add it to the list,
	//                                               add to the list the result of a recursive call

    // else, return
	else if (currentToken.type==COMMA){
		if (vec->array[vec->index-2].type==OPENPAR){
			printf("ERROR while parsing token number %d (can't have comma right after opening parenthesis)\n",vec->index-1);
			error = 3;
			return error;
		}
		if (vec->array[vec->index-2].type==COMMA){
			printf("ERROR while parsing token number %d (can't have two commas in a row)\n",vec->index-1);
			error = 4;
			return error;
		}
		else if (currentOperator){
			if (currentTreeNode->args.count < currentOperator->arity){ //the case of count>arity shouldn't be possible
				printf("ERROR while parsing token number %d (argument count < than operator \"\033[36m%s\033[0m\"'s arity, \033[36m%d\033[0m)\n",vec->index-1,currentOperator->symbol,currentOperator->arity);
				error = 2;
				return error;
			}
			else{
				ExpressionTreeNode *newTreeNode = alloc_ExpressionTreeNode(currentTreeNode,currentTreeNode->token);
				newTreeNode->args = currentTreeNode->args;
				currentTreeNode->token = (ExpressionToken){EXPRESSIONTREENODE_LIST,NULL};
				currentTreeNode->args = create_ExpressionTreeNode_List();
				addToTail_ExpressionTreeNode_List(&currentTreeNode->args,newTreeNode);
				addToTail_ExpressionTreeNode_List(
					&currentTreeNode->args,
					create_ExpressionTree_from_ExpressionToken_Vector(vec,currentTreeNode) //recursive call
				);
				
			}
		}
	}


	//if I encounter an operand:
	// if I haven't encountered an operator yet, add to the arg list
	// if I had already encountered an operator, look at arg list count to check compatibility with operator arity
	//  if arity not maxed out (count<arity), add to the arg list
	//  if arity maxed out, ERROR
	else if (currentToken.type==OPERAND){
		//check if two operands in a row?? , but it could be right eg. sum 2 2
		if (!currentOperator || currentTreeNode->args.count < currentOperator->arity){ //should never happen that count >= arity
			ExpressionTreeNode *newTreeNode = alloc_ExpressionTreeNode(currentTreeNode,currentToken);
			addToTail_ExpressionTreeNode_List(&currentTreeNode->args,newTreeNode);
		}
		else{
			printf("Error while parsing token number %d (argument count >= than operator \"\033[36m%s\033[0m\"'s arity, \033[36m%d\033[0m)\n",vec->index-1,currentOperator->symbol,currentOperator->arity);
			error = 2;
			return error;
		}
	}

	//if I encounter an operator:
	// if I hadn't encountered an operator yet, put the operator as element of the TreeNode
	// if I had already encountered an operator:
	//  if it's higher precedence compared to current operator,
    //   if it's INFIX,
	//    pop head of arg list (unless empty?), add new operator to arg list, add popped element to the new operator's arg list, recursive call on??? (problem: the new operator is the root of a subtree, but since it's initialized I can't call recursively, maybe make an ad-hoc func?)
    //   if it's PREFIX
    //     add new operator to arg list, recursive call on??? (problem: the new operator is the root of a subtree, but since it's initialized I can't call recursively, maybe make an ad-hoc func?)
	//  if it's equal or lower precedence compared to current operator:
	//   the current operator must have its arity maxed out (arg list count==arity)
	/*
		 newTreeNode root,element,args
		 newTreeNode->root = currentTreeNode
		 newTreeNode->element = currentTreeNode->element
		 newTreeNode->args = currentTreeNode->args

		 currentTreeNode->element = encountered operator
		 currentTreeNode->args add newTreeNode

		 keep parsing the string considering currentTreeNode (which now has the new operator)
	
	
	*/
	else if (currentToken.type==OPERATOR){
		if (!currentOperator){
			currentTreeNode->token = currentToken;
			currentOperator = (const Operator*)currentToken.data ;
			if (currentTreeNode->args.count > currentOperator->arity){
				printf("Error while parsing token number %d (argument count >= than operator \"\033[36m%s\033[0m\"'s arity, \033[36m%d\033[0m)\n",vec->index-1,currentOperator->symbol,currentOperator->arity);
				error = 2;
				return error;
			}
			if (currentOperator->fix==INFIX && currentTreeNode->args.count!=1){
				printf("Error while parsing token number %d (INFIX operator \"\033[36m%s\033[0m\" must have exactely one operand before it)\n",vec->index-1,currentOperator->symbol);
				error = 6;
				return error;
			}
			if (currentOperator->fix==PREFIX && currentTreeNode->args.count!=0){
				printf("Error while parsing token number %d (PREFIX operator \"\033[36m%s\033[0m\" must not have operands before it)\n",vec->index-1,currentOperator->symbol);
				error = 6;
				return error;
			}
			if (currentOperator->fix==POSTFIX && currentTreeNode->args.count!=currentOperator->arity){
				printf("ERROR while parsing token number %d (argument count < than operator \"\033[36m%s\033[0m\"'s arity, \033[36m%d\033[0m)\n",vec->index-1,currentOperator->symbol,currentOperator->arity);
				error = 2;
				return error;
			}
		}
		else{
			const Operator *newOp = (const Operator*)currentToken.data;
			if ( newOp->precedence > currentOperator->precedence ){
				ExpressionTreeNode *newTreeNode = alloc_ExpressionTreeNode(currentTreeNode,currentToken);
				newTreeNode->args = create_ExpressionTreeNode_List();
				if (newOp->fix==INFIX){
					addToTail_ExpressionTreeNode_List(&newTreeNode->args, removeTail_ExpressionTreeNode_List(&currentTreeNode->args) );
					if (newTreeNode->args.head->treeNode == NULL){
						printf("ERROR while parsing token number %d (INFIX operator \"\033[36m%s\033[0m\" doesn't have an operand preceding it)\n",vec->index-1,newOp->symbol);
						error = 2;
						return error;
					}
					else{
						addToTail_ExpressionTreeNode_List(&currentTreeNode->args,newTreeNode);
						error = continue_creating_ExpressionTree_from_ExpressionToken_Vector(vec,newTreeNode,currentTreeNode);	
					}
				}
				else if (newOp->fix==PREFIX){
					addToTail_ExpressionTreeNode_List(&currentTreeNode->args,newTreeNode);
					error = continue_creating_ExpressionTree_from_ExpressionToken_Vector(vec,newTreeNode,currentTreeNode);
				}
				else if (newOp->fix==POSTFIX){ //only case left actually
					for (int j=0;j<newOp->arity;j++){
						addToHead_ExpressionTreeNode_List(&newTreeNode->args, removeTail_ExpressionTreeNode_List(&currentTreeNode->args) );
						if (newTreeNode->args.tail->treeNode == NULL){
							printf("ERROR while parsing token number %d (POSTFIX operator \"\033[36m%s\033[0m\" has arity \033[36m%d\033[0m but doesn't have enough operands preceding it)\n",vec->index-1,newOp->symbol,newOp->arity);
							error = 2;
							return error;
						}
					}
					addToTail_ExpressionTreeNode_List(&currentTreeNode->args,newTreeNode);
					error = continue_creating_ExpressionTree_from_ExpressionToken_Vector(vec,newTreeNode,currentTreeNode);
				}
			}
			else{ // newOp has lower or equal precedence to currentOp
				if (newOp->fix==PREFIX){
					printf("ERROR while parsing token number %d (previous operator \"\033[36m%s\033[0m\" has higher or equal precedence than PREFIX operator \"\033[36m%s\033[0m\")\n",vec->index-1,currentOperator->symbol,newOp->symbol);
					error = 7;
					return error;
				}
				if (newOp->fix==POSTFIX && 0){
					//TODO
					//i'm not sure how to check properly in this case
					//who even came up with postfix operators anyway
				}
				if (currentTreeNode->args.count < currentOperator->arity){
					printf("ERROR while parsing token number %d (previous operator \"\033[36m%s\033[0m\" has higher or equal precedence than operator \"\033[36m%s\033[0m\", but doesn't have enough args for its arity, \033[36m%d\033[0m)\n",vec->index-1,currentOperator->symbol,newOp->symbol,currentOperator->arity);
					error = 2;
					return error;
				}
				//
				ExpressionTreeNode *newTreeNode = alloc_ExpressionTreeNode(currentTreeNode,currentTreeNode->token);
				newTreeNode->args = currentTreeNode->args;
				currentTreeNode->token = currentToken;
				currentTreeNode->args = create_ExpressionTreeNode_List();
				addToTail_ExpressionTreeNode_List(&currentTreeNode->args,newTreeNode);
			}
		}
	}

	//if I encounter ')'
	// if last token is '(', ERROR
    // if last token is ',', ERROR
    // if last token is operator that isn't POSTFIX, ERROR
    // if I had already encountered an operator and (arg list count!=arity), ERROR
    // else, return 0 //no error
	else if (currentToken.type==CLOSEPAR){
		if (vec->array[vec->index-2].type==OPENPAR){
			printf("Error while parsing token number %d (empty sub expression)\n",vec->index-1);
			error = 5;
			return error;
		}
		else if (vec->array[vec->index-2].type==COMMA){
			printf("Error while parsing token number %d (can't have closing parenthesis right after comma)\n",vec->index-1);
			error = 3;
			return error;
		}
		else if(currentOperator && currentTreeNode->args.count < currentOperator->arity){ //should never happen that count > arity
			printf("ERROR while parsing token number %d (argument count < than operator \"\033[36m%s\033[0m\"'s arity, \033[36m%d\033[0m)\n",vec->index-1,currentOperator->symbol,currentOperator->arity);
			error = 2;
			return error;
		}
		else{
			error = 0;
			return error;
		}
	}

	else{
		printf("ERROR while parsing token number %d, unrecognized token\n",vec->index-1);
		error = -1;
		return error;
	}

	} //end while

	return error;
}


ExpressionTreeNode *create_ExpressionTree_from_ExpressionToken_Vector(ExpressionToken_Vector *vec,ExpressionTreeNode *root){

	ExpressionTreeNode *currentTreeNode = alloc_ExpressionTreeNode(root,(ExpressionToken){EXPRESSIONTREENODE_LIST,NULL});
	currentTreeNode->args = create_ExpressionTreeNode_List();

	//leading '('
	if (OPENPAR != get_next_ExpressionToken_from_ExpressionToken_Vector(vec).type){
		printf("ERROR while parsing token number %d\n",vec->index-1);
		free_ExpressionTreeNode_List(&currentTreeNode->args);
		free_ExpressionTreeNode(currentTreeNode);
		return NULL;
	}

	//START PARSING LOOP inside the function:
	int error = continue_creating_ExpressionTree_from_ExpressionToken_Vector(vec,currentTreeNode,root);

	if (error){
		//print the error in continue_creating_ExpressionTree_from_ExpressionString
		//printf("ERROR while parsing token number %d\n",vec->index-1);
		free_ExpressionTreeNode_List(&currentTreeNode->args);
		free_ExpressionTreeNode(currentTreeNode);
		return NULL;
	}
  

	return currentTreeNode;
}

//maybe refactor: do u really need root argument and splitting in 2 funcs?
//also add support for writing arg lists both with parentheses and commas eg. sum(2,2) and without eg. sum 2 2

//maybe don't refactor actually

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
	if (tree->token.type==OPERAND){
		result = alloc_OperandVec(1);
		result->values[0] = ((Operand*) tree->token.data)->value ;
		return result;
	}

	result = alloc_OperandVec(tree->args.count);
	int i = 0;
	ExpressionTreeNode_ListNode *cur = tree->args.head;
	for (int j=0;j<tree->args.count;j++){
		

		cur = cur->next;
	}
}


