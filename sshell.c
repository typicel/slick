#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtin.c"

#define TOK_BUFSIZE  64
#define SH_BUFSIZE 1024
#define PATH_MAX 64

void type_prompt(){
    static int first_time = 1;
    if(first_time){
        const char* CLEAR_SCREEN_ANSI = " \e[1;1H\e[2J";
        write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
        first_time = 0;
        printf("Welcome to slick (sshell)\n");
        printf("Enter commands seperated by '~'\n");
    }

    int path_max = PATH_MAX;
    char cwd[path_max];
    getcwd(cwd, sizeof(cwd));

    printf("%s #> ", cwd);
}

char* read_input(void){

    size_t bufsize = SH_BUFSIZE;

    char *buffer = (char*) malloc(bufsize * sizeof(char));
    if(buffer == NULL){
        perror("Unable to allocate buffer");
        exit(1);
    }

    getline(&buffer, &bufsize, stdin);

    return buffer;
}

char** split_line(char* line){

    int bufsize = TOK_BUFSIZE;

    //Array to store our array of all the elements of the line
    char** tokens = malloc(bufsize * sizeof(char));
    int pos = 0;

    char* curToken = strtok(line, "~");

    while(curToken != NULL){
        tokens[pos] = curToken;
        pos++;
        curToken = strtok(NULL, "~");
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
            perror("sshell");
        }
        exit(EXIT_FAILURE);
    } else if(pid < 0){
        perror("sshell");
    } else { //successful fork

        do { //wait for the child process to finish execution of command
            wpid = waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));

        return 1;

    }
}

int execute(char** args){
    int i;

    if(args[0] == NULL){ //empty command
        return 1;
    }

    for(i = 0; i < num_bulitins(); i++){
        if(strcmp(args[0], builtins[i]) == 0){
            return (*builtin_functions[i])(args);
        }
    }


    return launch(args);
}

int main(){
    
    //Main shell loop
    char* line;
    char** args;
    int status;

    do{
        type_prompt();
        line = read_input();
        args = split_line(line);
        status = execute(args);

    } while(status);
}