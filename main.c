// includes
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// defines
#define EXIT_SUCCESS 0
#define BUFSIZE 1024
#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"
#ifndef NULL
#define NULL 0
#endif

void cmd_loop(void);
char *cmd_read(void);
char **cmd_split(char *);
int cmd_launch(char **args);
int cmd_exec(char **args);

int cmd_prompt(char **args);		//  change prompt string to args
int cmd_exit(char **args);			//  exit shell
int cmd_quit(char **args);			//  exit shell
int cmd_list(char **args);			//  same as ls command (using system call)



char *built_in_cmd_str[] = {
	"prompt",
	"exit",
	"quit",
	"list"
};

// list of functions

int (*built_in_func[]) (char **) = {
	&cmd_prompt,
	&cmd_exit,
	&cmd_quit,
	&cmd_list
};


int main(int argc, char **argv)
{
	putenv("PROMPT=$$");

	cmd_loop();


	return EXIT_SUCCESS;
}

void cmd_loop(void)		// loop to grab commands from user
{
	char *line;
	char **args;
	int status;
	char*** cmd = malloc(BUFSIZE*sizeof(char**));
	

	do 
	{
		const char *prmpt = getenv("PROMPT");
		printf("%s ", prmpt);
		line = cmd_read();			// gets command
		Parse(line);
		args = cmd_split(line);		// parses commands
		status = cmd_exec(args);	// runs command
		free(line);		// free up line memory
		free(args);		// free up args memory
	} while (status);
}

char *cmd_read(void)	// read command line entered
{
	char *line = NULL;
	ssize_t bufsize = 0;
	getline(&line, &bufsize, stdin);
	return line;
}

char **cmd_split(char *line)
{

	int bufsize = TOK_BUFSIZE, pos = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;

	if (!tokens)	// error allocating
	{
		fprintf(stderr, "shell: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, TOK_DELIM);
	while (token != NULL)	// go through line parsing things
	{
		tokens[pos] = token;
		pos++;

		if (pos >= bufsize)	// if it is too long, extend
		{
			bufsize += TOK_BUFSIZE;
			tokens = realloc(tokens,bufsize * sizeof(char*));
			if (!tokens)
			{
				fprintf( stderr, "shell: allocation error\n");
				exit(EXIT_FAILURE);
			}

		}	// end of extending

		token = strtok(NULL,TOK_DELIM);

	}	// end of while

	tokens[pos] = NULL;
	return tokens;
}	// end of cmd_split

int cmd_launch(char **args)
{
	pid_t pid, wpid;
	int status;

	pid = fork();
	if (pid == 0)	// child process to run command
	{
		if (execvp(args[0], args) == -1)	// if executing the command returns an error
		{
			perror("shell");	// print system error
		}
		exit(EXIT_FAILURE);
	}	// end of child
	else if (pid < 0)	// error forking
	{
		perror("shell");	// print system error
	}	// end of fork error
	else
	{
		// parent process
		do
		{
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}	// end of parent

	return 1;
}	// end of cmd_launch

int cmd_exec(char **args)	// handle built in commands before sending to cmd_launch
{
	int i;

	if (args[0] == NULL)	// empty command
	{
		return 1;
	}

	for (i = 0; i < cmd_count_built_in(); i++)	// run through built in commands
	{
		if (strcmp(args[0], built_in_cmd_str[i]) == 0)	// compare to the list of built in commands
		{
			return (*built_in_func[i])(args);	// run the built in command
		}
	}	// no built in commands found if exited this loop

	return cmd_launch(args);
}




/*******************************************
**          Built In Functions            **
*******************************************/

int cmd_count_built_in()
{
	return sizeof(built_in_cmd_str) / sizeof(char *);
};

// built in command functions, returns 1 to continue shell, returns 0 to exit

int cmd_prompt(char **args)		//  change prompt string to args
{
	if (args[1] == NULL)	// no argument after prompt
	{
		fprintf(stderr, "shell: expected argument to \"prompt\"\n");
	}
	else if (setenv("PROMPT", (const char*)args[1],1) != 0)
	{

		printf("Failed to change prompt");
		return 1;
	}
	return 1;
}
int cmd_exit(char **args)			//  exit shell
{
	return 0;
}
int cmd_quit(char **args)			//  exit shell
{
	return 0;
}
int cmd_list(char **args)			//  same as ls command (using system call)
{
	system("ls");
	return 1;
}