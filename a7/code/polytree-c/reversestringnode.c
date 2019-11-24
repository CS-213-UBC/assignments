#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "node.h"
#include "stringnode.h"
#include "reversestringnode.h"

struct ReverseStringNode_class ReverseStringNode_class_table = {
	ReverseStringNode_compareTo,
	StringNode_printNode,
	Node_insert,
	Node_print,
	ReverseStringNode_delete,

};

void ReverseStringNode_actor(void* thisv, char* s) {
	struct ReverseStringNode* this = thisv;
	StringNode_ctor(this, s);
}

int ReverseStringNode_compareTo(void* thisv, void* nodev){
	struct StringNode* node = nodev;
	struct StringNode* this = thisv;
	return (-strcmp (this->s, node->s));
}


void ReverseStringNode_delete (void* thisv){
  struct ReverseStringNode* this = thisv;
  if(this->left != NULL){
    this->left->class->delete(this->left);
  }
  if(this->right != NULL){
    this->right->class->delete(this->right);
  }

  free(this);
}



void* new_ReverseStringNode(char* s){
	struct ReverseStringNode* obj = malloc(sizeof(struct ReverseStringNode));
	obj->class = & ReverseStringNode_class_table;
	ReverseStringNode_actor(obj, s);
	return obj;
}
// TODO implementation of method(s) that ReverseStringNode overrides