/* process.c - code used by small shell to execute piped commands */

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<signal.h>
#include	<sys/wait.h>

int process(char *argv[], int pipes, int cmd[])
// function takes in same parameters as execute.c as well as the command location array
/*
 * purpose: run a program passing it arguments
 * returns: status returned via wait, or -1 on error
 *  errors: -1 on fork() or wait() errors

 */
{
	
	int	child_info = -1;
	int new_pipe[2];//initialise new pipe
	int prev_pipe[2];//initialise previous pipe
	int i = 0;

	if ( argv[0] == NULL ) {
		/* nothing succeeds	*/
		return 0;
	}
	
	for (i = 0; i < pipes; i++) {
		//iterating through the pipes
		if (i != pipes-1) {
			pipe(new_pipe);
			//creates new_pipe for number of pipes less 1
		}

		int	pid = fork();
		//forking
		
		if ( pid  < 0 ) {
			perror("fork");
			//forking failed
		}
		
		if ( pid > 0) {
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
			
		else if ( pid == 0 ){
			//if process is child
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
			
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			
			execvp(argv[cmd[i]], &argv[cmd[i]]);
			//loads arguments to be executed using argument list and int array that provides location of commands
			perror("cannot execute command");
			//if not executed will error
			exit(1);
		}
		else {
			if ( wait(&child_info) == -1 )
				perror("wait");
		}
	}
	return child_info;
}

