/*
    Driver file for running built in commands 
*/
#define GREEN   "\033[32m"
#define RESET   "\033[0m"

#include <stdio.h>
#include "ls.c"
#include "pwd.c"
#include "cd.c"

char* builtins[] = {"exit", "cd", "help", "pwd", "ls"};

int num_bulitins(){
    return sizeof(builtins) / sizeof(char*);
}

//Bulletin command to exit shell
int sshell_exit(char** args){
    printf("Exiting...\n");
    return 0;
}

int sshell_help(char** args){
    int i;

    printf("\n-- HOW TO USE --\n");
    printf("Enter the command you want to use and press enter\n");
    printf("List of built in commands:\n");

    for(i = 0; i < num_bulitins(); i++){
        printf(GREEN "%s     ", builtins[i]);
    }
    printf(RESET "\n");

    return 1;
}



int (*builtin_functions[])(char**) = {
    &sshell_exit,
    &sshell_cd,
    &sshell_help,
    &sshell_pwd,
    &sshell_ls
};


