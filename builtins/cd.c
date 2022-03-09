/* Implementation of the cd command  */
#include <stdio.h>

int sshell_cd(char** args){
    //printf("dir: %s\n", args[1]);

    if(args[1] == NULL){
        fprintf(stderr, "sshell: expected argument to command \"cd\"\n");
    } else {
        if(chdir(args[1]) != 0){
            perror("sshell");
        }
    }
    return 1;
}