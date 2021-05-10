/*Required Headers*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "rw.h"
#include "resource.h"

static resource_t data;
sem_t reader, writer;
int readers;


void initialize_readers_writer() {
    sem_init(&reader, 0, 1);
    sem_init(&writer, 0, 1);
    readers = 0;
    init_resource(&data ,"");
}


void rw_read(char *value, int len) {
    readers++;
    sem_wait(&reader);
    sem_post(&reader);
    read_resource(&data, value, len);
    readers--;
}


void rw_write(char *value, int len) {
    sem_wait(&writer);
    while(readers != 0){}
    sem_wait(&reader);
    write_resource(&data, value, len);
    sem_post(&reader);
    sem_post(&writer);
}
