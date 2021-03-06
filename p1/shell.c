DESIGN                                                                                              000644  000765  000024  00000010041 12267062653 013633  0                                                                                                    ustar 00gramasaurous                    staff                           000000  000000                                                                                                                                                                         DESIGN - PROJECT 1
cs111-w14
Graham Greving
ggreving@ucsc.edu

The G(raham)Shell

Specifications:
1. Internal Shell Command, Exit
	>> exit
2. Command with no arguments
	>> ls
3. Command with arguments
	>> ls -R /
4. A command, w/ or w/o arguments, whose output is redirected to a file specified on the command line.
	>> ls -R / > file1
4. A command, w/ or w/o arguments, who input is redirected from a file specified on the command line.
	>> wc < file1
5. Commands separated by the "&&" operator. 
	>> touch file && wc file
6. A command, w/ or w/o arguments, executed in the background.
	>> sleep 5 &

7. Any combination of the above features.

	>> ls -R / > file1 && wc < file1 &

Global Variables:
	int bg:
		Defines a command is to be executed in the background.
	int zombie:
		Defines a backgrounded process has terminated (set by the signal handler), value is that of the completed process id. Else, set to -1.
	int fdout:
		File descriptor of file to be read for input. If not open, set to -1.
	int fdin:
		File descriptor of file to be read for input. If not open, set to -1.

Functions:
	static void sig_chld(int signo):
		Catches SIGCHLD, sets a global variable to inform the shell that a background process has been waited on in order to print the information.
	int exec_command(char **cmdstring):
		Accepts a command string to execute with execvp. Also uses a few global variables to react to different shell cases. If the redirect files are open (fd >= 0), then they are duplicated with the stdout or stdin and the original is closed. if the background flag is set, then the parent process will not wait for the child, and the appropriate signal handler is set.
	int parseargv(int argc, char **argv):
		Takes in a set of tokens from the lex replacement function, and the number or tokens. Initializes any global variables, sets up a command string on the stack. Checks first token for "exit", acts accordingly. Loops over each token checking for special characters:
			"&" sets the bg flag
			"&&" executes the current cmdstring (via exec_command), if it returns with 0, then it calls parseargv(token+1) in order to process the remaining commands. After successful or unsuccsesful execution, returns from the function accordingly.
			">" opens the file specified in the following token. fdout is used as fd.
			"<" opens the file specified in the following token. fdin is used as fd.
		If no special character is found, then the token is a "normal word" and is placed on the cmdstring.
		Once all tokens have been processed, parseargv calls exec_command with the current cmdstring then closes and open file descriptors.
	int makeargv(const char *s, const char *delimiters, char ***argvp):
		From starter code, returns a list of tokens created from delimiters and input string.
	main:
		Sets up all global variables. Checks to see if any backgrounded processes have finished, if so, prints accordingly. Prints the prompt, recieves input from stdin (fgets), if fgets fails, clearerr(stdin) is called to clear the input buffer, preventing a bug on MINIX where a signal handler interrupts a system call, which creates an infinte prompt loop with no input possibility.
		Gets the tokens from the input.
		Parses the tokens.
		Frees the tokens.


Control Flow:
variables:
	cmdstring (string array)
	fdin (file descriptor)
	fdout (file descriptor)

while (1):
	getline (fgets)
	gettokens (makeargv)

	if token[0] == "exit"
		exit
	for token in tokens:
		switch token:
			case "&":
				set bgflag true
			case "&&":
				exec_cmd cmdstring, if returns OK, parse tokens+1
				continue
			case ">":
				fdout = open ++token
			case "<":
				fdin = open ++token
			default:
				cmdstring += token
	exec_cmd cmdstring

