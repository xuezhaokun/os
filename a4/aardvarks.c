#include "anthills.h" 
#include <pthread.h>
#include <semaphore.h>

#define TRUE  1
#define FALSE 0


int initialized = FALSE;
sem_t hill[ANTHILLS];
int slurping[ANTHILLS];
int ants_left[ANTHILLS];
pthread_t unlocker;

int slurp_hill[AARDVARKS];
double slurp_time[AARDVARKS];

/**
 * Manages the eating in a thread safe way.
 *
 * Manages the number of aardvarks on a hill and the number of ants on
 * a hill for optimality.
 */
void eat(char name, int i)
{
  int avark = name - 'A';

  if ((ants_left[i] > slurping[i])  &&  (sem_trywait(&hill[i])) != -1) {
    ++slurping[i];
    slurp_hill[avark] = i;
    slurp_time[avark] = elapsed() + 1.;
    slurp(name, i);
    --ants_left[i];
    --slurping[i];
    sem_post(&hill[i]);
    slurp_hill[avark] = -1;
  }
}

void* unlock_sempahores(void* arg)
{
  int i;
  while(TRUE) {
    printf("------------------------------\n");
    printf("ELAPSED: %f\n", elapsed());
    for(i = 0; i < AARDVARKS; ++i)
      printf("%2d: HILL %2d TIME %f\n", i, slurp_hill[i], slurp_time[i]);
    sleep(1);
  }
}


/**
 * Thread code. Simply calls eat while there are ants left.
 */
void *my_thread(void *input) { 
  char aname = *(char*) input;
  while (chow_time())
    eat(aname, lrand48() % ANTHILLS);
  return NULL; 
} 

/**
 * Initializes state and then runs thread code.
 */
void *thread_A(void *input) { 
  if (!initialized) {
    int i;
    for (i = 0; i < ANTHILLS; ++i) {
      ants_left[i] = ANTS_PER_HILL;
      slurping[i] = 0;
      sem_init(&hill[i], 0, AARDVARKS_PER_HILL);
    }

    pthread_create(&unlocker, NULL, unlock_sempahores, NULL);

    for(i = 0; i < AARDVARKS; ++i) {
      slurp_hill[i] = -1;
      slurp_time[i] =  0;
    }

    initialized = TRUE;
  }  
  return my_thread(input); 
}

//////////////////////////////////////////////////////////////////////
// Other threads
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

