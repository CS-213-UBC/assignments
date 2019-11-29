#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_sem.h"
#include "uthread_mutex_cond.h"
#include <time.h>

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

#define MAX_OCCUPANCY      3
#define NUM_ITERATIONS     100
#define N                  20
#define NUM_PEOPLE         20
#define FAIR_WAITING_COUNT 4

/**
 * You might find these declarations useful.
 */
enum Endianness {LITTLE = 0, BIG = 1, NONE = -1};
const static enum Endianness oppositeEnd [] = {BIG, LITTLE};

static int counter = 0;

static uthread_t threads[NUM_PEOPLE];
static uthread_mutex_t mx;
static uthread_sem_t can_enter[2];
static uthread_sem_t right_end[2];
static uthread_sem_t mx_sem;

struct Well {
  enum Endianness end; //current occupancy
  int totalOccupants; //number of occupants in well
  int occupants[2]; //number of people in well for each End
  int queue[2]; //number of people in queues for each End
};

struct Well* createWell() {
  struct Well* Well = malloc (sizeof (struct Well));
  Well->end = NONE;
  Well->totalOccupants = 0;

  Well->occupants[LITTLE] = 0;
  Well->occupants[BIG] = 0;

  Well->queue[LITTLE] = 0;
  Well->queue[BIG] = 0;

  return Well;
}

struct Well* Well;

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_PEOPLE)
int             entryTicker;                                          // incremented with each entry
int             waitingHistogram         [WAITING_HISTOGRAM_SIZE];
int             waitingHistogramOverflow;
uthread_sem_t waitingHistogrammutex;
uthread_sem_t occupancyHistorymutex;
int             occupancyHistogram       [2] [MAX_OCCUPANCY + 1];

void recordWaitingTime (int waitingTime) {
  uthread_sem_wait (waitingHistogrammutex);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  uthread_sem_signal (waitingHistogrammutex);
}

void recordWellConditions(enum Endianness e){
  uthread_sem_wait (occupancyHistorymutex);
  int num = Well->occupants[e];
  occupancyHistogram[e][num] ++;
  uthread_sem_signal (occupancyHistorymutex);
}

void enterWell (enum Endianness g) {
  //uthread_mutex_lock(mx);
  uthread_sem_wait(mx_sem);
 

  counter++;
  int start = counter;
  //printf("tried to enter the well %d, end is %d\n", start, g);
  if (Well->end == NONE){
    Well->end = g;
    uthread_sem_signal(can_enter[g]);
    uthread_sem_signal(can_enter[g]);
    uthread_sem_signal(can_enter[g]);
  }

  //if (Well->totalOccupants >= 3 || Well->occupants[g] >= 3 || Well->end != g){ //waiting time
  Well->queue[g] ++; //increase number in queue
  //uthread_cond_wait(can_enter[g]);
  uthread_sem_signal(mx_sem);

  uthread_sem_wait(can_enter[g]);
  uthread_sem_wait(mx_sem);

  printf("end %d officially entered the well %d\n", g, start);
  Well->queue[g] --;
  //}
  //now entered
  Well->totalOccupants ++;
  Well->occupants[g] ++;

  assert(Well->end == g);
  assert(Well->totalOccupants <= 3);
  assert(Well->occupants[g] <=3 );
  assert(Well->occupants[!g] == 0);
  recordWellConditions(g);
  recordWaitingTime(counter - start);

  printf("finished mx %d\n", start);

  uthread_sem_signal(mx_sem);
}

void leaveWell() {
  uthread_sem_wait(mx_sem);
  printf("LEaving well \n");
  printf("current no in queue for little is %d\n", Well->queue[LITTLE]);
  printf("current no in queue for big is %d\n", Well->queue[BIG]);
  //uthread_mutex_lock(mx);

  //implement policy here....
  Well->totalOccupants --;
   Well->occupants[Well->end] --;
  if (Well->totalOccupants == 0) {  

    if (Well->queue[!Well->end] > 0){
      Well->end = !Well->end;
    }
    uthread_sem_signal(can_enter[Well->end]);
    uthread_sem_signal(can_enter[Well->end]);
    uthread_sem_signal(can_enter[Well->end]);
    
  } else if (rand() % 5 == 0){ 
    if (Well->queue[Well->end] == 0){ //if end is already 0
      //Well->end = !Well->end;
      printf("HI");
    }
    //uthread_sem_signal(can_enter[Well->end]);
    //uthread_sem_wait(can_enter[!Well->end]);

  }

  uthread_sem_signal(mx_sem);
}

