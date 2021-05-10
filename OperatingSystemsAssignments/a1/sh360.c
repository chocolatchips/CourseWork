/*
*
*
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_ARGS 7
#define MAX_LINE_LENGTH 80
#define MAX_PROMPTS 10
#define MAX_NUM_DIR 10

char line[MAX_LINE_LENGTH];
char *arg_input[MAX_PROMPTS], *envp[] = {NULL};
char *line_prompt;

char *sys_call_loc(char *);
void execute();
int pipe_location();
void write_to_file();
void arg_setup();
void join_2(int);
void join_3(int, int);

/**
 * Function creates a mock shell that repeatedly prompts the user for commands and executes input
 * Any commands that are piped together must have the line start with 'PP'
 * Any commands that redirect output to file must begin line with 'OR'
 **/
int main(int argc, char *argv[]){
    char usr_input[MAX_LINE_LENGTH];
    int line_len;
    char prompt[MAX_LINE_LENGTH];
    FILE *source = fopen(".sh360rc", "r");
    fgets(prompt, MAX_LINE_LENGTH, source);
    if (prompt[strlen(prompt) - 1] == '\n') {
        prompt[strlen(prompt) - 1] = '\0';
    }
    
    for (;;){
        arg_setup();
        fprintf(stdout, "%s", prompt);
        fflush(stdout);
        fgets(usr_input, MAX_LINE_LENGTH, stdin);
        
        if (usr_input[strlen(usr_input) - 1] == '\n') {
            usr_input[strlen(usr_input) - 1] = '\0';
        }
        if (strcmp(usr_input, "exit") == 0) {
            exit(0);
        }
        char *arg = NULL;
        int i = 0;
        arg = strtok(usr_input, " ");
        while (arg != NULL) {
            arg_input[i] = arg;
            i++;
            arg = strtok(NULL, " ");
        }
        if (strcmp("PP", arg_input[0]) == 0){
            int out[2];
            out[0] = pipe_location(1);
            out[1] = pipe_location(out[0]+1);
            if (out[0] == -1 || out[1] == -1){
                fprintf(stderr, "incorrect placement of '->'\n");
            }
            else if (out[0] == 1){
                fprintf(stderr, "missing first command\n");
            }
            else if (out[1] > out[0]){
                join_3(out[0], out[1]);
            }
            else{
                join_2(out[0]);
            }

        }
        else if (strcmp("OR", arg_input[0]) == 0){
            write_to_file();
        }
        else{
            execute();
        }
        
    }

    return 0;
}


/**
 * Function used to connect two commands together using pipes, displaying the output of second
 * command on std out
 * 
 * @param:  arrow:  location of arrow in command line input
 **/
void join_2(int arrow){
    int status, pid_1, pid_2;
    int ppe[2];
    char *pid_head[2], *pid_tail[2];
    pipe(ppe);
    
    if ((arg_input[1] = sys_call_loc(arg_input[1])) != NULL){
        if ((pid_1 = fork()) == 0) {
        
            char *task[MAX_ARGS-arrow];
            for(int i = 0; i < arrow-1; i++){
                task[i] = arg_input[i+1];
            }
            task[arrow-1] = NULL;
            dup2(ppe[1], 1);
            for (int i = 0; i < 2; i++){
                close(ppe[i]);
            }
            
            execve(task[0], task, envp);
            printf("child (head): SHOULDN'T BE HERE.\n");
        }
        
    }
    
    if ((arg_input[arrow+1] = sys_call_loc(arg_input[arrow+1])) != NULL){
        if((pid_2 = fork()) == 0) {
            char *task[MAX_ARGS-arrow];
            int i = 1;
            while (arg_input[arrow+i] != NULL) {
                task[i-1] = arg_input[arrow+i];
                i++;
            }
            task[i-1] = NULL;
            
            dup2(ppe[0], 0);
            for (int i = 0; i < 2; i++){
                close(ppe[i]);
            }
            
            execve(task[0], task, envp);
            printf("child (tail): SHOULDN'T BE HERE.\n");
        }
    }
    
    for (int i = 0; i < 2; i++) {
        close(ppe[i]);
    }
    
    waitpid(pid_1, &status, 0);
    waitpid(pid_2, &status, 0); 
}

/**
 * Function joins together three commands through pipes outputting the result of third command to stdout
 * 
 * Reference for pipe setup: https://www.unix.com/programming/155190-pipes-connecting-3-processes-circle.html
 * 
 * @param:  first:  location of first arrow in command line
 * @param:  second: location of second arrow in command line
 **/
