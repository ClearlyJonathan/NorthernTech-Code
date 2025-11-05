#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
// Methods
void interface();
int startsWith(const char* str, const char* prefix);
int builtIn(char* userInput);

int main()
{
    // Create space in memory for the commands
    char userInput[100];
    interface();
    
    do
    {
        printf("virtual shell > ");
        // Gets the user input - Limited it to 100 characters
        fgets(userInput, 100, stdin);
        userInput[strcspn(userInput, "\n")] = 0; // removes \n


        int status = 0;
        switch (builtIn(userInput))
        {
        case 1:
            system(userInput);
            break;
        case 2:
            printf("Changing directories\n");
            char* dir = userInput;
            dir += 3; 
            chdir(dir);
            system("pwd");
            break;
        case -1:
            printf("Exiting\n");
            exit(1);
            break;
        // We assume that anything not recognized is a request to open an application - The user is responsible for correct name and type.
        default:
            char* start = userInput;
            char* items[] = {"/", "./", "~"};
            int hasExecuted = 0;
            pid_t forkID = fork();

            // Transform child into new process
            if (forkID == 0)
            {
                // Local search - Built in
                for (int i = 0; i < 3; i++)
                {
                    if (startsWith(userInput, items[i]))
                    {
                        char* args[] = { userInput, NULL };
                        hasExecuted = 1;
                        printf("\nLaunching:%s\n", userInput);
                        int f = execv(userInput, args); // Only returns if an error has occured. Error mode? Maybe use perror
                        if (f < 0)
                        {
                            perror("An error has occurred.");
                            exit(f);
                        }
                    }
                }

                // If command not in built in. AKA hasnt executed yet.
                if (hasExecuted == 0) 
                {
                    // Find path
                    char path[4000];
                    strncpy(path, getenv("PATH"), sizeof(path));
                    char* pathToTry = strtok(path, ":");
                    char absPath[1024];
                    while (pathToTry != NULL)
                    {

                        snprintf(absPath, sizeof(absPath), "%s/%s", pathToTry, userInput);
                        if (access(absPath, F_OK) == 0)
                        {
                            printf("Found");
                            char* args[] = { absPath, NULL };
                            int f = execv(absPath, args);
                            if (f < 0)
                            {
                                perror("An error has occurred,");
                                exit(f);
                            }
                        }
                        pathToTry = strtok(NULL, ":");
                    }
                }
            }
            // Wait for child process to finish
            else
            {
                wait(&status);

                if (WIFEXITED(status))
                {
                    printf("Ended normally with status code %d\n", WEXITSTATUS(status));
                }
                else
                {
                    printf("Ended abruptally - Killed %d\n", WTERMSIG(status));
                }
            }
           
            break;
        }

    } while (1); //Invariant - I want the program to always run until exitted
    
    return 0;
}

// Stack overflow
int startsWith(const char* str, const char* prefix) {
    return strncmp(str, prefix, strlen(prefix)) == 0;
}


// Method to just print interface
void interface()
{
    printf( "Print the current directory: pwd\n"
            "Change current directory: cd <dir>\n"
            "List contents of current directory: ls\n"
            "To exit virtual bash: exit\n");
}

// Method to check userinput
int builtIn(char* userInput)
{
    if (strcmp(userInput, "pwd") == 0 || strcmp(userInput, "ls") == 0)
    {
        return 1;
    }
    // cd must be followed by space
    else if (strncmp(userInput, "cd", 2) == 0 && userInput[2] == ' ')
    {
        return 2;
    }
    else if (strcmp(userInput, "exit") == 0)
    {
        return -1;
    }
    return 0;
}