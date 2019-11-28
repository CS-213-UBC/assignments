#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include <time.h>
#include "uthread_mutex_cond.h"

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
 * problem: gatekeeper grants access: people of opposing endianness cannot be at the same well at the same time
 * no more than 3 people at the well at a time
 * gate keeper is fair: waiting times are roughly uniform, want to operate efficiently (not always fair)
 * balances fairness and efficiency
 * person = thread, well is a critical section protected by mutex, gatekeeper is a procedure called by each thread
 * when entering and leaving well 
 * thread waits on a condition variable
 * 
 * counter for no. of people
 * boolean var for thread
 * /

/**
 * You might find these declarations useful.
 */
enum Endianness {LITTLE = 0, BIG = 1, NONE = -1};
const static enum Endianness oppositeEnd [] = {BIG, LITTLE};

static int counter = 0;

static uthread_t threads[NUM_PEOPLE];
static uthread_mutex_t mx;
static uthread_cond_t can_enter[2];

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
uthread_mutex_t waitingHistogrammutex;
uthread_mutex_t occupancyHistorymutex;
int             occupancyHistogram       [2] [MAX_OCCUPANCY + 1];

void recordWaitingTime (int waitingTime) {
  uthread_mutex_lock (waitingHistogrammutex);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  uthread_mutex_unlock (waitingHistogrammutex);
}

void recordWellConditions(enum Endianness e){
  uthread_mutex_lock (occupancyHistorymutex);
  int num = Well->occupants[e];
  occupancyHistogram[e][num] ++;
  uthread_mutex_unlock (occupancyHistorymutex);
}

void enterWell (enum Endianness g) {
  uthread_mutex_lock(mx);
  counter++;
  int start = counter;
  if (Well->end == NONE){
    Well->end = g;
  }

  while(Well->totalOccupants >= 3 || Well->occupants[g] >= 3 || Well->end != g){ //waiting time
    Well->queue[g] ++; //increase number in queue
    uthread_cond_wait(can_enter[g]);
    //printf("finished waiting, now continue loop %d\n", start);
    Well->queue[g] --;
    if(Well->totalOccupants == 0){ //if all have left...
      Well->end = g;
    }
  }
  //now entered
  Well->totalOccupants ++;
  Well->occupants[g] ++;

  assert(Well->end == g);
  assert(Well->totalOccupants <= 3);
  assert(Well->occupants[g] <=3 );
  assert(Well->occupants[!g] == 0);
  recordWellConditions(g);
  recordWaitingTime(counter - start);
  uthread_mutex_unlock(mx);
}

void leaveWell() {
  uthread_mutex_lock(mx);

  //implement policy here....
  Well->totalOccupants --;
   Well->occupants[Well->end] --;
  if (Well->totalOccupants == 0) {    
    if (Well->queue[!Well->end] == 0){ //if other end is already 0
      uthread_cond_broadcast(can_enter[Well->end]);
    } else {
      uthread_cond_broadcast(can_enter[!Well->end]);
    }
  } else if (rand() % 5 == 0){
    if (Well->queue[Well->end] == 0){ //if end is already 0
      uthread_cond_broadcast(can_enter[!Well->end]);
    } else {
      uthread_cond_broadcast(can_enter[Well->end]);
    }
    
  }
  uthread_mutex_unlock(mx);
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
  uthread_init (4);
  Well = createWell();
  uthread_t pt [NUM_PEOPLE];
  waitingHistogrammutex = uthread_mutex_create ();
  occupancyHistorymutex = uthread_mutex_create ();
  mx = uthread_mutex_create ();

  can_enter[LITTLE] = uthread_cond_create(mx);
  can_enter[BIG] = uthread_cond_create(mx);

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
