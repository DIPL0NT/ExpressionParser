#include "ExpressionTree_Definition.c"

int fill_INCOMPLETE_ExpressionTreeNode(ExpressionToken_Vector *vec,ExpressionTreeNode *currentNode);

ExpressionTreeNode *create_ExpressionTree_from_ExpressionToken_Vector(ExpressionToken_Vector *vec){
    ExpressionTreeNode *tree = alloc_ExpressionTreeNode(NULL,INCOMPLETE_NODE, get_next_ExpressionToken_from_ExpressionToken_Vector(vec) );
    //if (!tree) ...

    int res = fill_INCOMPLETE_ExpressionTreeNode(vec,tree);
    if (res!=0 && res!=1){
        printf("\033[31mERROR\033[0m while parsing occurred (%d)\n",res);
        free_ExpressionTreeNode(tree);
        return NULL;
        //return tree;
    }

    return tree;
}

ExpressionTreeNode *create_LIST_ExpressionTreeNode_from_ExpressionToken_Vector(ExpressionToken_Vector *vec,ExpressionTreeNode *root){
    ExpressionTreeNode *tree = alloc_ExpressionTreeNode(root,LIST_NODE,(ExpressionToken){OPENPAR,NULL});
    //if (!tree) ...

    int complete = 0;
    ExpressionTreeNode *listElement = NULL;
    while (!complete){
        listElement = alloc_ExpressionTreeNode(tree,INCOMPLETE_NODE,(ExpressionToken){OPENPAR,NULL});
        complete = fill_INCOMPLETE_ExpressionTreeNode(vec,listElement);
        //addToTail_ExpressionTreeNode_List(&tree->args,listElement);

        //experiment
        if (listElement->type==LIST_NODE){
            concat_ExpressionTreeNode_List(&tree->args,&listElement->args);
            free(listElement);
        }
        else{
            addToTail_ExpressionTreeNode_List(&tree->args,listElement);
        }

    }
    
    if (complete!=1){ //an error occurred
        printf("\033[31mERROR\033[0m while parsing occurred (%d)\n",complete);
        free_ExpressionTreeNode(tree);
        return NULL;
        //return tree;
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
            while (currentNode->type!=INCOMPLETE_NODE){
                if (currentNode->type==OPERATOR_NODE && currentNode->args.count!=((const Operator*)currentNode->token.data)->arity){
                    printf("\033[31mERROR\033[0m while parsing (argument count %d != \033[36m%d\033[0m = operator \"\033[36m%s\033[0m\"'s arity\n",currentNode->args.count,((Operator*)currentNode->token.data)->arity,((Operator*)currentNode->token.data)->symbol);
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
            while (currentNode->type!=INCOMPLETE_NODE){
                if (currentNode->type==OPERATOR_NODE && currentNode->args.count!=((const Operator*)currentNode->token.data)->arity){
                    printf("\033[31mERROR\033[0m while parsing token number %d (argument count %d != \033[36m%d\033[0m = operator \"\033[36m%s\033[0m\"'s arity\n",vec->index-1,currentNode->args.count,((Operator*)currentNode->token.data)->arity,((Operator*)currentNode->token.data)->symbol);
                    error = 2;
                    return error;
                }
                currentNode = currentNode->root;
            }

            if (currentNode->args.count==0){
                printf("\033[31mERROR\033[0m while parsing token number %d (comma following empty expression)\n",vec->index-1);
                error = 4;
                return error;
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

            int tokenN = vec->index-1;
            ExpressionTreeNode *subtree = create_LIST_ExpressionTreeNode_from_ExpressionToken_Vector(vec,currentNode); //recursive call
            if (subtree==NULL){
                //printf
                error = -1;
                return error;
            }

            if (subtree->type==LIST_NODE){ //subtree is LIST for sure , must instead check if its args are lists
                if (currentNode->type==OPERATOR_NODE && currentNode->args.count+subtree->args.count > currentOp->arity){
                    printf("\033[31mERROR\033[0m while parsing token number %d (argument count > \033[36m%d\033[0m = operator \"\033[36m%s\033[0m\"'s arity\n",tokenN,currentOp->arity,currentOp->symbol);
                    error = 2;
                    return error;
                }
                
                concat_ExpressionTreeNode_List(&currentNode->args,&subtree->args);
                free(subtree);
            }
            else{ //useless
                printf("I'M NOT USELESS\n");
                addToTail_ExpressionTreeNode_List(&currentNode->args,subtree);
            }

            break;
        }

        case (OPERAND):{
            while (currentNode->type==OPERATOR_NODE && currentNode->args.count==currentOp->arity){ //check compatibility with current op
                currentNode = currentNode->root;
                currentOp = (const Operator*)currentNode->token.data ;
            }
            
            addToTail_ExpressionTreeNode_List(&currentNode->args, alloc_ExpressionTreeNode(currentNode,OPERAND_NODE,currentToken) );

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
                if (currentNode->type==INCOMPLETE_NODE){
                    if (currentNode->args.count<1){
                        printf("\033[31mERROR\033[0m while parsing token number %d (INFIX operator \"\033[36m%s\033[0m\" must have exactely one operand before it)\n",vec->index-1,newOp->symbol);
                        error = 2;
                        return error;
                    }
                    ExpressionTreeNode *newNode = alloc_ExpressionTreeNode(currentNode,OPERATOR_NODE,currentToken);
                    addToHead_ExpressionTreeNode_List(&newNode->args,removeTail_ExpressionTreeNode_List(&currentNode->args));
                    addToTail_ExpressionTreeNode_List(&currentNode->args,newNode);
                    currentNode = newNode;
                    currentOp = newOp;
                }
                else if (currentNode->type==OPERATOR_NODE){
                    if (newOp->precedence > currentOp->precedence){
                        if (currentNode->args.count<1){
                            printf("\033[31mERROR\033[0m while parsing token number %d (INFIX operator \"\033[36m%s\033[0m\" must have exactely one operand before it)\n",vec->index-1,newOp->symbol);
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
                        while (currentNode->root->type!=INCOMPLETE_NODE && newOp->precedence <= currentOp->precedence){
                            if (currentNode->args.count != currentOp->arity){
                                printf("\033[31mERROR\033[0m while parsing (insufficient arg count for operator \"\033[36m%s\033[0m\")\n",currentOp->symbol);
                                error = 2;
                                return error;
                            }
                            currentNode = currentNode->root;
                            currentOp = (const Operator*)currentNode->token.data ;
                        }
                        if (currentNode->args.count != currentOp->arity){
                            printf("\033[31mERROR\033[0m while parsing (insufficient arg count for operator \"\033[36m%s\033[0m\")\n",currentOp->symbol);
                            error = 2;
                            return error;
                        }

                        ExpressionTreeNode *newNode = alloc_ExpressionTreeNode(currentNode,currentNode->type,currentNode->token);
                        newNode->args = currentNode->args;
                        currentNode->token = currentToken;
                        currentOp = newOp;
                        currentNode->args = create_ExpressionTreeNode_List();
                        addToHead_ExpressionTreeNode_List(&currentNode->args,newNode); //empty list so it's equivalent to addToTail
                    }
                }
            }
            else if (newOp->fix==POSTFIX){
                while (currentNode->type!=INCOMPLETE_NODE){
                    currentNode = currentNode->root;
                }
                /*
                TODO:
                handle case where the INCOMPLETE node has args that are lists that must be considered as args for the POSTFIX operator
                */
                if (currentNode->args.count < newOp->arity){
                    printf("\033[31mERROR\033[0m while parsing token number %d (argument count < \033[36m%d\033[0m) = operator \"\033[36m%s\033[0m\"'s arity\n",vec->index-1,newOp->arity,newOp->symbol);
                    error = 2;
                    return error;
                }
                ExpressionTreeNode *newNode = alloc_ExpressionTreeNode(currentNode,OPERATOR_NODE,currentToken);
                for (int i=0;i<newOp->arity;i++){
                    addToHead_ExpressionTreeNode_List(&newNode->args,removeTail_ExpressionTreeNode_List(&currentNode->args));
                }
                addToTail_ExpressionTreeNode_List(&currentNode->args,newNode);
            }

            break;
        }

        default:{
            printf("\033[31mERROR\033[0m while parsing token number %d (unexpected token type %d)\n",vec->index-1,currentToken.type);
            error = -2;
            return error;
        }

        }

    }
    
    return 0;
}
