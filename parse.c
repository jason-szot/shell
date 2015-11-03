#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#define TRUE 1
#define FALSE 0
#define MAX_ARGS 10
#define MAX_PIPES 10
#define BUFSIZE 1024
#define TOK_BUFSIZE 64
#define MAX_SIZE 100
#define TOK_DELIM " \t\r\n\a"

void Parse(char *line0){
	char* null_ptr = 0;
	char*** cmd = malloc(MAX_SIZE * sizeof(char**));
	/*
	char arg[] = argument
	char* argv[] = argument array
	char** cmd[] = array of argument arrays
	*/
	int bufsize = MAX_SIZE, cmdp = 0, argp = 0, com = FALSE, redir = FALSE;
	char *token;
	char* line = malloc(100*sizeof(char));
	strcpy(line,line0);

	token = strtok(line, TOK_DELIM);
	while (token){
		if (*token == ';' ){	// new command string
			char* tmp1 = malloc(BUFSIZE * sizeof(char));
			char** tmpa = malloc(BUFSIZE * sizeof(char*));
			strcpy(tmp1, token);
			tmp1[sizeof(token)] = '\0';
			tmpa[0]=tmp1;
			cmd[cmdp] = tmpa;
			argp = 0;
			cmdp++;
			com = FALSE;
			redir = FALSE;
		}
		else if (*token == '>' || *token == '<' || token == ">>"){	// redirects
			argp = 0;
			char* tmp1 = malloc(BUFSIZE * sizeof(char));
			char** tmpa = malloc(BUFSIZE * sizeof(char*));
			strcpy(tmp1, token);
			tmp1[sizeof(token)] = '\0';
			tmpa[argp]=tmp1;
			argp++;
			printf("Redirect: %s\n",tmp1);
			com = FALSE;
			redir = TRUE;
		}
		else if (*token == '|'){		// pipe
			printf("PIPE\n");
			cmdp++;
			argp = 0;
			com = FALSE;
		}
		else if (redir){		// redirect file name
			// redirect token stored in arg[]
			char* tmp1 = malloc(BUFSIZE * sizeof(char));
			char** tmpa = malloc(BUFSIZE * sizeof(char*));
			strcpy(tmp1, token);
			tmp1[sizeof(token)] = '\0';
			tmpa[argp]=tmp1;
			cmd[cmdp]=tmpa;
			argp = 0;
			cmdp++;
			redir = FALSE;
			com = FALSE;
			printf("File: %s\n", token);
		}
		else if (*token == '&')		// background
		{
			cmdp++;
			argp = 0;
			char* tmp1 = malloc(BUFSIZE * sizeof(char));
			char** tmpa = malloc(BUFSIZE * sizeof(char*));
			strcpy(tmp1, token);
			tmp1[sizeof(token)] = '\0';
			tmpa[0]=tmp1;
			cmd[cmdp]=tmpa;

			printf("Background");
		}
		else if (!com && !redir){	// command entered
			argp = 0;
			char* tmp1 = malloc(BUFSIZE * sizeof(char));
			char** tmpa = malloc(BUFSIZE * sizeof(char*));
			strcpy(tmp1, token);
			tmp1[sizeof(token)] = '\0';
			tmpa[argp] = tmp1;

			argp++;
			printf("Command %s\n", token);
			com = TRUE;
		}
		else if (com){		// argument to command, all other redirects and pipes taken care of
			char* tmp1 = malloc(BUFSIZE * sizeof(char));
			char** tmpa = malloc(BUFSIZE * sizeof(char*));
			strcpy(tmp1, token);
			tmp1[sizeof(token)] = '\0';
			tmpa[argp] = tmp1;
			argp++;
			printf("Argument: %s\n", token);
			//cmd[cmdp] = argv;		// save current working argument array
			//cmdp++;
		}
		// end of if else statements

		token = strtok(NULL, TOK_DELIM);



	}	// end of while
	cmdp++;
	cmd[cmdp] = NULL;
}