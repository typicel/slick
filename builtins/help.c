/* Implementation for the help command */

#define GREEN   "\033[32m"
#define RESET   "\033[0m"

//list of builtins to print out
char* builtins[] = {"exit", "cd", "help", "pwd", "ls"};

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