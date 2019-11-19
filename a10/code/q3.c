#include <stdlib.h>
#include <stdio.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#define NUM_THREADS 3
uthread_t threads[NUM_THREADS];
uthread_mutex_t mx; 
uthread_cond_t printing_a; 

void randomStall() {
  int i, r = random() >> 16;
  while (i++<r);
}

void waitForAllOtherThreads() {
  
  //uthread_mutex_lock(mx);
  uthread_cond_wait(printing_a);
  //uthread_mutex_unlock(mx);
  
}

void* p(void* v) {
  
  printf("a\n");
  uthread_mutex_lock(mx);
  waitForAllOtherThreads();
  uthread_mutex_unlock(mx);

  printf("b\n");
  //uthread_mutex_lock(mx);
  uthread_cond_broadcast(printing_a);
  uthread_cond_signal(printing_a);

  
  //uthread_cond_signal(printing_a);
  //uthread_mutex_unlock(mx);
  return NULL;
}

int main(int arg, char** arv) {
  uthread_init(4);
  mx = uthread_mutex_create();
  printing_a = uthread_cond_create(mx);
  for (int i=0; i<NUM_THREADS; i++)
    threads[i] = uthread_create(p, NULL);
  
  for (int i=0; i<NUM_THREADS; i++)
    uthread_join (threads[i], NULL);
  printf("------\n");
}