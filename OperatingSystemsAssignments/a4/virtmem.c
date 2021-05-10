/*
 * Skeleton code for CSC 360, Spring 2021,  Assignment #4
 *
 * Prepared by: Michael Zastre (University of Victoria) 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/*
 * Some compile-time constants.
 */

#define REPLACE_NONE 0
#define REPLACE_FIFO 1
#define REPLACE_LRU  2
#define REPLACE_SECONDCHANCE 3
#define REPLACE_OPTIMAL 4


#define TRUE 1
#define FALSE 0
#define PROGRESS_BAR_WIDTH 60
#define MAX_LINE_LEN 100


/*
 * Some function prototypes to keep the compiler happy.
 */
int setup(void);
int teardown(void);
int output_report(void);
long resolve_address(long, int);
void error_resolve_address(long, int);


/*
 * Variables used to keep track of the number of memory-system events
 * that are simulated.
 */
int page_faults = 0;
int mem_refs    = 0;
int swap_outs   = 0;
int swap_ins    = 0;


/*
 * Page-table information. You may want to modify this in order to
 * implement schemes such as SECONDCHANCE. However, you are not required
 * to do so.
 */
struct page_table_entry *page_table = NULL;
struct page_table_entry {
    long page_num;
    int dirty;
    int free;
};

/*
 * These global variables will be set in the main() function. The default
 * values here are non-sensical, but it is safer to zero out a variable
 * rather than trust to random data that might be stored in it -- this
 * helps with debugging (i.e., eliminates a possible source of randomness
 * in misbehaving programs).
 */

int size_of_frame = 0;  /* power of 2 */
int size_of_memory = 0; /* number of frames */
int page_replacement_scheme = REPLACE_NONE;


/*
 * Node used for list in hash table and queue
 */
typedef struct node{
    struct node *next;
    long value;
    int second;
}node;

/*
 * Hash table used for search of current pages
 */
struct Hash_table{
    node **items;
    int count;
}*hash_table;

node *head;
node *tail;
int count;

int hash(long);
node *create_node(long);
void create_hash_table();

void remove_hash(long);
node *remove_front(node *);
node *remove_node(node *, long);

node *add_node_end(node *, node *);
void insert_hash(long);

int search_hash(long);
int search_list(node *, long);

void free_hash_list(node *);
void free_table();
void free_list();
void free_node(node *);

void second_chance(long);
void first_in(long);
void least_recent_used(long);
node *rotate_list(long , node *);

/*
 * Funciton allocates memory for node and assigns values
 */
node *create_node(long value){
    node *item = (node *) malloc (sizeof(node));
    item->next = NULL;
    item->value = value;
    item->second = FALSE;
    return item;
}
/*
 * Simple hash to place every node into table
 */
int hash(long key){
    return key % size_of_memory;
}

/*
 * Function allocates memory for a hash table with lists to handle collisions
 */
void create_hash_table(){
    hash_table = (struct Hash_table *) malloc (sizeof(struct Hash_table));
    hash_table->items = (node **) calloc (size_of_memory, sizeof(node *));
    if (hash_table == NULL){
        fprintf(stderr, "Simulator error: cannot allocate memory for hash table.\n");
        exit(1);
    }
    for (int i = 0; i < size_of_memory; i++){
        hash_table->items[i] = NULL;
    }
    hash_table->count = 0;
}

/*
 * Deallocates memory for nodes
 */
void free_node(node *item){
    free(item);
}

/*
 * Deallocates memory for the queue of current pages
 */
void free_list(){
    node *temp;
    while(head != NULL){
        temp = head->next;
        free_node(head);
        head = temp;
    } 
}

/*
 * Deallocates memory for a list in a hash list
 */
void free_hash_list(node *list){
    node *temp;
    while (list != NULL){
        temp = list->next;
        free_node(list);
        list = temp;
    }
}

/*
 * Deallocates memory for hash table and all lists within the table
 */
void free_table(){
    node *temp;
    for (int i = 0; i < size_of_memory; i++){
        temp = hash_table->items[i];
        free_hash_list(temp);
    }
    free(hash_table->items);
    free(hash_table);
}

/*
 * Function searches list for value, if found changes node->second to 1
 * @Return: True if found
 * @Return: FALSE if not found
 */
int search_list(node *list, long value){
    if (list == NULL){
        return FALSE;
    }
    while (list != NULL){
        if (list->value == value){
            list->second = 1;
            return TRUE;
        }
        list = list->next;
    }
    return FALSE;
}

/*
 * Searches hash table for value
 * @Return: TRUE if found
 * @Return: FALSE if not found
 */
int search_hash(long value){
    int index = hash(value);
    if (hash_table->items[index] != NULL){
        return search_list(hash_table->items[index], value);
    }
    return FALSE;
}

/*
 * Function creates and inserts a node into the hash table and increments table size
 * If value is already in the table the node is not created
 */
