# rw.c

### rw_read

This method first increases the number of readers waiting before waiting for the readers semaphore to become available. The thread then releases the semaphore so multiple readers can read the resource simultaneously before redducing the number of readers once finished.

### rx_write

This method enforces the reader priority by having the writers wait in a while loop until there are no readers waiting. Once the writer begins to write the readers then wait until the writing is completed.

# meetup.c

### initialize_meetup

I initialize two condition variables and mutexes to enable the use of two barriers.

### join_meetup

The method assigns an specific order in the group to the thread as it comes in. The threads then pass into the first barrier until the specified group size is achieved. Once released from the first barrier the threads enter the second barrier before finishing.

### barrier

The method is based off the example on slide 73 of presentation 5 "Synchronization." The threads are kept in the method while the group is waiting for enough threads to match the specified group size. Once the final thread in the group arrives the barrier releases all threads to continue processing.

### barrier_2

The method first traps any threads that are in a group that entered the barrier before the current group finishes their reads and writes. The current group continues split the thread specified by MEETFIRST or MEETLAST. This separated thread then writes the resource before iterating current group and releasing the other threads.
The read threads wait until the current group has been iterated by the writing thread. Once released the threads read the resource before broadcasting to the condition lock to ensure all read threads leave before the next group starts.
