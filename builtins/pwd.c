/* Implementation of the pwd command */

int sshell_pwd(char** args){
    int path_max = 256;
    char cwd[path_max];
    getcwd(cwd, sizeof(cwd));

    printf("%s\n", cwd);

    return 1;
}