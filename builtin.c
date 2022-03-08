/*
    Contains all builtin functions to slick that don't need to be forked
*/

#include <stdio.h>
#define PATH_MAX 64;

int sshell_exit(char** args);
int sshell_cd(char** args);
int sshell_help(char** args);
int sshell_pwd(char** args);

char* builtins[] = {"exit", "cd", "help", "pwd"};

int num_bulitins(){
    return sizeof(builtins) / sizeof(char*);
}

int (*builtin_functions[])(char**) = {
    &sshell_exit,
    &sshell_cd,
    &sshell_help,
    &sshell_pwd
};

//Bulletin command to exit shell
int sshell_exit(char** args){
    printf("Exiting...\n");
    return 0;
}

int sshell_cd(char** args){
    printf("dir: %s\n", args[1]);
    if(args[1] == NULL){
        fprintf(stderr, "sshell: expected argument to command \"cd\"\n");
    } else {
        if(chdir(args[1]) != 0){
            perror("sshell");
        }
    }
    return 1;
}

int sshell_help(char** args){
    int i;

    printf("Welcome to Slick\n");
    printf("HOW TO USE\n");
    printf("Commands are seperated from arguments using the '~' character (ex. 'echo~hello world!')\n");
    printf("List of builtin commands:");

    for(i = 0; i < num_bulitins(); i++){
        printf("%s()", builtins[i]);
    }
}

int sshell_pwd(char** args){
    int path_max = PATH_MAX;
    char cwd[path_max];
    getcwd(cwd, sizeof(cwd));

    printf(cwd);
}
