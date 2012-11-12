#include "anthills.h" 
#include <pthread.h>
#include <semaphore.h>

#define TRUE  1
#define FALSE 0

int initialized=FALSE; // semaphores and mutexes are not initialized 

// define your mutexes and semaphores here 

// hill state
sem_t hill[4];
sem_t ants[4];
int ants_left[4] = {20, 20, 20, 20};
int slurping[4]  = { 0,  0,  0,  0};


void eat(char name, int i)
{
  int semval;
  sem_getvalue(&hill[i], &semval);
  int full_slots = 3 - semval;

  if ((ants_left[i] > full_slots)  &&  (sem_trywait(&hill[i])) != -1) {
    if (ants_left[i] > 0) {
      slurp(name, i);
      int a = --ants_left[i];
      printf(">>>>>>>>>>>>>> HILL %d: %d\n", i, a);
    }
    sem_post(&hill[i]);
  }
}


void *my_thread(void *input) { 
  char aname = *(char *)input; // name of aardvark, for debugging
  while (chow_time()) { 
    int i = lrand48() % ANTHILLS;
    eat(aname, i);
  }
  return NULL; 
} 

// first thread initializes mutexes 
void *thread_A(void *input) { 
  if (!initialized) {
    // init semaphores
    sem_init(&hill[0], 0, 3);
    sem_init(&hill[1], 0, 3);
    sem_init(&hill[2], 0, 3);
    sem_init(&hill[3], 0, 3);

    // init mutexes
    sem_init(&ants[0], 0, 1);
    sem_init(&ants[1], 0, 1);
    sem_init(&ants[2], 0, 1);
    sem_init(&ants[3], 0, 1);

    initialized=TRUE;
  }  
  return my_thread(input); 
}

// other threads proceed after initialization
void *thread_B(void *input) { 
  while (!initialized);
  return my_thread(input); 
} 
void *thread_C(void *input) { 
  while (!initialized)
    return my_thread(input); 
} 
void *thread_D(void *input) { 
    while (!initialized);
    return my_thread(input); 
} 
void *thread_E(void *input) { 
    while (!initialized);
    return my_thread(input); 
} 
void *thread_F(void *input) { 
    while (!initialized);
    return my_thread(input); 
} 
void *thread_G(void *input) { 
    while (!initialized);
    return my_thread(input); 
} 
void *thread_H(void *input) { 
    while (!initialized);
    return my_thread(input); 
} 
void *thread_I(void *input) { 
    while (!initialized);
    return my_thread(input); 
} 
void *thread_J(void *input) { 
    while (!initialized);
    return my_thread(input); 
} 
void *thread_K(void *input) { 
    while (!initialized);
    return my_thread(input); 
} 

