#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define MAX_CMDS 20
#define MAX_BUF 80

int main()
{
	// the while(1) loop simulates a terminal
	// buf is the terminal line that commands are written to
	// those commands are processed character by character to variable 'ch'
	// and strung together to form commands, 'cmd', that are placed into argv

	char *argv[MAX_CMDS], buf[MAX_BUF], ch, *cmd;
	int argc, status, inword, continu;

	while(1)
	{
		inword = 0;
		cmd = buf;
		argc = 0;
		continu=0;

		printf( "\nshhh> ");

		while ( ( ch = getchar() ) != '\n'  || continu ) {
			if ( ch ==  ' ' && inword )
			{
				inword = 0;
				*cmd++ = 0;
			}
			else if ( ch == '\n' ) continu = 0;
			else if ( ch == '\\' && !inword ) continu = 1;
			else if ( !inword )
			{
				inword = 1;
				argv[argc++] = cmd;
				*cmd++ = ch;
			}
			else *cmd++ = ch;
		}

		*cmd++ = 0;
		argv[argc] = 0;

		if ( argv[0] == 0 ) continue; // no input prompt again without processing
		if ( strcmp(argv[0],"exit") == 0 ) exit(0);

        int pipes = 0, start[MAX_CMDS], redir_in = 0, redir_out = 0;
		char *infile, *outfile;

		start[0] = 0; // first process handles first arg

        // Preprocessor Section
		// Only exepects infile on first pipe and outfile for last pipe
		for (int i = 0; i < argc; i++)
        {
			if ( strcmp(argv[i],"|") == 0 ) // determine # pipes & replace with 0
			{
				start[++pipes] = i + 1; // next process starts at next arg
				argv[i] = 0;
			}
			else if ( strcmp(argv[i],"<") == 0 ) // input redirection
			{
				redir_in = 1; // set flag true
				argv[i] = 0;
				infile = argv[i + 1]; // expects next arg to be filename
			}
			else if ( strcmp(argv[i],">") == 0 ) // output redirection
			{
				redir_out = 1;
				argv[i] = 0;
				outfile = argv[i + 1]; // expects next arg to be filename
			}
        }

        int r_fd[2], l_fd[2];
        for (int j = 0; j <= pipes; j++)
        {
			// only need to create one pipe as it's shared between left & right
            if(pipes) pipe(r_fd);

            pid_t pid = fork();

            if(pid < 0) // invalid process
            {
                fprintf(stderr, "Fork Failed");
                return -1;
            }
            else if (pid > 0) // parent process
            {
                if (j > 0)
                {
                    close(l_fd[0]);
                    close(l_fd[1]);
                }

				l_fd[0] = r_fd[0]; // left becomes previous right
				l_fd[1] = r_fd[1];
            }
            else if (pid == 0) // child process
            {
                if (redir_in && j == 0) // first child read in
                {
					int n = open(infile, O_RDONLY);
					close(0); // close process input
					dup(n); // duplicate infile into input
					close(n);
                }

                if (pipes)
                {
                    if (j == 0) // first child
                    {
                        // output to r_fd
						close(1);
						dup(r_fd[1]);
						close(r_fd[0]);
						close(r_fd[1]);
                    }
					else if (j == pipes) // last child
                    {
                        // input from l_fd
						close(0);
						dup(l_fd[0]);
						close(l_fd[0]);
						close(l_fd[1]);
                    }
                    else // middle child
                    {
                        // input from l_fd
						close(0);
						dup(l_fd[0]);
						close(l_fd[0]);
						close(l_fd[1]);

						// output to r_fd
						close(1);
						dup(r_fd[1]);
						close(r_fd[0]);
						close(r_fd[1]);
                    }
                }

                if (redir_out && j == pipes) // last child write out
                {
					int n = creat(outfile, 0754);
					close(1); // close process output
					dup(n); // duplicate outfile into output
					close(n);
                }

                execvp(argv[start[j]], &argv[start[j]]); // path holds reference to point in argv

                // If this line reached, execv failed
                fprintf(stderr, "Invalid Command\n");
                exit(-1);
            }
			wait(&status);
        }
	}
    return(0);
}
