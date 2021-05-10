#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

#define MAX_BUFFER_LEN 80

taskval_t *event_list = NULL;
/*
FILE *results;
float wait_times[1000];
float turnaround_times[1000];

void print_avg(){
    float wait_avg = 0;
    float turn_avg = 0;
    for (int i = 0; i < 1000; i++){
        wait_avg += wait_times[i];
        turn_avg += turnaround_times[i];
    }
    wait_avg = wait_avg/1000;
    turn_avg = turn_avg/1000;
    fprintf(results, "%f, %f\n", wait_avg, turn_avg);
}

void print_task(taskval_t *t, void *arg) {
    printf("task %03d: %5d %3.2f %3.2f\n",
        t->id,
        t->arrival_time,
        t->cpu_request,
        t->cpu_used
    );  
}
*/

void increment_count(taskval_t *t, void *arg) {
    int *ip;
    ip = (int *)arg;
    (*ip)++;
}

void run_simulation(int qlen, int dlen) {
    taskval_t *ready_q = NULL;
    int current = 0;
    int quant, dis, run;
    quant = run = 0;
    dis = dlen;
    int idle = 0;
    while(event_list != NULL || ready_q != NULL){
        fprintf(stderr, "[%05d] ", quant);
        if (event_list != NULL && quant == event_list->arrival_time){
            taskval_t *temp = remove_front(event_list);
            ready_q = add_end(ready_q, event_list);
            event_list = temp;
        }
        if (ready_q == NULL){
            fprintf(stderr,"IDLE\n");
            idle++;
        }
        else{
            idle = 0;
            if (dis > 0){
                fprintf(stderr, "DISPATCHING\n");
                dis--;
            }
            else{
                if (run == qlen){
                    dis = dlen-1;
                    run = 0;
                    fprintf(stderr, "DISPATCHING\n");
                    taskval_t *temp = ready_q;
                    ready_q = remove_front(ready_q);
                    ready_q = add_end(ready_q, temp);
                }
                else{
                    fprintf(stderr, "id=%05d ", ready_q->id);
                    if (ready_q->cpu_used >= ready_q->cpu_request){
                        float turn = quant - ready_q->arrival_time;
                        float wait = turn - ready_q->cpu_request;
                        fprintf(stderr, "EXIT w=%.2f ta=%.2f\n", wait, turn);
                        //wait_times[current] = wait;
                        //turnaround_times[current] = turn;
                        current++;
                        //fprintf(results, "%.2f, %.2f\n", wait, turn);
                        taskval_t *temp = ready_q;
                        ready_q = remove_front(ready_q);
                        end_task(temp);
                        run = -1;
                        dis = dlen;
                        quant--;
                    }
                    else{
                        fprintf(stderr, "req=%.2f used=%.2f\n", ready_q->cpu_request, ready_q->cpu_used);
                        ready_q->cpu_used++;
                        run++;
                    }
                }
            }
        }
        quant++;
    }
}


int main(int argc, char *argv[]) {
    char   input_line[MAX_BUFFER_LEN];
    int    i;
    int    task_num;
    int    task_arrival;
    float  task_cpu;
    int    quantum_length = -1;
    int    dispatch_length = -1;
    //char   filename[100];

    taskval_t *temp_task;
    
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--quantum") == 0 && i+1 < argc) {
            quantum_length = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "--dispatch") == 0 && i+1 < argc) {
            dispatch_length = atoi(argv[i+1]);
        }
        /*else if (strcmp(argv[i], "--file") == 0 && i+1 < argc) {
            strcpy(filename, argv[i+1]);
        }*/
    }

    if (quantum_length == -1 || dispatch_length == -1) {
        fprintf(stderr, 
            "usage: %s --quantum <num> --dispatch <num>\n",
            argv[0]);
        exit(1);
    }
    //strcat(filename, ".csv");
    //results = fopen(filename, "a");
    //fprintf(results, "Wait Time, Turnaround Time, Quantum = %d, Dispatch = %d\n", quantum_length, dispatch_length);
    while(fgets(input_line, MAX_BUFFER_LEN, stdin)) {
        sscanf(input_line, "%d %d %f", &task_num, &task_arrival,
            &task_cpu);
        temp_task = new_task();
        temp_task->id = task_num;
        temp_task->arrival_time = task_arrival;
        temp_task->cpu_request = task_cpu;
        temp_task->cpu_used = 0.0;
        event_list = add_end(event_list, temp_task);
    }
/*
#ifdef DEBUG
    int num_events;
    apply(event_list, increment_count, &num_events);
    printf("DEBUG: # of events read into list -- %d\n", num_events);
    printf("DEBUG: value of quantum length -- %d\n", quantum_length);
    printf("DEBUG: value of dispatch length -- %d\n", dispatch_length);
#endif
*/
    run_simulation(quantum_length, dispatch_length);
    //print_avg();
    return (0);
}
