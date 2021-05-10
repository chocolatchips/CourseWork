/*Required Headers*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "meetup.h"
#include "resource.h"


int group_num, group_order, group_size, current_group, meet;
int barrier_size;
pthread_cond_t cond, cond_2;
pthread_mutex_t mutex, mutex_2;
resource_t codeword;

void barrier(int);
void barrier_2(int, char*, int);

void initialize_meetup(int n, int mf) {

    if (n < 1) {
        fprintf(stderr, "Who are you kidding?\n");
        fprintf(stderr, "A meetup size of %d??\n", n);
        exit(1);
    }

    group_num = group_order = 0;
    current_group = barrier_size = 0;
    group_size = n;
    meet = mf == MEET_FIRST ? 0 : n-1;
    init_resource(&codeword, "");
    fprintf(stderr, "Group size %d\n", group_size);

    if (pthread_cond_init(&cond, NULL) != 0){
        fprintf(stderr, "Error initializing first condition variable");
        exit(1);
    }
    if (pthread_cond_init(&cond_2, NULL) != 0){
        fprintf(stderr, "Error initializing second condition variable");
        exit(1);
    }
    if(pthread_mutex_init(&mutex, NULL) != 0){
        fprintf(stderr, "Error initializing mutex");
        exit(1);
    }
    if(pthread_mutex_init(&mutex_2, NULL) != 0){
        fprintf(stderr, "Error initializing mutex");
        exit(1);
    }
    
}


void join_meetup(char *value, int len) {
    int my_group, my_order;
    if (group_order < group_size){
        my_order = group_order;
        group_order++;
    }
    else{
        my_order = 0;
        group_order = 1;
    }
    my_group = group_num;
    barrier(my_group);
    barrier_2(my_order, value, my_group);
}


/*
 * 
 * Method based on example from slide 73 of Synchronization presentation
 * 
*/
void barrier(int my_group){
    pthread_mutex_lock(&mutex);
    barrier_size++;
    if (barrier_size < group_size){
        while(my_group == group_num){
            pthread_cond_wait(&cond, &mutex);
        }
    }
    else{
        barrier_size = 0;
        group_num++;
        pthread_cond_broadcast(&cond);
    }
    pthread_mutex_unlock(&mutex);
}


void barrier_2(int my_order, char *value, int my_group){
    pthread_mutex_lock(&mutex_2);
    while (my_group > current_group){
        pthread_cond_wait(&cond_2, &mutex_2);
    }
    if (my_order == meet){
        write_resource(&codeword, value, MAX_VALUE_LEN);
        current_group++;
        pthread_cond_broadcast(&cond_2);
    }
    else{
        while (my_group == current_group){
            pthread_cond_wait(&cond_2, &mutex_2);
        }
        read_resource(&codeword, value, MAX_VALUE_LEN);
        pthread_cond_broadcast(&cond_2);
    }

    pthread_mutex_unlock(&mutex_2);
}