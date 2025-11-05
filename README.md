# NorthernTech-Code
This project implements a basic interactive shell, supporting builtin commands and can execute external programs using the fork and exec pattern.

The shell runs in an infinite loop until the user exits.
Executes system commands like pwd, ls, or any other executablel.
Displays the exit code for each executed command.

Requirements
A C compiler. gcc task.c

# How to run
After compiling, run the output file ./a.out and you should be met with an interface.

# Examining exit code
The shell is configured in a way that it prints out the status after each executed run, printing out "Ended normally with status code 0" if normal behaviour, and printing "Ended abruptally" if the program was killed


# Example output
<img width="1590" height="813" alt="image" src="https://github.com/user-attachments/assets/2491b747-e764-4434-9535-cd359ab316ba" />