void insert_hash(long value){
    int index = hash(value);
    node *temp = hash_table->items[index];
    if (search_hash(value) == TRUE){
        return;
    }
    node *item = create_node(value);
    if (temp == NULL)
        hash_table->items[index] = item;
    
    else {
        while (temp->next != NULL){
            temp = temp->next;
        }
        temp->next = item;
    }
    hash_table->count++;
}

/*
 * Function adds node to end of queue
 */
node *add_node_end(node *item, node *list){
    if(list == NULL){
        tail = item;
        return item;
    }
    tail->next = item;
    tail = item;
    item->next = NULL;
    return list;
}

/*
 * Function removes a node from specified value in a list
 * @Param: item - node in a list to remove the value from
 * @Param: value - the value of node to be removed from the list
 * @Return: removed node if found
 * @Return: NULL if node not in list
 */
node *remove_node(node *item, long value){
    while (item->next != NULL){
        if (item->next->value == value){
            node *temp = item->next;
            if (temp->next != NULL)
                item->next = temp->next;
            
            else 
                item->next = NULL;
            
            return temp;
        }
        item = item->next;
    }
    return NULL;
}

/*
 * Function removes node from hash list
 */
void remove_hash(long value){
    int index = hash(value);
    node *temp = hash_table->items[index];
    if (temp == NULL)
        return;
    
    if (temp->value == value){
        hash_table->items[index] = temp->next;
        free_node(temp);
    }
    else{
        temp = remove_node(temp, value);
        if (temp != NULL)
            free_node(temp);
        
    }
    hash_table->count--;
}

/*
 * Function removes the node in the front of a list
 * @Param: item - the start of list to remove front of
 */
node *remove_front(node *item){
    node *temp = item->next;
    free_node(item);
    return temp;
}

/*
 * Function uses a queue and hash table to perform the FIFO page replacement scheme
 * The function uses the hash table to search for values in the queue and updates
 * the queue with the current pages
 * @Param: value - the value of the page that is being looked for
 */
void first_in(long value){
    if (!search_hash(value)){
        page_faults++;
        node *item = create_node(value);
        if (hash_table->count == size_of_memory){
            swap_outs++;
            remove_hash(head->value);
            head = remove_front(head);
        }
        swap_ins++;
        insert_hash(item->value);
        head = add_node_end(item, head);
    }
    
}

/*
 * Function uses a queue and hash table to impliment the second chance page
 * replacement scheme. The funciton uses the hash table to search for the 
 * pages currently in the queue
 * @Param: value - current value of page that is searched for
 */
void second_chance(long value){
    if (!search_hash(value)){
        page_faults++;
        node *item = create_node(value);
        if (hash_table->count == size_of_memory){
            swap_outs++;
            while(head->second == 1){
                head->second = 0;
                node *temp = create_node(head->value);
                add_node_end(temp, head);
                head = remove_front(head);
            }
            remove_hash(head->value);
            head = remove_front(head);
        }
        swap_ins++;
        insert_hash(item->value);
        head = add_node_end(item, head);
    }
    else {
        search_list(head, value);
    }
}

/*
 * Function rotates a node matching the corresponding value and
 * moves the node to the end of the list parameter
 * @Param: value - value of node to be rotated
 * @Param: list - head of list that is searched for value
 * @Return: returns head of list
 */
node *rotate_list(long value, node *list){
    if (list == NULL){
        return NULL;
    }
    node *temp, *current;
    if (list->value == value){
        temp = list;
        list = list->next;
        list = add_node_end(temp, list);
        return list;
    }
    temp = list;
    current = list->next;
    while (current != NULL){
        if (current->value == value){
            if (current->next == NULL){
                return list;
            }
            temp->next = current->next;
            temp = add_node_end(current, temp);
        }
        temp = current;
        current = current->next;
    }
    return list;
}

/*
 * Function uses a queue and hash table to impliment LRU page replacement
 * scheme. If the page is already in the list it is rotated to the end.
 * This way the front page of the queue is the page that was least recently used
 * @Param: value - the value of the page to be included
 */
void least_recent_used(long value){
    if(!search_hash(value)){
        page_faults++;
        if (hash_table->count == size_of_memory){
            swap_outs++;
            remove_hash(head->value);
            head = remove_front(head);
        }
        swap_ins++;
        node *temp = create_node(value);
        head = add_node_end(temp, head);
        insert_hash(value);
        return;
    }
    head = rotate_list(value, head);
}

int find_frame_num(long value){
    node *temp = head;
    int i = 0;
    while(temp != NULL){
        if (temp->value == value){
            return i;
        }
        i++;
        temp = temp->next;
    }
    return -1;
}

/*
 * Function to convert a logical address into its corresponding 
 * physical address. The value returned by this function is the
 * physical address (or -1 if no physical address can exist for
 * the logical address given the current page-allocation state.
 */

