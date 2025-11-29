#include "ExpressionTree_V2.c"

int fill_EMPTY_ExpressionTreeNode(ExpressionToken_Vector *vec,ExpressionTreeNode *currentNode);

ExpressionTreeNode *create_ExpressionTree_from_ExpressionToken_Vector(ExpressionToken_Vector *vec,ExpressionTreeNode *root){
    ExpressionTreeNode *tree = alloc_ExpressionTreeNode(root,LIST,(ExpressionToken){OPENPAR,NULL});
    //if (!tree) ...

    int incomplete = 0;
    ExpressionTreeNode *listElement = NULL;
    while (!incomplete){
        listElement = alloc_ExpressionTreeNode(tree,EMPTY,(ExpressionToken){NULLTERM,NULL});
        incomplete = fill_EMPTY_ExpressionTreeNode(vec,listElement);
        addToTail_ExpressionTreeNode_List(&tree->args,listElement);
    }
    

    return tree;
}

int fill_EMPTY_ExpressionTreeNode(ExpressionToken_Vector *vec,ExpressionTreeNode *currentNode){

    ExpressionToken currentToken = (ExpressionToken){OPENPAR,NULL};
    const Operator *currentOp = NULL;

    int error = 0;
    while (1 /*currentToken.type!=NULLTERM*/){

        currentToken = get_next_ExpressionToken_from_ExpressionToken_Vector(vec);

        switch (currentToken.type){
        
        case (CLOSEPAR):{
            return 0;
        }
        case (COMMA):{
            return 1;
        }

        case (OPENPAR):{
            if (currentNode->type==OPERATOR_NODE){ //check compatibility with current op
                if (currentOp->fix==PREFIX){
                    if (currentNode->args.count /*>*/= currentOp->arity){
                        printf("ERROR while parsing token number %d (argument count > \033[36m%d\033[0m = operator \"\033[36m%s\033[0m\"'s arity\n",vec->index-1,currentOp->arity,currentOp->symbol);
                        error = 2;
                        return error;
                    }
                }
                if (currentOp->fix==INFIX){
                    if (currentNode->args.count != 1){
                        printf("ERROR while parsing token number %d (INFIX operator \"\033[36m%s\033[0m\" must have exactely one operand before it)\n",vec->index-1,currentOp->symbol);
                        error = 2;
                        return error;
                    }
                }
                if (currentOp->fix==POSTFIX){
                    /* CHECKS on POSTFIX OPERATORS ARE DONE AS SOON AS the OPERATOR is ENCOUNTERED
                    if (currentNode->args.count != currentOp->arity){
                        printf("ERROR while parsing token number %d (argument count < \033[36m%d\033[0m) = operator \"\033[36m%s\033[0m\"'s arity\n",vec->index-1,currentOp->arity,currentOp->symbol);
                        error = 2;
                        return error;
                    }
                    */
                }
            }

            addToTail_ExpressionTreeNode_List(
                &currentNode->args,
                create_ExpressionTree_from_ExpressionToken_Vector(vec,currentNode) //recursive call
            );
            break;
        }

        case (OPERAND):{
            if (currentNode->type==EMPTY){
                currentNode->token = currentToken;
                currentNode->type = OPERAND_NODE;
            }
            else if (currentNode->type==OPERATOR_NODE){
                if (currentOp->fix==PREFIX){
                    if (currentNode->args.count /*>*/= currentOp->arity){
                        printf("ERROR while parsing token number %d (argument count > \033[36m%d\033[0m = operator \"\033[36m%s\033[0m\"'s arity\n",vec->index-1,currentOp->arity,currentOp->symbol);
                        error = 2;
                        return error;
                    }
                }
                if (currentOp->fix==INFIX){
                    if (currentNode->args.count != 1){
                        printf("ERROR while parsing token number %d (INFIX operator \"\033[36m%s\033[0m\" must have exactely one operand before it)\n",vec->index-1,currentOp->symbol);
                        error = 2;
                        return error;
                    }
                }
                if (currentOp->fix==POSTFIX){
                    /* CHECKS on POSTFIX OPERATORS ARE DONE AS SOON AS the OPERATOR is ENCOUNTERED
                    if (currentNode->args.count < currentOp->arity){
                        printf("ERROR while parsing token number %d (argument count < \033[36m%d\033[0m) = operator \"\033[36m%s\033[0m\"'s arity\n",vec->index-1,currentOp->arity,currentOp->symbol);
                        error = 2;
                        return error;
                    }
                    */
                }

                addToTail_ExpressionTreeNode_List(
                    &currentNode->args,
                    alloc_ExpressionTreeNode(currentNode,OPERAND,currentToken)
                );
            }
            //there should be no other else
            break;
        }

        case (OPERATOR):{
            if (currentNode->type==EMPTY){
                currentOp = (const Operator*)currentToken.data;
                if (currentOp->fix==PREFIX){
                    //if (currentNode->args.count!=0){
                    //    printf("ERROR while parsing token number %d (arguments preceding PREFIX operator \"\033[36m%s\033[0m\"\n",vec->index-1,currentOp->symbol);
                    //    error = 3;
                    //    return error;
                    //}
                }
                if (currentOp->fix==INFIX){
                    //if (currentNode->args.count!=1){
                        printf("ERROR while parsing token number %d (no argument preceding PREFIX operator \"\033[36m%s\033[0m\"\n",vec->index-1,currentOp->symbol);
                        error = 3;
                        return error;
                    //}
                }
                if (currentOp->fix==POSTFIX){
                    //if (currentNode->args.count != currentOp->arity){
                        printf("ERROR while parsing token number %d (no arguments preceding POSTFIX operator \"\033[36m%s\033[0m\"'s arity\n",vec->index-1,currentOp->symbol);
                        error = 3;
                        return error;
                    //}
                }
                currentNode->token = currentToken;
                currentNode->type = OPERATOR_NODE;
            }
            else if (currentNode->type==OPERAND_NODE){
                currentOp = (const Operator*)currentToken.data;
                if (currentOp->fix==PREFIX){
                    //if (currentNode->args.count!=0){
                        printf("ERROR while parsing token number %d (arguments preceding PREFIX operator \"\033[36m%s\033[0m\"\n",vec->index-1,currentOp->symbol);
                        error = 3;
                        return error;
                    //}
                }
                if (currentOp->fix==INFIX){
                    if (currentNode->args.count!=1){
                        printf("ERROR while parsing token number %d (INFIX operator \"\033[36m%s\033[0m\" must have exactely one operand before it)\n",vec->index-1,currentOp->symbol);
                        return error;
                    }
                }
                if (currentOp->fix==POSTFIX){
                    if (currentNode->args.count != currentOp->arity){
                        printf("ERROR while parsing token number %d (argument count != \033[36m%d\033[0m = POSTFIX operator \"\033[36m%s\033[0m\"'s arity\n",vec->index-1,currentOp->arity,currentOp->symbol);
                        error = 3;
                        return error;
                    }
                }
                currentNode->token = currentToken;
                currentNode->type = OPERATOR_NODE;
            }
            else if (currentNode->type==OPERATOR_NODE){
                const Operator* newOp = (const Operator*)currentToken.data;
                if ( newOp->precedence > currentOp->precedence ){

                }
                else{ // newOp->precedence <= currentOp->precedence

                }
            }
            //there should be no other else
            break;
        }

        }

    }

    if (currentNode->root->type)
    
    return 0;
}