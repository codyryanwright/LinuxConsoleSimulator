This program simulates the linux console, and is able to handle the following...

	1. Can execute a command with the accompanying arguments.  
	2. Recognize multiple pipe requests and handle them.  
	3. Recognize redirection requests and handle them.  
	4. Type "exit" to quit the shhh shell.

The program does expect valid commands, but will notify the user of commands it cannot parse.

You will need to compile the programs for execution by running the following commands...

	$gcc -std=c99 -o pre pre.c
	$gcc -std=c99 -o sort sort.c
	$gcc -std=c99 -o linuxConsole linuxConsole.c

You can then run ./linuxConsole and the console will prompt the following...

	shhh>

Sample commands:

shhh>ls 

shhh>ls -t -al

shhh>ls -al > output.txt

shhh>cat output.txt

shhh>ls | more | wc

shhh>./pre < input.txt | ./sort > output.txt  (input.txt has been prepared for you)

shhh> exit
