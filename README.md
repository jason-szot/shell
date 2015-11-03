# shell program
shell assignment for SE 3376

Description:
Write, compile, and run a C program for a custom-shell.

Part 1. Your shell will do the following, using system( ) to handle each command
1. Command prompt (for example, #, ##, %, %%, or of your choice) and be able to change the command prompt by prompt command (e.g., prompt "##" to change the command prompt to ##).
2. Command “exit” and “quit” will exit (to terminate the program).
3. Command “list” will do what ls command does (using system call)


Part 2. Command Parsing (and print its components correctly).
           	Your shell should be able to parse the command from user. 
	Assume my home directory is: /home/owner or you may use ~/ 

1. One command with arguments and options.  
            For example, ls –la  /home/owner
                Command: ls
                Options: -la
                Arguments: /home/owner

2. Two commands with 1 pipe.  
            For example, ls –la  /home/owner
                Command: la
                Options: -la
                Arguments: /home/owner
            For example, ls | wc –l should be parsed and display
            Command: ls
            Pipe
            Command: wc –l

3. Two commands with semicolon. For example, ls ;  date

4. One command with IO redirection symbol (<, >, >>)
            For example, ls  <  junk.txt  >  output.txt
            Command: ls
            File Redirection: < 
            File: junk.txt
            File Redirection: >
            File: output.txt

5. Three commands or more commands with pipe (and with other symbols). For example
           	ls –l | grep *.txt | wc –l
	ls –l | grep *.txt | wc –l ; ls > junk.txt ; ls >> junk1.txt &