void join_3(int first, int second){
    int status, pid_1, pid_2, pid_3;
    int pipe_1[2], pipe_2[2];
    char *pid_head[2], *pid_mid[2], *pid_tail[2];
    pipe(pipe_1);
    
    if ((arg_input[1] = sys_call_loc(arg_input[1])) != NULL){
        if ((pid_1 = fork()) == 0) {
        
            char *task[MAX_ARGS-first];
            for(int i = 0; i < first-1; i++){
                task[i] = arg_input[i+1];
            }
            task[first-1] = NULL;
            dup2(pipe_1[1], 1);
            for (int i = 0; i < 2; i++){
                close(pipe_1[i]);
            }
            
            execve(task[0], task, envp);
            fprintf(stderr, "head screwed up");
        }
        
    }
    
    pipe(pipe_2);
    if ((arg_input[first+1] = sys_call_loc(arg_input[first+1])) != NULL){
        if((pid_2 = fork()) == 0) {
            char *task[MAX_ARGS-(second-first)];
            
            for(int i = 0; i < first-1; i++){
                task[i] = arg_input[i+1];
            }
            task[first-1] = NULL;
            dup2(pipe_1[0], 0);
            dup2(pipe_2[1], 1);
            for (int i = 0; i < 2; i++){
                close(pipe_1[i]);
                close(pipe_2[i]);
            }
            
            execve(task[0], task, envp);
            fprintf(stderr, "mid screwed up");
        }
    }
    
    if ((arg_input[second+1] = sys_call_loc(arg_input[second+1])) != NULL){
        if((pid_3 = fork()) == 0) {

            char *task[MAX_ARGS-second];
            int i = 1;
            while (arg_input[second+i] != NULL) {
                task[i-1] = arg_input[second+i];
                i++;
            }
            task[i-1] = NULL;
            dup2(pipe_2[0], 0);
            for (int i = 0; i < 2; i++) {
                close(pipe_1[i]);
                close(pipe_2[i]);
            }
            execve(task[0], task, envp);
            fprintf(stderr, "tail screwed up");
        }
    }

    for (int i = 0; i < 2; i++) {
        close(pipe_1[i]);
        close(pipe_2[i]);
    }

    waitpid(pid_1, &status, 0);
    waitpid(pid_2, &status, 0); 
    waitpid(pid_3, &status, 0);     

}

/**
 * Function returns location of first '->' (arrow) encountered in the command line entered by user.
 * 
 * @param:  int start:  location in arg_input that search begins at
 * @return:             function returns the location of arrow in input line. If no arrow found returns 0,
 *                      else if arrow is last input returns -1.
 **/
int pipe_location(int start){
    int len = sizeof(arg_input)/sizeof(arg_input[0]);
    int i;
    for (i = start; i < len; i++){
        if (arg_input[i] == NULL){
            break;
        }
        else if (strcmp("->", arg_input[i]) == 0){
            if (i + 1 < len && arg_input[i +1] != NULL)
                return i;
            else {
                return -1;
            }
        }
    }
    
    return 0;
}

/** 
 * Function directs output from command following "OR" into file specified after "->"
 * The output file is created if it does not exist or overwriten if already present
 * Sections of this function was based on appendix_c.c reference code
 * 
 * @return: outputs results of command entered into specified file
 **/
void write_to_file(){
    int status, pid, fd;
    int out = pipe_location(2);

    fprintf(stdout, "%s\n", arg_input[out+1]);
    if (out == 0)
        fprintf(stderr, "missing \"->\" symbol\n");
    else if (out == -1)
        fprintf(stderr, "missing filename");
    
    if ((arg_input[1] = sys_call_loc(arg_input[1])) != NULL) {
        int pid = fork();
        if (pid == -1){
            fprintf(stderr, "fork failed");
            return;
        }
        else if (pid  == 0) {
            
            char *task[MAX_PROMPTS];
            for(int i = 0; i < out-1; i++){
                task[i] = arg_input[i+1];
            }
            
            fd = open(arg_input[out+1], O_CREAT|O_RDWR|O_TRUNC, S_IRUSR|S_IWUSR);
            
            if (fd == -1){
                fprintf(stderr, "error opening %s for writing\n", arg_input[out+1]);
                exit(1);
            }
            dup2(fd, 1);
            execve(task[0], task, envp);
            fprintf(stderr, "child went wrong\n");
        }

        waitpid(pid, &status, 0);
    }
    else {
        fprintf(stderr, "%s: command not found\n", arg_input[1]);
    }
}

/**
 * Function performs command entered without any pipes or redirects
 * 
 * @return: provides output of command entered, or ouputs error message to stderr
 **/
void execute(){
    int status, pid;
    
    if ((arg_input[0] = sys_call_loc(arg_input[0])) != NULL) {
        
        pid = fork();
        if (pid == -1){
            fprintf(stderr, "fork failed\n");
            return;
        }
        else if (pid  == 0) {
            execve(arg_input[0], arg_input, envp);
            fprintf(stderr, "child went wrong\n");
        }
        waitpid(pid, &status, 0);
    }
    else {
        if (arg_input[0] == NULL){
            fprintf(stderr, "no command entered\n");
        }
        else
            fprintf(stderr, "%s: command not found\n", arg_input[0]);
    }
}

/**
 * Funciton finds the path to the command in the .sh360rc file that matches the passed in keyword
 * 
 * @param:  arg: keyword used to find path of command. Example: arg = ls finds /bin/ls
 * @return: returns the path for keyword command if found, NULL is not found in file
 **/
char *sys_call_loc(char *arg){
    FILE *source = fopen(".sh360rc", "r");
    while(!feof(source)){
        fgets(line, MAX_LINE_LENGTH, source);
        if (feof(source)){
            break;
        }
        if (strstr(line, arg) != NULL){
            fclose(source);
            if (line[strlen(line) -1] == '\n')
                line[strlen(line)-1] = '\0';
            return line;
        }
    }
    fclose(source); 
    return NULL;
}

/*
 *Function resets all values in arg_input array to NULL 
 **/
void arg_setup(){
    for(int i = 0; i < MAX_PROMPTS; i++){
        arg_input[i] = NULL;
    }
}