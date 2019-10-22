#include <stdio.h>
#include "refcount.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct A {
    int* x;
};

struct A* create(int length) {
    struct A* a = rc_malloc(sizeof(a));       //*** [1]
    a->x = rc_malloc(sizeof(a->x) * length);  //*** [2]
    return a;
}

void foo(){
    int x = 0;
}

int main (int argc, char** argv) {
    struct A* l = create(100);
    foo(l);
    //**** [3]
    //**** [4]
  
}
