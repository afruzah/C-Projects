#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
	/* num_arg and num_command relate to the position of the commands and their related arguments in the array that is set by char * line */
	int num_arg = 0;
	int num_command = 0;
	
	char command[256]; /*command is used to the part of the array we will extract with strtok to be used as a command in the command array *line */
	char * line[100][256]; /*array that will be populated with commands and their arguments */
	
	char * token; /*token is the variable that is defined when the input file is read line by line and separated by spaces and new lines, token will be inserted into the line array and put arguments and commands in their relative position*/
	
	while (fgets(command, 256, stdin)!=NULL) {
		token = strtok(command," \n");/*reading file line by line and separating input, separated string is stored into variable token*/
		while (token!=NULL) { /* reading until end of line */
			line[num_command][num_arg] = (char *)(malloc(sizeof(char)*100)); /*allocating memory to line array so that variables can be stored */
			strcpy(line[num_command][num_arg], token); /* copying in variable token to line array in given space*/
			token = strtok(NULL, " \n"); /*continues to retreive tokens until there are no more tokens to recieve and hence will return NULL*/
			num_arg++; /* incrementing the number of arguments */
		}
		line[num_command][num_arg]=NULL; /*adding NULL pointer to end of array*/
		num_arg=0;/*resetting position of argument so that next line (command) starts at beginning of arguments*/
		num_command++; /* increment number of commands */
	}
	
	
	
	for (int i = 0; i < num_command; i++) { /* repeating processes below for each command */
		int pid = fork(); /* initialising process id and creating new process (identical)*/
		if (pid < 0) { /* if fork is less than 0 then forking has failed */
			return -1;
		}
		
		if (pid == 0) { /* identified child process*/
			execvp(line[i][0],line[i]); /*execute command at this position in array*/
			return 0;
		}
		else if (pid > 0) { /* identified parent process */
			wait(NULL); /* wait for child process */
		}
		
	}
	
	
	
	
	return 0;
}

