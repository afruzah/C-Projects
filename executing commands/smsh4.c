#include	<stdio.h>
#include	<stdlib.h>
#include  	<string.h>
#include	<unistd.h>
#include	<signal.h>
#include 	<glob.h>
#include 	<stdbool.h>
#include	"smsh.h"


#define	DFL_PROMPT	"> "

bool checkForGlob (char* arglist) {
	//function to check if globbing exists in argument string
	int i;
	for (i = 0; i < strlen(arglist); i++) {
		//iterating through all characters of string
		if (arglist[i] == '*') {
			//if one of the characters is '*' function returns true
			return true;
		}
	}
	
	return false;
	//if no globbing found function returns false
}

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
			bool globExist = false;
			//existence of glob is set to false
			glob_t toGlob;
			//creating glob structure
			
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
				} else if (checkForGlob(arglist[i])==true) {
					//checks if glob exists using function
					globExist = true;
					//sets the existence of glob to true
					toGlob.gl_offs = i;
					//slots to reserve in matched path names is the number of iterations
					glob(arglist[i],GLOB_DOOFFS, NULL, &toGlob);
					//searching for pathnames with matching patterns and storing in struct
					int j;
					for (j = 0; j < i; j++) {
						toGlob.gl_pathv[j] = arglist[j];
						//matched pathnames stored in struct
					}
				}
				
			}
			
			if (globExist == true) {
				//if there is globbing
				result = redirect(toGlob.gl_pathv, pipes+1, cmd, redirection);
				//redirect function is called with the array that has all matching path names stored as well as pipes and redirection array
				freelist(toGlob.gl_pathv);
				//frees array with globbed paths
			} else {
				result = redirect(arglist, pipes+1, cmd, redirection);
				//if no globbing redirect function is called with argument array and number of pipes and redirection
				freelist(arglist);
				//frees argument array
			}
			
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


	




