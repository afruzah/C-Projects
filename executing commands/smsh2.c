#include	<stdio.h>
#include	<stdlib.h>
#include  	<string.h>
#include	<unistd.h>
#include	<signal.h>
#include	"smsh.h"

#define	DFL_PROMPT	"> "

int main()
{
	char	*cmdline, *prompt, **arglist;
	//intialising command line and argument variables
	int	result;
	void	setup();

	prompt = DFL_PROMPT ;
	setup();
	
	
	
	while ( (cmdline = next_cmd(prompt, stdin)) != NULL ){
		// while the next command in the command line is not null
		int num = 0;
		//initialising integer to store number of arguments
		int i = 0;
		for (i = 0; i < 255; i++) {
			//for loop checks each command in command line to count
			if (cmdline[i] == ' ') {
				num++;
				//number of arguments increases when a space is identified
			}
		}
		if ( (arglist = splitline(cmdline)) != NULL  ){
			// while the array of arguments is not null
			int pipes = 0;
			//initialise number of pipes to 0
			int cmd[num];
			//initialise integer array that allocates where arguments will go
			cmd[0] = 0;
			// the first argument location will always be at 0
			i = 0;
			
			for (i = 0; i < num + 1; i++) {
				//for loop checks through arguments to see if there is a pipe
				if (*arglist[i]== '|') {
					//if pipe is found number of pipes increases through loop
					pipes++;
					cmd[pipes] = i+1;
					// location of argument to be piped is set one space after where pipe was originally; the argument would be after the pipe
					arglist[i]=NULL;
					//sets pipe in arguments array to null
				}
			}
			if (pipes > 0) {
				result = process(arglist, pipes+1, cmd);
				//executes function that handles pipes if pipe exists
				freelist(arglist);
				//frees memory from argument array
			} else {
				execute(arglist);
				//executes original function from "smsh1.c" if no pipe exists
				freelist(arglist);
				//frees memory from argument array
			}
		}
		free(cmdline);
		//frees memory from command line
	}
	return 0;
}

void setup()
/*
 * purpose: initialize shell
 * returns: nothing. calls fatal() if trouble
 */
{
	signal(SIGINT,  SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
{
	fprintf(stderr,"Error: %s,%s\n", s1, s2);
	exit(n);
}
	


