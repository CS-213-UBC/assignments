#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void* rc_malloc (int nbytes) {
  // allocate nbytes plus room for count, retaining alignment
  int* ref_count = malloc (nbytes + 8);
  // initialize ref count to 1;
  *ref_count = 1;
  // return pointer to client portion of allocation
  return ((void*) ref_count) + 8;
}

void  rc_keep_ref(void* p) {
  // get pointer to ref count from client pointer
  int* ref_count = p - 8;
  // increment ref count
  (*ref_count) ++;
}

void  rc_free_ref(void* p) {
  // get pointer to ref count from client pointer
  int* ref_count = p - 8;
  // decrement ref count
  (*ref_count) --;
  // free allocation when ref count is 0
  if (*ref_count == 0)
    free (ref_count);
}

void print_rc(void* p){
  int* ref_count = p - 8;
  printf("%d", *ref_count);

}

int* create(int length) {
    int* result = rc_malloc(sizeof(*result) * length);
    //**** [1] result?
    print_rc(result);
    printf("%d", 3);
    
    return result;
}

void dothis(int i) {
    int* x = create(1);
    //**** [5] x?
    //print_rc(x);
    *x = i;
    processhi(x);
    //**** [6] x?
    rc_free_ref(x);
}

int* even = NULL;
int* odd  = NULL;

void processhi(int* a) {
    if (*a & 1 == 0) {
        if (even != NULL)
            rc_free_ref(even);
        even = a;
    } else {
        if (odd != NULL) {
            //**** [2] odd?
            rc_free_ref(odd);
        }
        //**** [3] odd?
        odd = a;
    }
    rc_keep_ref(a);
    //**** [4] a?
}




void main (int argc, char** argv) {
    printf("%d", 0);

    for (int i=1; i<argc; i++){
        dothis(atoi(argv[i]));
    }
        
}