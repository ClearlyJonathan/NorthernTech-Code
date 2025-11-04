#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>

// Methods
void interface();
int builtIn();

int main()
{
    // Create space in memory for the commands
    char userInput[100 * sizeof(char)];
    interface();
    
    do
    {
        // Gets the user option - Limited it to 100 characters
        fgets(userInput, 100, stdin);
        userInput[strcspn(userInput, "\n")] = 0; // removes \n


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
        // We assume that anything not recognized is a request to open an .exe
        default:
            int forkID = fork();

            // Child 
            if (forkID == 0) // Transform child process into new process (app)
            {
                char* args[] = { userInput, NULL };
                printf("Launching:%s\n", userInput);
                int f = execv(userInput, args);
                if (f < 0)
                {
                    printf("An error has occurred. Code :%d", f);
                    exit(f);
                }
            }
            break;
        }

    } while (true);
    
    return 0;
}

void interface()
{
    printf( "Print the current directory: pwd\n"
            "Change current directory: cd <dir>\n"
            "List contents of current directory: ls\n"
            "To exit virtual bash: exit\n");
}

int builtIn(char* userInput)
{
    if (strcmp(userInput, "pwd") == 0 || strcmp(userInput, "ls") == 0)
    {
        return 1;
    }
    else if (strstr(userInput, "cd") != NULL)
    {
        return 2;
    }
    else if (strcmp(userInput, "exit") == 0)
    {
        return -1;
    }
    return 0;
}