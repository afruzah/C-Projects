/* process.c - code used by small shell to execute piped commands */

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<signal.h>
#include	<sys/wait.h>
#include 	<fcntl.h>

int redirect(char *argv[], int pipes, int cmd[], char* redirection[])
/*
 * purpose: run a program passing it arguments
 * returns: status returned via wait, or -1 on error
 *  errors: -1 on fork() or wait() errors

 */

//char *redirection[]
{
	
	int	child_info = -1;
	int new_pipe[2];
	int prev_pipe[2];
	int i = 0;


	if ( argv[0] == NULL ) {
		/* nothing succeeds	*/
		return 0;
	}
	
	for (i = 0; i < pipes; i++) {
		if (i != pipes-1) {
			pipe(new_pipe);
		}

		int	pid = fork();
		
		if  ( pid < 0 ) {
			perror("fork");
			
		}
		else if ( pid > 0 ) {
			//if parent
			if(i != 0) {
				//if not first command
				close(prev_pipe[0]);
				//close previous pipe to standard input
			}
			if(i != pipes-1)
				//if not last command
			{
				prev_pipe[0] = new_pipe[0];
				//the previous stdin pipe is equal to the new stdin pipe
				prev_pipe[1] = new_pipe[1];
				//the previous stdout pipe is equal to the new stdout pipe
				close(prev_pipe[1]);
				//close the stdout pipe
			}
			
			wait(NULL);
			//waiting for child
			
		}
		else if ( pid == 0 )
		{ //if child
			
			if(i != 0)
				//if not first command
			{
				dup2(prev_pipe[0], 0);
				//receiving input to previous pipe
				close(prev_pipe[0]);
				//closing previous stdin pipe
			}
			
			if(i != pipes-1)
				//if not last command
			{
				dup2(new_pipe[1], 1);
				//receiving output to new pipe
				close(new_pipe[1]);
				//closing new stdout pipe
			}
			
			if((redirection[0] != NULL) && (i == 0)) {
				//checks if first element in redirection array exists
				int in = open(redirection[0], O_RDONLY, 0);
				//opens the first element in array as read only and is instantiated as variable in for input
				dup2(in, STDIN_FILENO);
				//duplicates in to stdin
			}
			
			if( (redirection[1] != NULL) && (i == pipes-1) ) {
				//checks if second element in redirection array exists
				int out = open(redirection[1], O_WRONLY |  O_CREAT, 0644);
				//opens second element in array as readable and writable by file owner and instantiated as variable out for output
				dup2(out, STDOUT_FILENO);
				//duplicates out to stdout
			}
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
		
			
			execvp(argv[cmd[i]], &argv[cmd[i]]);
			//loads arguments to be executed using argument list and int array that provides location of commands
			perror("cannot execute command");
			//if not executed will error
			exit(1);
		}
		else
		{
			if ( wait(&child_info) == -1 )
				perror("wait");
		}
	}
	
	return child_info;
}


