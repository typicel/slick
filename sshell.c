#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "builtins/builtin.c"
#include "handler.c"

#define TOK_BUFSIZE  64 /* size of each token */
#define SH_BUFSIZE 1024 /* size of input buffer */
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

char** split_line(char* line){

    int bufsize = TOK_BUFSIZE;

    //Array to store all the elements of the line
    char** tokens = malloc(TOK_BUFSIZE * sizeof(char*));
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
    char* line = NULL;
    char** args;
    size_t len = 0;
    ssize_t read;
    int status;
    char home[64];

    char* f_path = getenv("HOME");
    strcpy(home, f_path);
    strcat(home, "/.ssrc");


    fp = fopen(home, "r");
    if (fp == NULL){
        printf("not yass");
        exit(EXIT_FAILURE);
    }

    //read from .ssrc and run startup commands
    while ((read = getline(&line, &len, fp)) != -1) {
        line[read-1] = '\0';
        args = split_line(line);
        status = launch(args);
    }
    fclose(fp);

}


void sshell_loop(void){
    //Main shell loop
    char** args;
    int status;
    char* line = NULL;
    char* env;
    
    signalManager();

    do{
        line = read_input();
        add_history(line); //add command entered to history

        args = split_line(line);

        //Repalce environment variables
        for(int i = 0; i < TOK_BUFSIZE; i++){
            if(args[i] == NULL){break;}
            if(args[i][0] == '$'){
                env = getenv(args[i]+1);
                strcpy(args[i], env);
            }
        }

        status = execute(args);

    } while(status);
}


int main(){
    using_history();
    startup();
    sshell_loop();

    return 1;
}