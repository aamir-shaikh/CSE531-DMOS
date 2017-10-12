// CSE 531 DMOS Assignment 1 Part 2 Submission

// Project submitted by: 
// Manjit Chakravarthy Manukonda, ASU ID : 1211306886,
// Aamir Ahmed Aboobakar Sadique Shaikh, ASU ID: 1211053906

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "sem.h"
#include <unistd.h>
#include <pthread.h>

#define N 3
semaphore_t empty[N], full, mutex;
int buffer[N];
int child_delay = 1, parent_delay = 1, cnt = 0;


void child (int *arg)
{
    while (1){ 
        P(&empty[*arg]);
          P(&mutex);
	    printf("Child %d: incrementing slot #%d\n", *arg, *arg);
            buffer[*arg]++;
	    cnt++;
	    if(cnt == N){
		V(&full);
	    }
        V(&mutex);
	sleep(child_delay);
    }
}    

void parent(int *arg)
{
    int id[N];
    for(int i=0; i<N; i++){
	id[i] = i;
	buffer[i] = 0;
	start_thread(child, &id[i]);
    }
    while(1){
        P(&full);
          P(&mutex);
	    printf("Parent %d: reading array\n", *arg);
	    for(int i=0; i<N; i++){
		printf("%d ",buffer[i]);
		V(&empty[i]);
	    }
	    cnt = 0;
	    printf("\n");
          V(&mutex);
        sleep(parent_delay);
    }    
}


int main()
{
    int id = 0;

    init_sem(&full, 0);
    for(int i=0; i<N; i++){
	init_sem(&empty[i], 1);
    }
    init_sem(&mutex, 1);

    start_thread(parent, &id);
    while (1) {
	sleep(1);
    };
}