exec_cmd:
	fork
	error:
		exit
	child:
		if fdout is open:
			duplicate fdout, stdout
			close fdout
		if fdin is open:
			duplicate fdin, stdin
			close fdin
		exec cmdstring
	parent:
		if fdout is open:
			close fdout
		if fdin is open:
			close fdin
		if bg flag is set:
			set up signal handler to catch SIGCHLD
			print "background pid"
		else:
			set up signal handler to ignore SIGCHLD
		wait  for child
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               ./._Makefile                                                                                        000644  000765  000024  00000001040 12266336541 014747  0                                                                                                    ustar 00gramasaurous                    staff                           000000  000000                                                                                                                                                                             Mac OS X            	   2  �                                            ATTR          �  X                  �    %com.apple.metadata:kMDItemWhereFroms   �   >  com.apple.quarantine bplist00�_�https://s3.amazonaws.com/piazza-resources/hpvmf4tj33s6oq/hqh7nfgf1m242d/Makefile?AWSAccessKeyId=AKIAJKOQYKAYOBKKVTKQ&Expires=1390012305&Signature=vi8Vxr0hcFa3uu2tTkvID4u%2FyL4%3D_4https://piazza.com/ucsc/winter2014/cmps111/resources�                            �q/0001;52d9bd61;Chromium;FD08F35F-3349-4093-B52D-7DFF03ADEDAD                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 Makefile                                                                                            000644  000765  000024  00000000253 12266336541 014402  0                                                                                                    ustar 00gramasaurous                    staff                           000000  000000                                                                                                                                                                         all: shell

shell: shell.o makeargv.o
	gcc -o shell shell.o makeargv.o

shell.o: shell.c
	gcc -c shell.c

makeargv.o: makeargv.c
	gcc -c makeargv.c

clean:
	rm shell *.o

                                                                                                                                                                                                                                                                                                                                                     README                                                                                              000644  000765  000024  00000001407 12267066704 013626  0                                                                                                    ustar 00gramasaurous                    staff                           000000  000000                                                                                                                                                                         README - PROJECT 1
cs111-w14
Graham Greving
ggreving@ucsc.edu

The G(raham) Shell

To Compile:
	$ make clean
	$ make all

To Run:
	$ ./gshell

To Use:
	The G(raham) Shell operates similar to any other shell, albeit with reduced functionality. Enter commands at the prompt:
		>> rm -Rf *
	^ probably not a good idea.
	The G(raham) Shell accepts commands with and without arguments, input and output redirection:
		>> ls -R / > file
		>> wc < file
	As well as compounding commands:
		>> ls -R && wc file
	And backgrounding processes:
		>> sleep 5 &
		Backgrounding 1231...
		>> 1234 Terminated!
		>>
	Additionally, the internal command "exit" is supported:
		>> exit
		Exiting!
	As an added bonus, the user can change the prompt characters:
		>> chp g:
		Changing prompt!
		g:
                                                                                                                                                                                                                                                         ./._makeargv.c                                                                                      000644  000765  000024  00000000754 12266317175 015265  0                                                                                                    ustar 00gramasaurous                    staff                           000000  000000                                                                                                                                                                             Mac OS X            	   2  �     �                                      ATTR      �   �  $                  �   �  %com.apple.metadata:kMDItemWhereFroms   �   >  com.apple.quarantine bplist00�_�https://s3.amazonaws.com/piazza-resources/hpvmf4tj33s6oq/hqh8za5rhhb1lq/makeargv.c?AWSAccessKeyId=AKIAJKOQYKAYOBKKVTKQ&Expires=1390004386&Signature=5c5hlEXvSJfigCtddtssNl%2FvLKw%3DP�                            �q/0041;52d99e88;Chromium;41BD266C-539F-4C40-9652-209265EA010E                     makeargv.c                                                                                          000644  000765  000024  00000003251 12266317175 014706  0                                                                                                    ustar 00gramasaurous                    staff                           000000  000000                                                                                                                                                                         /* 
 * makeargv.c - Tokenizer with user-specified delimiters
 *
 * Taken from the source code for the book
 * "UNIX Systems Programming: Communication, Concurrency, and Threads"
 * written by Kay A. Robbins & Steven Robbins
 *
 */