long resolve_address(long logical, int memwrite)
{
    int i;
    long page, frame;
    long offset;
    long mask = 0;
    long effective;

    /* Get the page and offset */
    page = (logical >> size_of_frame);

    for (i=0; i<size_of_frame; i++) {
        mask = mask << 1;
        mask |= 1;
    }
    offset = logical & mask;

    /* Find page in the inverted page table. */
    frame = -1;
    if (page_replacement_scheme == REPLACE_FIFO){
        first_in(page);
    }
    else if (page_replacement_scheme == REPLACE_LRU){
        least_recent_used(page);
    }
    else if (page_replacement_scheme == REPLACE_SECONDCHANCE){
        second_chance(page);
    }
    
    frame = find_frame_num(page);
    /* If frame is not -1, then we can successfully resolve the
     * address and return the result. */
    if (frame != -1) {
        effective = (frame << size_of_frame) | offset;
        return effective;
    }
    else {
        return -1;
    }
}

/*
 * Super-simple progress bar.
 */
void display_progress(int percent)
{
    int to_date = PROGRESS_BAR_WIDTH * percent / 100;
    static int last_to_date = 0;
    int i;

    if (last_to_date < to_date) {
        last_to_date = to_date;
    } else {
        return;
    }

    printf("Progress [");
    for (i=0; i<to_date; i++) {
        printf(".");
    }
    for (; i<PROGRESS_BAR_WIDTH; i++) {
        printf(" ");
    }
    printf("] %3d%%", percent);
    printf("\r");
    fflush(stdout);
}


int setup()
{
   create_hash_table();

    return -1;
}


int teardown()
{
    free_list();
    free_table();
    return -1;
}


void error_resolve_address(long a, int l)
{
    fprintf(stderr, "\n");
    fprintf(stderr, 
        "Simulator error: cannot resolve address 0x%lx at line %d\n",
        a, l
    );
    exit(1);
}


int output_report()
{
    printf("\n");
    printf("Memory references: %d\n", mem_refs);
    printf("Page faults: %d\n", page_faults);
    printf("Swap ins: %d\n", swap_ins);
    printf("Swap outs: %d\n", swap_outs);

    return -1;
}


int main(int argc, char **argv)
{
    /* For working with command-line arguments. */
    int i;
    char *s;

    /* For working with input file. */
    FILE *infile = NULL;
    char *infile_name = NULL;
    struct stat infile_stat;
    int  line_num = 0;
    int infile_size = 0;

    /* For processing each individual line in the input file. */
    char buffer[MAX_LINE_LEN];
    long addr;
    char addr_type;
    int  is_write;

    /* For making visible the work being done by the simulator. */
    int show_progress = FALSE;

    /* Process the command-line parameters. Note that the
     * REPLACE_OPTIMAL scheme is not required for A#3.
     */
    for (i=1; i < argc; i++) {
        if (strncmp(argv[i], "--replace=", 9) == 0) {
            s = strstr(argv[i], "=") + 1;
            if (strcmp(s, "fifo") == 0) {
                page_replacement_scheme = REPLACE_FIFO;
            } else if (strcmp(s, "lru") == 0) {
                page_replacement_scheme = REPLACE_LRU;
            } else if (strcmp(s, "secondchance") == 0) {
                page_replacement_scheme = REPLACE_SECONDCHANCE;
            } else if (strcmp(s, "optimal") == 0) {
                page_replacement_scheme = REPLACE_OPTIMAL;
            } else {
                page_replacement_scheme = REPLACE_NONE;
            }
        } else if (strncmp(argv[i], "--file=", 7) == 0) {
            infile_name = strstr(argv[i], "=") + 1;
        } else if (strncmp(argv[i], "--framesize=", 12) == 0) {
            s = strstr(argv[i], "=") + 1;
            size_of_frame = atoi(s);
        } else if (strncmp(argv[i], "--numframes=", 12) == 0) {
            s = strstr(argv[i], "=") + 1;
            size_of_memory = atoi(s);
        } else if (strcmp(argv[i], "--progress") == 0) {
            show_progress = TRUE;
        }
    }

    if (infile_name == NULL) {
        infile = stdin;
    } else if (stat(infile_name, &infile_stat) == 0) {
        infile_size = (int)(infile_stat.st_size);
        /* If this fails, infile will be null */
        infile = fopen(infile_name, "r");  
    }


    if (page_replacement_scheme == REPLACE_NONE ||
        size_of_frame <= 0 ||
        size_of_memory <= 0 ||
        infile == NULL)
    {
        fprintf(stderr, 
            "usage: %s --framesize=<m> --numframes=<n>", argv[0]);
        fprintf(stderr, 
            " --replace={fifo|lru|optimal} [--file=<filename>]\n");
        exit(1);
    }


    setup();

    while (fgets(buffer, MAX_LINE_LEN-1, infile)) {
        line_num++;
        if (strstr(buffer, ":")) {
            sscanf(buffer, "%c: %lx", &addr_type, &addr);
            if (addr_type == 'W') {
                is_write = TRUE;
            } else {
                is_write = FALSE;
            }

            if (resolve_address(addr, is_write) == -1) {
                error_resolve_address(addr, line_num);
            }
            mem_refs++;
        } 

        if (show_progress) {
            display_progress(ftell(infile) * 100 / infile_size);
        }
    }
    

    teardown();
    output_report();

    fclose(infile);

    exit(0);
}
