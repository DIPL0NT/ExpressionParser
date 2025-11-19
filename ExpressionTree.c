#include "ExpressionElement_dataStructures.c"

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


int continue_creating_ExpressionTree_from_ExpressionString(ExpressionTreeNode *currentNode,ExpressionElement_Vector *vec,ExpressionTreeNode *root){

    //if I encounter '('
	// if last token is operand, ERROR
	// if (arg list count>=arity), ERROR
	// else, add to the arg list the result of a recursive call and then continue parsing

	//if I encounter ','
	// if it's the first token since the leading '(', ERROR
	// if I have already encountered an operator, ERROR
	// if last token is ',', ERROR
    // else, continue parsing

	//if I encounter an operand:
	// if I haven't encountered an operator yet, add to the arg list
	// if I had already encountered an operator, look at arg list count to check compatibility with operator arity
	//  if arity not maxed out (count<arity), add to the arg list
	//  if arity maxed out, ERROR

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

	//if I encounter ')'
    // if last token is ',', ERROR
    // if last token is operator that isn't POSTFIX, ERROR
    // if I had already encountered an operator and (arg list count!=arity), ERROR
    // else, return 0 //no error

}


ExpressionTreeNode *create_ExpressionTree_from_ExpressionString(ExpressionElement_Vector *vec,ExpressionTreeNode *root){

	//currentTreeNode
	//currentTreeNode->root = root
	//currentTreeNode->element = {EXPRESSIONTREENODE_LIST,NULL} //default node type is an expression list
	//currentTreeNode->args = create_ExpressionTreeNode_List()

	//leading '('

  //START PARSING LOOP inside the function:
  //int error = continue_creating_ExpressionTree_from_ExpressionString(currentNode,vec,root,);

  /*if (error){
    //CLEANUP
    return NULL;
  }
  */

  //return currentNode;

}





