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

ExpressionTreeNode_List create_ExpressionTreeNode_List(){
	ExpressionTreeNode_List list;
	list.head = NULL;
	list.tail = NULL;
	list.count = 0;
	return list;
}

void add_to_ExpressionTreeNode_List(ExpressionTreeNode_List *list,ExpressionTreeNode *treeNode){
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

ExpressionTreeNode *alloc_ExpressionTreeNode(ExpressionTreeNode *root,ExpressionElement el){
	ExpressionTreeNode *newNode = (ExpressionTreeNode*) malloc(sizeof(ExpressionTreeNode));
	//if (!newNode) ...
	newNode->root = root;
	newNode->element = el;
	newNode->args.head = NULL; //{NULL,0};
	newNode->args.count = 0;
	return newNode;
}

void free_ExpressionTreeNode(ExpressionTreeNode* node){
	//if (!node) ...
	free_ExpressionTreeNode_List(node->args);
	release_ExpressionElement(node->element);
	free(node);
	return;
}

//Maybe implement ExpressionTreeNode.args as a list of nodes that keeps track of its size
// so that args can be added before encountering the operator in case of INFIX or POSTFIX operators,
// when encountering the operator compare operator arity and current args list size

ExpressionTreeNode *create_ExpressionTree_from_ExpressionString(ExpressionString *es,ExpressionTreeNode *root){
	



}



