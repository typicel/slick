#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TOK_BUFSIZE 64

void split_line(char* line, char* args[]){
    printf("Reading command: %s\n", line);
 
    //Array to store all the elements of the line
    int numtokens = 0;
    int pos = 0;
    char* token = malloc(TOK_BUFSIZE * sizeof(char));
    // char token[64];


    for(int i = 0; i < strlen(line); i++){

        if(line[i] == ' '){
            printf("Adding %s\n", token);
           	args[numtokens++] = strdup(token);
            memset(token, 0, strlen(token));
            pos = 0;
        } else if(line[i] == '\"'){
            i++; /*Move to next character inside quotes*/
            while(line[i] != '\"'){
                token[pos] = line[i];
                pos++;
                i++;
            }
            printf("Adding quoted word: %s\n", token);
            args[numtokens++] = strdup(token);
            memset(token, 0, strlen(token));
            pos = 0;
        } else { /* Character is neither white space or quote*/
            token[pos] = line[i];
            pos++;
        }
    }

}


int main(){

	char* line = "git commit -m \"yass mode\"";
	char* args[4];

	split_line(line, args);


	for(int i= 0; i < 4; i++){
		printf("args[%d]: %s\n", i, args[i]);
	}



}