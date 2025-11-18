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

// args added to the head of the list, so when evaluating must start from tail

//the practical arity of a PREFIX operator is always 1 eg Sum(a,b) takes the only arg (a,b) ,an expression list

ExpressionTreeNode *create_ExpressionTree_from_ExpressionString(ExpressionString *es,ExpressionTreeNode *root){

	//currentTreeNode
	//currentTreeNode->root = root
	//currentTreeNode->element = {EXPRESSIONTREENODE_LIST,NULL}
	//currentTreeNode->args = create_ExpressionTreeNode_List()

	//leading '('

  //START PARSING LOOP

	//if I encounter '('
	// if last token is operand, ERROR
	// if (arg list count>=arity), ERROR
	// else, add to the arg list the result of a recursive call and then continue parsing

	//if I encounter ','
	// if it's the first token since the leading '(', ERROR
	// if I have already encountered an operator, ERROR
	// if 

	//if I encounter an expression (either simple or between parenthesis):
	// if I haven't encountered an operator yet, add to the arg list
	// if I had already encountered an operator, look at arg list count to check compatibility with operator arity
	//  if arity not maxed out (count<arity), add to the arg list
	//  if arity maxed out, ERROR

	//if I encounter an operator:
	// if I hadn't encountered an operator yet, put the operator as element of the TreeNode
	// if I had already encountered an operator:
	//  if it's equal or higher precedence compared to current operator,
	//   pop head of arg list (unless empty?), add new operator to arg list and add popped element to the new operator's arg list
	//  if it's lower precedence compared to current operator:
	//   the current operator must have its arity maxed out (count==arity)
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


}



