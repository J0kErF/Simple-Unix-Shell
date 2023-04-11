#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE 100

// define a struct to represent a history item
struct history_item
{
    char *command;
    struct history_item *next;
};

// global variable to keep track of the head of the history linked list
struct history_item *history_head = NULL;
int count = 0;
// function to add a command to the history list
void add_history_item(char *command)
{
    // create a new history item
    struct history_item *item = malloc(sizeof(struct history_item));
    item->command = malloc(strlen(command) + 1);
    strcpy(item->command, command);

    // adding the command to the head of the linked list
    item->next = history_head;
    history_head = item;
    count++;
}

// function to print the entire history list
void print_history()
{
    int i = count;
    struct history_item *curr = history_head;
    while (curr != NULL)
    {
        printf("%d \t %s", i, curr->command);
        curr = curr->next;
        i--;
    }
}
// function to get a number starting from pos(1)
int find_first_number(const char *str)
{
    if (str == NULL || strlen(str) == 0)
    {
        return 0; // empty or NULL string
    }

    if (str[0] != '!')
    {
        return 0; // string doesn't start with an exclamation mark
    }

    int i = 1; // start from the second character
    int num = 0;
    while (isdigit(str[i]))
    {
        num = num * 10 + (str[i] - '0');
        i++;
    }

    return num;
}
// function to parse a command string into an array of arguments
int command_parser(char *args[], char command[])
{
    int i;
    char *word = strtok(command, " ");
    for (i = 0; word != NULL; i++)
    {
        args[i] = word;
        word = strtok(NULL, " ");
    }
    if (i > 0 && strcmp(args[i - 1], "&") == 0)
    {
        args[i - 1] = NULL;
        return 0;
    }
    return 1;
}

int main(void)
{
    close(2);
    dup(1);
    char command[BUFFER_SIZE];
    char *args[BUFFER_SIZE];
    int waitf = 0;

    while (1)
    {
        fprintf(stdout, "my-shell> ");
        memset(command, 0, BUFFER_SIZE);
        fgets(command, BUFFER_SIZE, stdin);

        // check if the command is empty
        int empty_command = 1;
        for (int i = 0; i < strlen(command); i++)
        {
            if (!isspace(command[i]))
            {
                empty_command = 0;
                break;
            }
        }
        if (empty_command)
        {
            continue; // skip empty command and prompt user again
        }
        
        if (strncmp(command, "exit", 4) == 0)
        {
            break;
        }
        else
        {
            if (strncmp(command, "!", 1) == 0)
            {
                // execute command from history list

                int command_num = find_first_number(command);

                if (command[1] == '!')
                {
                    command_num = count;
                    if(command_num==0)
                    {
                        printf("No History\n");
                        continue;
                    }
                }
                if (command_num > 0)
                {
                    struct history_item *curr = history_head;
                    int i = count;
                    while (curr != NULL && i != command_num)
                    {
                        curr = curr->next;
                        i--;
                    }
                    if (curr != NULL)
                    {
                        // execute the command
                        strncpy(command, curr->command, BUFFER_SIZE);
                        
                    }
                    else
                    {
                        printf("No History\n");
                        continue;
                    }
                }
            }
            if (strncmp(command, "history", 7) == 0)
            {
                // print the history list
                add_history_item(command);
                print_history();
                continue;
            }
            
        }

        add_history_item(command);
        strtok(command, "\r\n");
        memset(args, 0, sizeof(args));
        waitf = command_parser(args, command);

        pid_t pid = fork();

        if (pid == 0)
        {
            if (execvp(args[0], args) < 0)
            {
                perror("error");
            }
            exit(0);
        }
        else if (pid > 0)
        {
            if (waitf)
            {
                while (wait(NULL) != pid)
                {
                }
            }
        }
        else
        {
            perror("error");
            exit(0);
        }
    }

    return 0;
}
