/* Implementation for ls command */
#include <dirent.h>
#include <stdio.h>
#include <errno.h>

int sshell_ls(char** args){
    /* get the current working directory */
    int path_max = 64;
    char cwd[path_max];
    getcwd(cwd, sizeof(cwd));

    //open the current directory
    struct dirent *d;
    DIR *dh = opendir(cwd);

    //If we couldn't open directory
    if(!dh){
        if(errno == ENOENT){
            perror("sshell: Directory not found");
        } else {
            perror("sshell: Unable to read directory");
        }
        exit(1);
    }

    //Read all contents of current directory, ignoring hidden files
    while((d = readdir(dh)) != NULL){
        if(d->d_name[0] != '.'){
            printf("%s    ",d->d_name);
        }
    } 
    printf("\n");


    return 1;
}
