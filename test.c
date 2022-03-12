#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TOK_BUFSIZE 64

void split_line(char* line, char* args[]){
    //printf("Reading command: %s\n", line);
 
    //Array to store all the elements of the line
    int numtokens = 0;
    int pos = 0;
    char token[128];
    memset(args, 0, TOK_BUFSIZE*sizeof(char));

    /* "test command" */
    for(int i = 0; i < strlen(line); i++){

        if(line[i] == ' '){
            token[pos] = '\0';
            args[numtokens++] = strdup(token);
            memset(token, 0, sizeof(char)*128);
            pos = 0;
        } else if(line[i] == '\"'){
            i++; /*Move to next character inside quotes*/
            while(line[i] != '\"'){
                token[pos++] = line[i++];
            }
            token[pos] = '\0';
            args[numtokens++] = strdup(token);
            memset(token, 0, sizeof(char)*128);
            pos = 0;
        } else { /* Character is neither white space or quote*/
            token[pos++] = line[i];
        }
    }
    token[pos] = '\0';
    args[numtokens] = strdup(token); /* Add last command */
    args[numtokens+1] = (char*)NULL; /* args is terminated by a null char* pointer*/

}


int main(){

	char* line = "git commit -m \"yass mode\"";
	char* args[TOK_BUFSIZE];

	split_line(line, args);

    int i;
	for(i = 0; i < 4; i++){
		printf("args[%d]: %s\n", i, args[i]);
	}
    printf("args[%d]: %s\n", i, args[i]);



}