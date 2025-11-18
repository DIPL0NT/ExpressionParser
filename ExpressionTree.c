#include "ExpressionElement_and_ExpressionString.c"

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

typedef struct ExpressionTreeNode ExpressionTreeNode;

typedef struct ExpressionTreeNode_ListNode{
	ExpressionTreeNode *treeNode;
	struct ExpressionTreeNode_ListNode *next;
} ExpressionTreeNode_ListNode;

typedef struct ExpressionTreeNode_List{
	ExpressionTreeNode_ListNode *head;
	ExpressionTreeNode_ListNode *tail;
	int count;
} ExpressionTreeNode_List;

typedef struct ExpressionTreeNode{
	struct ExpressionTreeNode *root; //parent node
	ExpressionElement element;
	ExpressionTreeNode_List args;   //list of child nodes
} ExpressionTreeNode;

ExpressionTreeNode *alloc_ExpressionTreeNode(ExpressionTreeNode *root,ExpressionElement el){
	ExpressionTreeNode *newNode = (ExpressionTreeNode*) malloc(sizeof(ExpressionTreeNode));
	//if (!newNode) ...
	newNode->root = root;
	newNode->element = el;
	newNode->args.head = NULL; //{NULL,0};
	newNode->args.count = 0;
	return newNode;
}

//implemented further down
//damned circular dependencies
void free_ExpressionTreeNode_List(ExpressionTreeNode_List list);

void free_ExpressionTreeNode(ExpressionTreeNode* node){
	//if (!node) ...
	free_ExpressionTreeNode_List(node->args);
	release_ExpressionElement(node->element);
	free(node);
	return;
}

ExpressionTreeNode_List create_ExpressionTreeNode_List(){
	ExpressionTreeNode_List list;
	list.head = NULL;
	list.tail = NULL;
	list.count = 0;
	return list;
}

void addToTail_ExpressionTreeNode_List(ExpressionTreeNode_List *list,ExpressionTreeNode *treeNode){
	ExpressionTreeNode_ListNode *newNode = (ExpressionTreeNode_ListNode*) malloc(sizeof(ExpressionTreeNode_ListNode));
	//if (!newNode) ...
	newNode->treeNode = treeNode;
	newNode->next = NULL;

	if (!list->head){
		list->head = newNode;
		list->tail = newNode;
	}
	else{
		list->tail->next = newNode;
		list->tail = newNode;
	}
	list->count++;
	return;
}

void addToHead_ExpressionTreeNode_List(ExpressionTreeNode_List *list,ExpressionTreeNode *treeNode){
	ExpressionTreeNode_ListNode *newNode = (ExpressionTreeNode_ListNode*) malloc(sizeof(ExpressionTreeNode_ListNode));
	//if (!newNode) ...
	newNode->treeNode = treeNode;
	newNode->next = list->head;

	list->head = newNode;
	if (!list->tail){
		list->tail = newNode;
	}
	else{
		//nothing
	}
	list->count++;
	return;
}

ExpressionTreeNode *removeTail_ExpressionTreeNode_List(ExpressionTreeNode_List *list){
	//if (!list) return NULL; //should never happen anyway

	ExpressionTreeNode_ListNode *tail = list->tail;
	//if ()

	//fuck this, too slow
	return NULL;
}

ExpressionTreeNode *removeHead_ExpressionTreeNode_List(ExpressionTreeNode_List *list){
	//if (!list) return NULL; //should never happen anyway

	ExpressionTreeNode_ListNode *head = list->head;
	if (!head) return NULL; //empty list
	if (list->count==1){ //list->head == list->tail
		list->head = NULL;
		list->tail = NULL;
		list->count = 0;
	}
	else{
		list->head = head->next;
		list->count--;
	}
	
	ExpressionTreeNode *resNode = head->treeNode;
	free(head);

	return resNode;
}

void free_ExpressionTreeNode_List(ExpressionTreeNode_List list){
	if (!list.head) return;
	ExpressionTreeNode_ListNode* next = NULL;
	while (list.head){
		next = list.head->next;
		free_ExpressionTreeNode(list.head->treeNode);
		free(list.head);
		list.head = next;
	}
	return;
}



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


