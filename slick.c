#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "builtins/builtin.c"
#include "handler.c"

#define TOK_BUFSIZE  128 /* size of each token */
#define SH_BUFSIZE 1024 /* size of input buffer */
#define PATH_MAX 256
#define MAGENTA "\033[35m"      
#define RESET   "\033[0m"


void generate_prompt(char* prompt, int len){
    int path_max = PATH_MAX;
    char cwd[path_max];
    getcwd(cwd, sizeof(cwd));
    int status;

    //Get the last directory to display to user
    char* folder;
    char* token;
    token = strtok(cwd, "/");
    while(token != NULL){
        folder = token;
        token = strtok(NULL, "/");
    }
    
    //save the printf output into prompt
    snprintf(prompt, len, MAGENTA "%s #> " RESET, folder);
}

char* read_input(void){

    size_t bufsize = SH_BUFSIZE;
    char prompt[128];

    char *buffer = (char*) malloc(bufsize * sizeof(char));
    if(buffer == NULL){
        perror("Unable to allocate buffer");
        exit(1);
    }

    generate_prompt(prompt, 128); //store the prompt into prompt variable to pass into readline
    buffer = readline(prompt);
    return buffer;
}


void split_line(char* line, char* args[]){
    //printf("Reading command: %s\n", line);
 
    //Array to store all the elements of the line
    int numtokens = 0;
    int pos = 0;
    char token[128];
    memset(args, 0, TOK_BUFSIZE*sizeof(char));

    /* "test command" */
    for(int i = 0; i < strlen(line); i++){

        if(line[i] == ' '){
            token[pos] = '\0';
            args[numtokens++] = strdup(token);
            memset(token, 0, sizeof(char)*128);
            pos = 0;
        } else if(line[i] == '\"'){
            i++; /*Move to next character inside quotes*/
            while(line[i] != '\"'){
                token[pos++] = line[i++];
            }
            token[pos] = '\0';
            args[numtokens++] = strdup(token);
            memset(token, 0, sizeof(char)*128);
            pos = 0;
        } else { /* Character is neither white space or quote*/
            token[pos++] = line[i];
        }
    }
    if(strlen(token) > 0){ /* if the final token has content */
        token[pos] = '\0';
        args[numtokens] = strdup(token); /* Add last command */
    }
    args[numtokens+1] = (char*)NULL; /* args is terminated by a null char* pointer*/

}


int launch(char** args){
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == 0){
        if(execvp(args[0], args) == -1){
            perror("slick");
        }
        exit(EXIT_FAILURE);
    } else if(pid < 0){
        perror("slick");
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

    return launch(args); //if not builtin, run execvp on the command
}

void startup(){
    FILE* fp;
    char* line, *env, *args[TOK_BUFSIZE];
    size_t len = 0;
    ssize_t read;
    int status;
    char home[64];

    char* f_path = getenv("HOME");
    strcpy(home, f_path);
    strcat(home, "/.ssrc");


    fp = fopen(home, "r");
    if (fp == NULL){
        printf("slick: could not find /usr/bin/.ssrc");
        exit(EXIT_FAILURE);
    }

    //read from .ssrc and run startup commands
    while ((read = getline(&line, &len, fp)) != -1) {
        line[read-1] = '\0';
        split_line(line, args);
        
        
        status = launch(args);
    }
    fclose(fp);

}


void sshell_loop(void){
    //Main shell loop
    char* args[TOK_BUFSIZE];
    int status;
    char* line;
    char* env;
    char* fullpath;
    
    signalManager();

    do{
        line = read_input();
        add_history(line); //add command entered to history

        split_line(line, args);

        //Repalce environment variables
        for(int i = 0; i < TOK_BUFSIZE; i++){
            if(args[i] == NULL){break;}
            if(args[i][0] == '$'){
                env = getenv(args[i]+1);
                strcpy(args[i], env);
            }
            // if(args[i][0] == '~'){
            //     env = getenv("HOME");
            //     strcpy(fullpath, env);
            //     strcat(fullpath, args[i]);
            //     args[i] = fullpath;
            // }
        }

        status = execute(args);

    } while(status);
}


int main(){

    // char* line = "Hello \"Hello World\"";
    // char** args = malloc(TOK_BUFSIZE*sizeof(char*));
    // split_line(line, args);

    using_history();
    startup();
    sshell_loop();

    return 1;
}