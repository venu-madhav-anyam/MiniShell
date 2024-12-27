#include "msh.h"

extern char prompt[100];
extern char input_str[100];
list *head = NULL;
extern pid_t child_pid;

void signal_handler(int sig_num)
{
    if (sig_num != SIGCHLD)
        printf("\n");

    if (strlen(input_str) < 2 && sig_num != SIGCHLD)
    {
        printf("%s", prompt);
        printf("\x1b[0m$ " RESET);
        fflush(stdout);
    }

    if (sig_num == SIGTSTP && strlen(input_str) > 2)
    {
        insert_at_first(&head, child_pid, input_str);
    }

    if (sig_num == SIGCHLD)
        waitpid(-1, NULL, WNOHANG);
}

int insert_at_first(list **head, pid_t pid, char *cmnd)
{
    list *new = malloc(sizeof(list));
    if (!new)
    {
        return FAILURE;
    }

    new->pid = pid;
    strcpy(new->cmnd, cmnd);

    new->next = *head;
    *head = new;

    return SUCCESS;
}

// Displaying Suspended Processes
void display_jobs(list *head)
{
    list *temp = head;

    while (temp)
    {
        printf("Pid :- %d   Cmnd :- %s\n", temp->pid, temp->cmnd);

        temp = temp->next;
    }
}