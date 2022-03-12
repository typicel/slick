/*
    Driver file for running built in commands 
*/
#define GREEN   "\033[32m"
#define RESET   "\033[0m"

#include <stdio.h>
#include "pwd.c"
#include "cd.c"

char* builtins[] = {"exit", "cd", "info", "pwd"};

int num_bulitins(){
    return sizeof(builtins) / sizeof(char*);
}

//Bulletin command to exit shell
int sshell_exit(char** args){
    printf("Exiting...\n");
    return 0;
}

int sshell_info(char** args){
    int i;

    printf("== SSHELL: SLICK == \n");
    printf("Slick is a UNIX shell built by Tyler McCormick (typicel)\n");
    printf("It has most of the functionality of a normal shell but it's still missing a few things, sorry!\n");
    printf("Created in 2022\n");

    // for(i = 0; i < num_bulitins(); i++){
    //     printf(GREEN "%s     ", builtins[i]);
    // }
    // printf(RESET "\n");

    return 1;
}



int (*builtin_functions[])(char**) = {
    &sshell_exit,
    &sshell_cd,
    &sshell_info,
    &sshell_pwd
};