#include <errno.h>
#include <stdlib.h>
#include <string.h>

int makeargv(const char *s, const char *delimiters, char ***argvp) {
   int error;
   int i;
   int numtokens;
   const char *snew;
   char *t;

   if ((s == NULL) || (delimiters == NULL) || (argvp == NULL)) {
      errno = EINVAL;
      return -1;
   }
   *argvp = NULL;                           
   snew = s + strspn(s, delimiters);         /* snew is real start of string */
   if ((t = malloc(strlen(snew) + 1)) == NULL) 
      return -1; 
   strcpy(t, snew);               
   numtokens = 0;
   if (strtok(t, delimiters) != NULL)     /* count the number of tokens in s */
      for (numtokens = 1; strtok(NULL, delimiters) != NULL; numtokens++) ; 

                             /* create argument array for ptrs to the tokens */
   if ((*argvp = malloc((numtokens + 1)*sizeof(char *))) == NULL) {
      error = errno;
      free(t);
      errno = error;
      return -1; 
   } 
                        /* insert pointers to tokens into the argument array */
   if (numtokens == 0) 
      free(t);
   else {
      strcpy(t, snew);
      **argvp = strtok(t, delimiters);
      for (i = 1; i < numtokens; i++)
          *((*argvp) + i) = strtok(NULL, delimiters);
    } 
    *((*argvp) + numtokens) = NULL;             /* put in final NULL pointer */
    return numtokens;
}

void freemakeargv(char **argv) {
    if (argv == NULL)
        return;
    if (*argv != NULL)
        free(*argv);
    free(argv);
}

                                                                                                                                                                                                                                                                                                                                                       shell.c                                                                                             000644  000765  000024  00000012105 12267066672 014222  0                                                                                                    ustar 00gramasaurous                    staff                           000000  000000                                                                                                                                                                         #define _POSIX_SOURCE   /* for MAX_INPUT */
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

#define PROMPT          "$: "
#define DELIMITERS      " \t" /* whitespace, tab */

// Function declarations
int makeargv(const char *s, const char *delimiters, char ***argvp);
int freemakeargv(char **argv);
int parseargv(int argc, char **argv);
int exec_command(char **cmdstring);
static void sig_chld(int);
int chp(char *newprompt);


char prompt[4];

// Global background and zombie flags
int bg;
int zombie;
int fdout;
int fdin;


int main(int argc, char **argv) {

	char input[MAX_INPUT];
	int numtokens;
	char **tokens;

	prompt[0] = '>';
	prompt[1] = '>';
	prompt[2] = ' ';
	prompt[3] = '\0';

	// Init the flags
	zombie = -1;
	bg = 0;

	// Loop indefinitely.
	while (1) {
		// Clear the background flag 
		bg = 0;
		// Check for finished zombies.
		if (zombie > 0) {
			printf("Process %d terminated.\n", zombie);
			zombie = -1;
		}

		// Print the prompt!
		printf("%s", prompt);
		
		// Get the input line!
		// Move on if input is null or empty newline
		if (fgets(input, MAX_INPUT, stdin) == NULL) {
			// Fixes a bug where the signal handler interrupts the read system
			// call and forces an infinite prompt loop.
			// Thanks to Daniel Bittman and Justin Lardinois on Piazza :)
			clearerr(stdin);
			continue;
		}
		if (input[0] == '\n') continue;
		
		// Cut trailing newline
		int inputlen = strlen(input);
		if (input[inputlen - 1] == '\n') input[inputlen - 1] = 0;
		
		// Get the tokens
		numtokens = makeargv(input, DELIMITERS, &tokens);
		// Parse the command line!
		parseargv(numtokens, tokens);
		
		// Clean up
		freemakeargv(tokens);
	}
}

