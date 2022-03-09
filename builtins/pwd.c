/* Implementation of the pwd command */

int sshell_pwd(char** args){
    int path_max = 64;
    char* cwd = malloc(path_max * sizeof(char));
    getcwd(cwd, sizeof(cwd));

    printf("%s", cwd);

    free(cwd);
    return 1;
}