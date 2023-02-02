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
		token = strtok(command," \n"); /*reading file line by line and separating input, separated string is stored into variable token*/
		while (token!=NULL) { /* reading until end of line */
			line[num_command][num_arg] = (char *)(malloc(sizeof(char)*100)); /*allocating memory to line array so that variables can be stored */
			strcpy(line[num_command][num_arg], token); /* copying in variable token to line array in given space*/
			token = strtok(NULL, " \n"); /*continues to retreive tokens until there are no more tokens to recieve and hence will return NULL*/
			num_arg++; /* incrementing the number of arguments */
		}
		line[num_command][num_arg]=NULL; /*adding NULL pointer to end of array*/
		num_arg=0; /*resetting position of argument so that next line (command) starts at beginning of arguments*/
		num_command++; /* increment number of commands */
	}
	
	int new_pipe[2]; /*initialise new pipe*/
	int prev_pipe[2]; /* initalise previous pipe */

	
	
	for (int i = 0; i < num_command; i++) { /* repeating the process for each command */
		if (i != num_command-1) {
			pipe(new_pipe); /*creating new pipes an amount of times that is 1 less than the number of commands*/
		}
		
		
		int pid = fork(); /* initialising process id, fork creates new process (but it's an identical process)*/
		
		if (pid < 0) { /*fork fails if less than 0*/
			return -1;
		}
		if (pid > 0) { /* if fork > 0 parent process identified*/
			if (i != 0) {
				close(prev_pipe[0]); /*closing stdin pipe*/
				close(prev_pipe[1]); /*closing stdout pipe*/
				
			}
			if ( i != num_command-1) {
				prev_pipe[1] = new_pipe[1]; /*assigning new_pipe data to prev_pipe*/
				prev_pipe[0] = new_pipe[0];
			}
			wait(NULL); /* wait for child process */
			
		}
		if (pid == 0) { /* child process identified*/
			if (i != 0) { /* when not first command */
				close(prev_pipe[1]); /* closing prev stdout pipe */
				dup2(prev_pipe[0], STDIN_FILENO); /*stdin is now prev_pipe*/
				close(prev_pipe[0]); /* close prev stdin pipe */
			}
			if ( i != num_command-1) { /* when not last command */
				close(new_pipe[0]); /* close new stdin pipe */
				dup2(new_pipe[1], STDOUT_FILENO); /* stdout pipe is now new_pipe */
				close(new_pipe[1]); /* close new stdout pipe */
			}
			execvp(line[i][0], line[i]); /* execute command from array that is populated with commands and arguments*/
					
		}
		
	}
	return 0;
}