// Given an input string from the command line, parses
// for special characters, &, &&, >, <, acts accordingly,
// forks and executes the necessary processes.
int parseargv(int argc, char **argv) {
	int n = 0;
	int i = 0;
	fdin = -1;
	fdout = -1;
	char *cmdstring[argc];

	// Check for 'exit'
	if (strcmp(argv[0], "exit") == 0) {
		freemakeargv(argv);
		printf("Exiting!\n");
		exit (1);
	} else if (strcmp(argv[0], "chp") == 0) {
		chp(argv[1]);
		return (-1);
	}

	// Loop through the tokens, creating the command string
	for (i = 0; i < argc; i++) {
		if (strcmp(argv[i],"&") == 0) {					// Background process
			bg = 1;
		}  else if (strcmp (argv[i], "&&") == 0) {		// Compound execution
			if (exec_command(cmdstring) == 0) {
				parseargv(argc - i, &argv[i + 1]);
				return (1);
			} else {
				return (-1);
			}
		} else if (strcmp(argv[i], ">") == 0) {			// Output redirection
			// Open file specified right after the special character
			fdout = open(argv[++i], O_CREAT | O_WRONLY, 0400 | 0200);
			if (fdout < 0) {
				perror("parseargv():open()");
				return (-1);
			}
		} else if (strcmp(argv[i], "<") == 0) { 	// Input redirection
			// Open file specified right after the special character
			fdin = open(argv[++i], O_RDONLY);
			if (fdin < 0) {
				printf("Error: file cannot be opened for reading.\n");
				return (-1);
			}
		} else {									// To be executed.
			cmdstring[n++] = argv[i];
		}
	}
	// Null terminate the command string
	cmdstring[n] = 0;
	// Execute the command!
	if (exec_command(cmdstring) < 0) {
		exit (-1);
	}

	// Close files in case they didn't get closed
	close (fdin);
	close (fdout);
	return (1);

}

int exec_command(char **cmdstring) {
	// Fork and execute the command string
	int pid = fork();
	if (pid < 0) {				// Fork error
		perror("parseargv():fork()");
		return (-1);
	} else if (pid == 0) {		// Child, execute
		// Handle I/O redirection
		if (fdout >= 0) {
			dup2(fdout, 1);
			close(fdout);
		} if (fdin >= 0) {
			dup2(fdin, 0);
			close(fdin);
		}
		// Execute the command string.
		if (execvp(cmdstring[0], cmdstring) < 0) {
			perror("parseargv():execvp()");
			return(-1);
		} else {
			exit(1);
		}
	} else {					// Parent, wait
		//printf("parent. fdout: %d. fdin: %d\n", fdout, fdin);
		int status = 0;
		if (fdout >= 0) {
			close(fdout);
		} if (fdin >= 0) {
			close(fdin);
		}
		// Process should be backgrounded.
		// Set up the SIGCHLD signal handler and print
		// the backgrounded process.
		if (bg == 1) {
			if (signal(SIGCHLD, sig_chld) == SIG_ERR) {
				perror("parseargv():signal()");
				return (-1);
			}
			printf("background: %d\n", pid);
			return (1);
		}  else {
			// Ignore SIGCHLD
			if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
				perror("parseargv():signal()");
			}
			// Wait normally.
			waitpid(pid, &status, 0);
			//printf("%d\n", status);
			return (1);
		}
	}
}

// Catch SIGCHLD
// Wait for the zombie, and set global zombie
// variable to the zombies pid
static void sig_chld(int signo) {
	int status;
	if ((zombie = wait(&status)) < 0) {
		perror("sig_chld():wait():");
	}
}


// Change the prompt character.
int chp(char *newprompt) {
	if (newprompt == NULL) {
		return (-1);
	} else if (strlen(newprompt) != 2) {
		printf("Sorry. That prompt is too long.\n");
		return (-1);
	} else {
		prompt[0] = newprompt[0];
		prompt[1] = newprompt[1];
		printf("Changing Prompt!\n");
		return 1;
	}
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           