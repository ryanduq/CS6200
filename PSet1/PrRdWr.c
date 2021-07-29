#include "PrRdWr.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// CONSTANTS
#define NUM_READERS 5
#define NUM_WRITERS 5
#define NUM_READS 5
#define NUM_WRITES 5

// GLOBAL SHARED DATA
unsigned int gSharedValue = 0;
pthread_mutex_t gSharedMemoryLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gReadPhase = PTHREAD_COND_INITIALIZER;
pthread_cond_t gWritePhase = PTHREAD_COND_INITIALIZER;
int gWaitingReaders = 0,  gReaders = 0;


int main(void)
{
    // Run the server here

    int readerNums[NUM_READERS];
    int writerNums[NUM_READERS];
    
    pthread_t readerThreadIDs[NUM_READERS];
    pthread_t writerThreadIDs[NUM_WRITERS];

    srandom((unsigned int)time(NULL));

    // Start the readers
    for(int i = 0; i < NUM_READERS; i++)
    {
        readerNums[i] = i;
        pthread_create(&readerThreadIDs[i],NULL,reader,&readerNums[i]);
    }

    // Start the writers
    for(int i = 0; i < NUM_READERS; i++)
    {
        writerNums[i] = i;
        pthread_create(&writerThreadIDs[i],NULL,writer,&writerNums[i]);
    }

    // Wait for reader to finish
    for(int i = 0; i < NUM_READERS; i++)
    {
        pthread_join(readerThreadIDs[i],NULL);
    }

    // Wait for writer to finish
    for(int i = 0; i < NUM_READERS; i++)
    {
        pthread_join(writerThreadIDs[i],NULL);
    }

    return 0;
}


void *reader(void *arg)
{
    // Run a reader thread; priority over writer
    int id = *(int*)arg;
    int numReaders = 0;

    for (int i = 0; i < NUM_READS; i++)
    {
        usleep(1000 * (random() % NUM_READERS + NUM_WRITERS));

        // Enter critical section
        pthread_mutex_lock(&gSharedMemoryLock);
            gWaitingReaders++;
            while(gReaders == -1)
            {
                pthread_cond_wait(&gReadPhase,&gSharedMemoryLock);
            }
            gWaitingReaders--;
            numReaders = ++gReaders;
        pthread_mutex_unlock(&gSharedMemoryLock);

        // Read shared data
        printf("Thread [r%d] reads shared value of %2d. There are currently %d readers.\n",id,gSharedValue,numReaders);

        // Exit critical section
        pthread_mutex_lock(&gSharedMemoryLock);
            gReaders--;
            if(gReaders == 0)
            {
                pthread_cond_signal(&gWritePhase);
            }
        pthread_mutex_unlock(&gSharedMemoryLock);
    }

    pthread_exit(0);
}


void *writer(void *arg)
{
    // Run a writer thread
    int id = *(int*)arg;
    int numReaders = 0;

    for (int i = 0; i < NUM_READS; i++)
    {
        usleep(1000 * (random() % NUM_READERS + NUM_WRITERS));

        // Enter critical section
        pthread_mutex_lock(&gSharedMemoryLock);
            while(gReaders != 0)
            {
                pthread_cond_wait(&gWritePhase, &gSharedMemoryLock);
            }
            gReaders = -1;
            numReaders = gReaders;
        pthread_mutex_unlock(&gSharedMemoryLock);

        // Write data
        printf("Thread [r%d] writes value of %2d. There are currently %d readers.\n",id,++gSharedValue,numReaders);

        // Exit critical section
        pthread_mutex_lock(&gSharedMemoryLock);
            gReaders = 0;
            if(gWaitingReaders > 0)
            {
                pthread_cond_broadcast(&gReadPhase);
            }
            else
            {
                pthread_cond_signal(&gWritePhase);
            }
            
        pthread_mutex_unlock(&gSharedMemoryLock);
    }

    pthread_exit(0);
}