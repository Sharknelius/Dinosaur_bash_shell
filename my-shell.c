/**
 * My simple shell interface
 * Last modified: 10/21/2024
 * A C program that acts as a bash shell.
 */

#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h> // For wait()
#include <sys/types.h> // For pid_t

#define MAX_LINE 80 /* 80 chars per line, per command */

#define RED "\x1b[31m" // Error color
#define BLUE "\x1b[34m" // motd and prompt
#define RESET "\x1b[0m"

// Function prototypes
void motd(void); // Initial message printed at startup
void forking(char **args, int background); // Take a task and begin forking process
int read_command(char *command, char *args[]); // Reads the user's string command

int main(void)
{
	motd();

	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
	char command[MAX_LINE];  // Line
    int should_run = 1; // Flag to run or exit
	
	// Main loop that waits for user input	
    	do
	{   
		int background = 0;

        	printf(BLUE "dino-bash>" RESET);
        	fflush(stdout);
        
        	/**
         	 * After reading user input, the steps are:
         	 * (1) fork a child process
         	 * (2) the child process will invoke execvp()
         	 * (3) if command included &, parent will invoke wait()
         	 */

		if (fgets(command, MAX_LINE, stdin) == NULL)
		{
			printf(RED "Could not read user input.\n" RESET);
			continue;
		}

		// Removes "\n" from user command
		command[strcspn(command, "\n")] = '\0';

		// If no input
		if (command[0] == '\0')
		{
			printf(RED "RAWR INPUT SOMETHING.\n" RESET);
			continue;
		}
		// Set background to read_command output
		background = read_command(command, args);

		if (strcmp(args[0], "exit") == 0)
		{
			should_run = 0; // End loop
			continue;
		}

		if (strcmp(args[0], "cd") == 0)
		{
			if (args[1] != NULL)
			{
				if (chdir(args[1]) != 0)
				{
					printf(RED "Failed to change directories.\n" RESET);
				}
			}
			else
			{
				printf(RED "Expected specified directory.\n" RESET);
			}
			continue;
		}

		else
		{
			forking(args, background);
		}
	} while(should_run);
		
    	printf(BLUE "Goodbye!\n" RESET);
	return EXIT_SUCCESS;
}

void motd(void)
{
	printf( BLUE "Welcome to Dino Bash. Have fun I guess.\n" RESET );
}

void forking(char **args, int background)
{
	// Fork child process
	pid_t pid = fork();
	int status;	

	if (pid < 0)
	{
		printf(RED "PID cannot be less than 0.\n" RESET);
	}
	else if (pid == 0)
	{
		if (execvp(args[0], args) == -1)
		{
			printf(RED "Execution failed.\n" RESET);
		}
		exit(1);
	}
	else
	{
		
		if (!background)
		{
			printf("---------- Starting program ----------\n");
			waitpid(pid, &status, 0);
			printf("---------- Program ended ----------\n");
		}
	}
}

int read_command(char *command, char *args[])
{ 
	char *user_input = strtok(command, " ");
	int i = 0;

	while (user_input != NULL)
	{
		args[i++] = user_input;
		// strtok splits user_input into command and args
		user_input = strtok(NULL, " ");
	}
	args[i] = NULL;

	// Set background when &
	int background = 0;
	if (strcmp(args[i-1], "&") == 0)
	{
		background = 1;
		args[i-1] = NULL;
	}
	return background;
}
