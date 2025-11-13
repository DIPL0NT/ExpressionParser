#include "ExpressionElement_and_ExpressionString.c"

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
	//it should never happen that el is of type NULLTERM, OPENPAR, CLOSEPAR, COMMA

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
	//it should never happen that el is of type NULLTERM, OPENPAR, CLOSEPAR, COMMA

	return;
}

ExpressionTreeNode *create_ExpressionTree_from_ExpressionString(ExpressionString *es){

}



