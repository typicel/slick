#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "builtins/builtin.c"

#define TOK_BUFSIZE  64 /* size of each token */
#define SH_BUFSIZE 1024 /* size of input buffer */
#define PATH_MAX 64 /* Max size of path */
#define MAX_HIST_SIZE 20 /* history array holds previous 20 commands */
#define MAGENTA "\033[35m"      
#define RESET   "\033[0m"

char** history[MAX_HIST_SIZE];
int history_count = 0;

void type_prompt(){
    int path_max = PATH_MAX;
    char cwd[path_max];
    getcwd(cwd, sizeof(cwd));

    //Get the last directory to display to user
    char* folder;
    char* token;
    token = strtok(cwd, "/");
    while(token != NULL){
        folder = token;
        token = strtok(NULL, "/");
    }

    printf(MAGENTA "%s #> " RESET, folder);
}

char* read_input(void){

    size_t bufsize = SH_BUFSIZE;

    char *buffer = (char*) malloc(bufsize * sizeof(char));
    if(buffer == NULL){
        perror("Unable to allocate buffer");
        exit(1);
    }

    buffer = readline(" ");
    return buffer;
}

char** split_line(char* line){

    int bufsize = TOK_BUFSIZE;

    //Array to store our array of all the elements of the line
    char** tokens = malloc(bufsize * sizeof(char));
    int pos = 0;
    char* delim = " ";

    char* curToken = strtok(line, delim);
    

    while(curToken != NULL){
        tokens[pos] = curToken;
        pos++;
        curToken = strtok(NULL, delim);
    }
    tokens[pos] = NULL;

    return tokens;
}


int launch(char** args){
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == 0){
        if(execvp(args[0], args) == -1){
            perror("sshell exe:");
        }
        exit(EXIT_FAILURE);
    } else if(pid < 0){
        perror("sshell");
    } else { //successful fork

        do { //wait for the child process to finish execution of command
            wpid = waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));


    }
    return 1;
}

int execute(char** args){
    int i;

    if(args[0] == NULL){ //empty command
        return 1;
    }

    for(i = 0; i < num_bulitins(); i++){ //check if command is a builtin command
        if(strcmp(args[0], builtins[i]) == 0){
            return (*builtin_functions[i])(args); //array of function pointers
        }
    }

    return launch(args);
}

void add_to_history(char* command){
    /* 1. If the history size is less than the size of array, add to end
       2. If not, shift all elements down one, and add new command to freed spot */

    if(history_count < MAX_HIST_SIZE){
        history[history_count] = command;
        history_count++;
    } else {
        for(int i = 0; i < history_count; i++){
            char* temp = history[i+1];
        }
    }

}

void startup(){
    FILE* fp;
    char* line = NULL;
    char** args;
    size_t len = 0;
    ssize_t read;
    int status;

    char* filepath = getenv("HOME");
    strcat(filepath, "/.ssrc");


    fp = fopen(filepath, "r");
    //printf("Opened file %s", filepath);
    if (fp == NULL){
        printf("uh oh");
        exit(EXIT_FAILURE);
    }

    //read from .ssrc and run startup commands
    while ((read = getline(&line, &len, fp)) != -1) {
        line[read-1] = '\0';
        args = split_line(line);
        status = launch(args);
    }

}


void sshell_loop(void){
    //Main shell loop
    char** args;
    int status;
    char* line = NULL;

    do{
        type_prompt();
        line = read_input();
        add_to_history(line); /* add the current line to history */
        args = split_line(line);
        status = execute(args);

    } while(status);
}


int main(){
    
    startup();
    sshell_loop();

    return 1;
}