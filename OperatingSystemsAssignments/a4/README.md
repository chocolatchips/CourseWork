# virtmem.c

## Data Structures

### Hash Table

The hash table I created is an array of lists to speed up the searching. The table uses lists to simplify the collisions.

### List

I used a list of nodes that primarily act as a queue except for the LRU replacement scheme. The difference is that the LRU scheme will take a node from anywhere within the list and place it at the end under certain situations while the other schemes only change the positions of the front and end nodes.


## Algorithms

### FIFO
For this scheme I create a hash table and a queue. When the page number is passed into the scheme, the hash table is searched for an existing entry. If the entry is found, the algorithm moves onto find the effective value. If an entry is not found then the node at the front of the queue is removed with its corresponding node in the hash table is removed as well. A new node is created for both the queue and table with the node for the queue is placed at the end.

### Second Chance
For this scheme I use the same base structure as the FIFO algorithm with a couple excemptions. For the search if a page is currently in the queue the node->second value is changed to 1. This allows the remove function to check the front node of the queue's second value for a 1. If the value is equal to 1 the value is changed to 0 before moving to the end of the queue. This repeats until a 0 is found for the front value.

### Least Recently Used
For this scheme I use a list where the front value is still removed when the list is full. This works because when a value is searched for and found in the list, the corresponding node is moved to the end of the list. By constantly rotating the nodes to the end when used, the least recently used page is always at the front of the list.