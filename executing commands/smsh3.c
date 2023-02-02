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
	int	result;
	void	setup();

	prompt = DFL_PROMPT ;
	setup();
	int in = 0; //in (stdin) set as 0 for ease of use
	int out = 1; //out (stdout) set to 1
	
	
	
	while ( (cmdline = next_cmd(prompt, stdin)) != NULL ){
		
		int num = 0;
		//initialising number of arguments
		int i = 0;
		for (i = 0; i < 255; i++) {
			if (cmdline[i] == ' ') {
				num++;
				//number of arguments increases when a space is identified
			}
		}
		if ( (arglist = splitline(cmdline)) != NULL  ){
			int pipes = 0;
			//number of pipes initialised to 0
			int cmd[num];
			//array that will hold command locations is initialised to be the size of the number of commands
			cmd[0] = 0;
			//the first command will be at the first location
			char * redirection[2] = {NULL, NULL};
			//initialising array to store arguments that would be redirected
			//all elements set to NULL by default in case no redirection
			
			
			for (i = 0; i < num + 1; i++)
			{
				if (*arglist[i]=='<') {
					//checks if redirection exists to STDIN
					redirection[in] = arglist[i+1];
					//argument after the redirection is stored in the array as the first element, labelled as variable 'in'
					arglist[i] = NULL;
					//sets argument to NULL in argument array
					
				} else if (*arglist[i]=='>'){
					//checks if redirection exists to STDOUT
					redirection[out] = arglist[i+1];
					//argument after redirection is stored in the array as the second element, labelled as variable 'out'
					arglist[i] = NULL;
					//setting argument to NULL in argument array
					
				} else if (*arglist[i]== '|') {
					//checks if argument is a pipe
					pipes++;
					//number of pipes increase
					cmd[pipes] = i+1;
					//location of argument to be piped is set to the number of the next iteration
					arglist[i]=NULL;
					//sets argument in argument array to NULL
				}
			}
			result = redirect(arglist, pipes+1, cmd, redirection);
			//executes redirect function with argument array, number of pipes, locations array and redirection array as parameters running input that includes pipes and redirection
			freelist(arglist);
			//frees memory from argument array
		}
		free(cmdline);
		//frees command line
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
	



