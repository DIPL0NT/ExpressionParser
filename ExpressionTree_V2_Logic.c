#include "ExpressionTree_V2.c"

int fill_INCOMPLETE_ExpressionTreeNode(ExpressionToken_Vector *vec,ExpressionTreeNode *currentNode);

ExpressionTreeNode *create_ExpressionTree_from_ExpressionToken_Vector(ExpressionToken_Vector *vec,ExpressionTreeNode *root){
    ExpressionTreeNode *tree = alloc_ExpressionTreeNode(root,LIST_NODE,(ExpressionToken){OPENPAR,NULL});
    //if (!tree) ...

    int complete = 0;
    ExpressionTreeNode *listElement = NULL;
    while (!complete){
        listElement = alloc_ExpressionTreeNode(tree,INCOMPLETE,(ExpressionToken){NULLTERM,NULL});
        complete = fill_INCOMPLETE_ExpressionTreeNode(vec,listElement);
        addToTail_ExpressionTreeNode_List(&tree->args,listElement);
    }
    
    if (complete!=1){ //an error occurred
        free_ExpressionTreeNode(tree);
        return NULL;
    }

    return tree;
}

int fill_INCOMPLETE_ExpressionTreeNode(ExpressionToken_Vector *vec,ExpressionTreeNode *currentNode){

    ExpressionToken currentToken = (ExpressionToken){OPENPAR,NULL};
    const Operator *currentOp = NULL;

    int error = 0;
    while (1 /*currentToken.type!=NULLTERM*/){

        currentToken = get_next_ExpressionToken_from_ExpressionToken_Vector(vec);

        switch (currentToken.type){
        
        case (CLOSEPAR):{
            //check if operators all have all their args
            while (currentNode->type!=INCOMPLETE){
                if (currentNode->type==OPERATOR_NODE && currentNode->args.count!=((const Operator*)currentNode->token.data)->arity){
                    printf("ERROR while parsing (argument count %d != \033[36m%d\033[0m = operator \"\033[36m%s\033[0m\"'s arity\n",currentNode->args.count,((Operator*)currentNode->token.data)->arity,((Operator*)currentNode->token.data)->symbol);
                    error = 2;
                    return error;
                }
                currentNode = currentNode->root;
            }

            if (currentNode->args.count > 1){
                currentNode->type = LIST_NODE;
            }
            else{
                ExpressionTreeNode *actualNode = removeHead_ExpressionTreeNode_List(&currentNode->args); //in this case equivalent to removeTail
                currentNode->type = actualNode->type;
                currentNode->token = actualNode->token;
                currentNode->args = actualNode->args;
                free(actualNode);
            }

            return 1;
            //break;
        }
        case (COMMA):{
            //check if operators all have all their args
            while (currentNode->type!=INCOMPLETE){
                if (currentNode->type==OPERATOR_NODE && currentNode->args.count!=((const Operator*)currentNode->token.data)->arity){
                    printf("ERROR while parsing (argument count %d != \033[36m%d\033[0m = operator \"\033[36m%s\033[0m\"'s arity\n",currentNode->args.count,((Operator*)currentNode->token.data)->arity,((Operator*)currentNode->token.data)->symbol);
                    error = 2;
                    return error;
                }
                currentNode = currentNode->root;
            }

            if (currentNode->args.count > 1){
                currentNode->type = LIST_NODE;
            }
            else{
                ExpressionTreeNode *actualNode = removeHead_ExpressionTreeNode_List(&currentNode->args); //in this case equivalent to removeTail
                currentNode->type = actualNode->type;
                currentNode->token = actualNode->token;
                currentNode->args = actualNode->args;
                free(actualNode);
            }

            return 0;
            //break;
        }

        case (OPENPAR):{
            while (currentNode->type==OPERATOR_NODE && currentNode->args.count==currentOp->arity){ //check compatibility with current op
                currentNode = currentNode->root;
                currentOp = (const Operator*)currentNode->token.data ;
            }

            int tokenN = vec->index;
            ExpressionTreeNode *subtree = create_ExpressionTree_from_ExpressionToken_Vector(vec,currentNode); //recursive call
            if (subtree==NULL){
                //printf
                error = -1;
                return error;
            }

            if (subtree->type==LIST_NODE){
                if (currentNode->type==OPERATOR_NODE && currentNode->args.count+subtree->args.count > currentOp->arity){
                    printf("ERROR while parsing token number %d (argument count > \033[36m%d\033[0m = operator \"\033[36m%s\033[0m\"'s arity\n",tokenN,currentOp->arity,currentOp->symbol);
                    error = 2;
                    return error;
                }
                
                concat_ExpressionTreeNode_List(&currentNode->args,&subtree->args);
                free(subtree);
            }
            else{
                addToTail_ExpressionTreeNode_List(&currentNode->args,subtree);
            }

            while (currentNode->type==OPERATOR_NODE && currentNode->args.count==currentOp->arity){
                currentNode = currentNode->root;
                currentOp = (const Operator*)currentNode->token.data ;
            }
             
            break;
        }

        case (OPERAND):{
            while (currentNode->type==OPERATOR_NODE && currentNode->args.count==currentOp->arity){ //check compatibility with current op
                currentNode = currentNode->root;
                currentOp = (const Operator*)currentNode->token.data ;
            }
            
            addToTail_ExpressionTreeNode_List(&currentNode->args, alloc_ExpressionTreeNode(currentNode,OPERAND,currentToken) );

            while (currentNode->type==OPERATOR_NODE && currentNode->args.count==currentOp->arity){
                currentNode = currentNode->root;
                currentOp = (const Operator*)currentNode->token.data ;
            }

            break;
        }

        case (OPERATOR):{
            const Operator *newOp = (const Operator*)currentToken.data;
            if (newOp->fix==PREFIX){
                while (currentNode->type==OPERATOR_NODE && currentNode->args.count==currentOp->arity){ //check compatibility with current op
                    currentNode = currentNode->root;
                    currentOp = (const Operator*)currentNode->token.data ;
                }
                ExpressionTreeNode *newNode = alloc_ExpressionTreeNode(currentNode,OPERATOR_NODE,currentToken);
                addToTail_ExpressionTreeNode_List(&currentNode->args,newNode);
                currentNode = newNode;
                currentOp = newOp;
            }
            else if (newOp->fix==INFIX){
                if (currentNode->type==INCOMPLETE){
                    if (currentNode->args.count!=1){
                        printf("ERROR while parsing token number %d (INFIX operator \"\033[36m%s\033[0m\" must have exactely one operand before it)\n",vec->index-1,newOp->symbol);
                        error = 2;
                        return error;
                    }
                    ExpressionTreeNode *newNode = alloc_ExpressionTreeNode(currentNode,OPERATOR_NODE,currentToken);
                    addToHead_ExpressionTreeNode_List(&newNode->args,removeTail_ExpressionTreeNode_List(&currentNode->args));
                    addToTail_ExpressionTreeNode_List(&currentNode->args,newNode);
                    currentNode = newNode;
                    currentOp = newOp;
                }
                else if (currentNode->type==OPERATOR){
                    if (newOp->precedence > currentOp->precedence){
                        if (currentOp->fix==INFIX){
                            printf("ERROR while parsing token number %d (INFIX operator right after an INFIX operator)\n",vec->index-1);
                            error = 3;
                            return error;
                        }
                        if (currentNode->args.count==0){
                            printf("ERROR while parsing token number %d (INFIX operator \"\033[36m%s\033[0m\" must have exactely one operand before it)\n",vec->index-1,newOp->symbol);
                            error = 2;
                            return error;
                        }
                        ExpressionTreeNode *newNode = alloc_ExpressionTreeNode(currentNode,OPERATOR_NODE,currentToken);
                        addToHead_ExpressionTreeNode_List(&newNode->args,removeTail_ExpressionTreeNode_List(&currentNode->args));
                        addToTail_ExpressionTreeNode_List(&currentNode->args,newNode);
                        currentNode = newNode;
                        currentOp = newOp;
                    }
                    else{ // newOp->precedence <= currentOp->precedence
                        while (currentNode->root->type!=LIST_NODE && newOp->precedence <= currentOp->precedence){
                            if (currentNode->args.count != currentOp->arity){
                                printf("ERROR while parsing (insufficient arg count for operator \"\033[36m%s\033[0m\")\n",currentOp->symbol);
                                error = 2;
                                return error;
                            }
                            currentNode = currentNode->root;
                            currentOp = (const Operator*)currentNode->token.data ;
                        }
                        ExpressionTreeNode *newNode = alloc_ExpressionTreeNode(currentNode,currentNode->type,currentNode->token);
                        newNode->args = currentNode->args;

                        currentNode->token = currentToken;
                        currentOp = newOp;
                        currentNode->args = create_ExpressionTreeNode_List();
                        addToHead_ExpressionTreeNode_List(&currentNode->args,newNode);
                    }
                }
            }
            else if (newOp->fix==POSTFIX){
                
            }

            break;
        }

        default:{
            printf("ERROR while parsing token number %d (unexpected token type %d)\n",vec->index-1,currentToken.type);
            error = -2;
            return -2;
        }

        }

    }

    //if (currentNode->root->type)
    
    return 0;
}




