#include "ExpressionToken.c"


typedef struct ExpressionTreeNode ExpressionTreeNode;

typedef struct ExpressionTreeNode_ListNode{
	ExpressionTreeNode *treeNode;
	struct ExpressionTreeNode_ListNode *next;
	struct ExpressionTreeNode_ListNode *prev;
} ExpressionTreeNode_ListNode;

typedef struct ExpressionTreeNode_List{
	ExpressionTreeNode_ListNode *head;
	ExpressionTreeNode_ListNode *tail;
	int count;
} ExpressionTreeNode_List;

typedef struct ExpressionTreeNode{
	struct ExpressionTreeNode *root; //parent node
	ExpressionToken token;
	ExpressionTreeNode_List args;   //list of child nodes
	                                //, ATTENTION: nodes get added to the head, so the head is always the right most argument. Have to pass args to functions in inverse order than the one in the list
} ExpressionTreeNode;

ExpressionTreeNode *alloc_ExpressionTreeNode(ExpressionTreeNode *root,ExpressionToken tok){
	ExpressionTreeNode *newNode = (ExpressionTreeNode*) malloc(sizeof(ExpressionTreeNode));
	//if (!newNode) ...
	newNode->root = root;
	newNode->token = tok;
	newNode->args.head = NULL; //{NULL,0};
	newNode->args.count = 0;
	return newNode;
}

//implemented further down
//damned circular dependencies
void free_ExpressionTreeNode_List(ExpressionTreeNode_List *list);

void free_ExpressionTreeNode(ExpressionTreeNode* node){
	//if (!node) ...
	free_ExpressionTreeNode_List(&node->args);
	release_ExpressionToken(node->token);
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
	newNode->prev = list->tail;

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
	newNode->prev = NULL;

	if (list->head){
		list->head->prev = newNode;
	}
	list->head = newNode;
	if (!list->tail){
		list->tail = newNode;
	}
	
	list->count++;
	return;
}

ExpressionTreeNode *removeTail_ExpressionTreeNode_List(ExpressionTreeNode_List *list){
	//if (!list) return NULL; //should never happen anyway

	ExpressionTreeNode_ListNode *tail = list->tail;
	if (!tail) return NULL; //empty list
	if (list->count==1){ //list->head == list->tail
		list->head = NULL;
		list->tail = NULL;
		list->count = 0;
	}
	else{
		list->tail = tail->prev;
		list->tail->next = NULL;
		list->count--;
	}
	
	ExpressionTreeNode *resNode = tail->treeNode;
	free(tail);

	return resNode;
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
		list->head->prev = NULL;
		list->count--;
	}
	
	ExpressionTreeNode *resNode = head->treeNode;
	free(head);

	return resNode;
}

void free_ExpressionTreeNode_List(ExpressionTreeNode_List *list){
	if (!list->head) return;
	ExpressionTreeNode_ListNode* next = NULL;
	while (list->head){
		next = list->head->next;
		free_ExpressionTreeNode(list->head->treeNode);
		free(list->head);
		list->head = next;
	}
	//list->head = NULL;
	//list->tail = NULL;
	//list->count = 0;
	return;
}





void print_ExpressionTree(ExpressionTreeNode *tree){
	
}