void leaveWell_Old() {
  uthread_sem_wait(mx_sem);
  //uthread_mutex_lock(mx);

  //implement policy here....
  Well->totalOccupants --;
   Well->occupants[Well->end] --;
  if (Well->totalOccupants == 0) {  
      
    if (Well->queue[!Well->end] == 0){ //if other end is already 0
      //uthread_cond_broadcast(can_enter[Well->end]);
      uthread_sem_signal(can_enter[Well->end]);
    } else {
      //uthread_cond_broadcast(can_enter[!Well->end]);
      uthread_sem_signal(can_enter[!Well->end]);
    }
  } else if (rand() % 5 == 0){
    if (Well->queue[Well->end] == 0){ //if end is already 0
      //uthread_cond_broadcast(can_enter[!Well->end]);
      uthread_sem_signal(can_enter[!Well->end]);
    } else {
      //uthread_cond_broadcast(can_enter[Well->end]);
      uthread_sem_signal(can_enter[Well->end]);
    }
    
  }
  //uthread_mutex_unlock(mx);
  uthread_sem_signal(mx_sem);
}



void* createPerson(void* dunno){
  enum Endianness end = rand() % 2; //LITTLE OR BIG
  for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
    enterWell(end);
    for (int i = 0; i < N; i++) {
      uthread_yield();
    }
    leaveWell();
    for (int i = 0; i < N; i++) {
      uthread_yield();
    }
  }
  return NULL;
}

int main (int argc, char** argv) {
  srand(time(NULL));
  uthread_init (1);
  Well = createWell();
  uthread_t pt [NUM_PEOPLE];
  waitingHistogrammutex = uthread_sem_create(1);
  occupancyHistorymutex = uthread_sem_create(1);
  mx_sem = uthread_sem_create(1);

  //general = uthread_sem_create (1);
  //mx = uthread_mutex_create (0);

  can_enter[LITTLE] = uthread_sem_create(0);
  can_enter[BIG] = uthread_sem_create(0);

  //right_end[LITTLE] = uthread_sem_create(0);
  //right_end[BIG] = uthread_sem_create(0);

  for (int i = 0; i < NUM_PEOPLE; i++){ //creating threads
    pt[i] = uthread_create(createPerson, (void*) 0);
  }

  // TODO
  enum Endianness g = Well->end;

  for (int i = 0; i < (sizeof pt) / (sizeof pt[0]); i++) {
    uthread_join(pt[i], NULL);
  }
  printf ("Times with 1 little endian %d\n", occupancyHistogram [LITTLE]   [1]);
  printf ("Times with 2 little endian %d\n", occupancyHistogram [LITTLE]   [2]);
  printf ("Times with 3 little endian %d\n", occupancyHistogram [LITTLE]   [3]);
  printf ("Times with 1 big endian    %d\n", occupancyHistogram [BIG] [1]);
  printf ("Times with 2 big endian    %d\n", occupancyHistogram [BIG] [2]);
  printf ("Times with 3 big endian    %d\n", occupancyHistogram [BIG] [3]);
  printf ("Waiting Histogram\n");
  for (int i=0; i<WAITING_HISTOGRAM_SIZE; i++)
    if (waitingHistogram [i])
      printf ("  Number of times people waited for %d %s to enter: %d\n", i, i==1?"person":"people", waitingHistogram [i]);
  
  if (waitingHistogramOverflow)
    printf ("  Number of times people waited more than %d entries: %d\n", WAITING_HISTOGRAM_SIZE, waitingHistogramOverflow);



}
