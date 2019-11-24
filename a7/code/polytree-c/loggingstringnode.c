#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "node.h"
#include "stringnode.h"
#include "loggingstringnode.h"

struct LoggingStringNode_class LoggingStringNode_class_table = {
	StringNode_compareTo,
	StringNode_printNode,
	LoggingStringNode_insert,
	Node_print,	
	LoggingStringNode_delete,
};

void LoggingStringNode_ctor (void* thisv, char* s){
	struct LoggingStringNode* this = thisv;
	StringNode_ctor(this, s);
}

void LoggingStringNode_insert(void* thisv, void* nodev) {
	struct LoggingStringNode* this = thisv;
	struct LoggingStringNode* node = nodev;
	Node_insert(this, node);
	printf("insert " );
	node->class->printNode(node); 
}
void LoggingStringNode_delete(void* thisv){
  struct LoggingStringNode* this = thisv;
  if(this->left != NULL){
    this->left->class->delete(this->left);
  }
  if(this->right != NULL){
    this->right->class->delete(this->right);
  }

  free(this);
}

void* new_LoggingStringNode(char* s) {
	struct LoggingStringNode* obj = malloc(sizeof(struct LoggingStringNode));
	obj->class = &LoggingStringNode_class_table;
	LoggingStringNode_ctor(obj, s);
	return obj;
}