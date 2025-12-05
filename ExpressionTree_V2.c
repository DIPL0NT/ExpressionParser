#include "ExpressionToken.c"

typedef enum{LIST_NODE,INCOMPLETE,OPERAND_NODE,OPERATOR_NODE} ExpressionTreeNodeType;
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
    ExpressionTreeNodeType type;
	ExpressionTreeNode_List args;    //list of child nodes
    ExpressionToken token;
} ExpressionTreeNode;

ExpressionTreeNode *alloc_ExpressionTreeNode(ExpressionTreeNode *root,ExpressionTokenType type,ExpressionToken tok){
	ExpressionTreeNode *newNode = (ExpressionTreeNode*) malloc(sizeof(ExpressionTreeNode));
	//if (!newNode) ...
	newNode->root = root;
    newNode->type = type;
	newNode->token = tok;
	newNode->args = create_ExpressionTreeNode_List();
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

//doesn't modify list2
void concat_ExpressionTreeNode_List(ExpressionTreeNode_List *list1,ExpressionTreeNode_List *list2){
	if (!list1 || !list2){
		//printf ...
		return;
	}

	if (list1->head==NULL){
		list1->head = list2->head;
		list1->tail = list2->tail;
		list1->count = list2->count;
		return;
	}

	list1->tail->next = list2->head;
	list1->tail = list2->tail ? list2->tail : list1->tail ;
	list1->count += list2->count;

	return;
}




