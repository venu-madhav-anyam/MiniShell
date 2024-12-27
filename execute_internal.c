#include "msh.h"

extern char prompt[100];
extern char input_str[100];
extern list *head;
extern int status;

void execute_internal_commands(char *ip_str, char *prompt)
{
    char buf[100];

    if (!strcmp(ip_str, "exit"))
    {
        system("clear");
        exit(0);
    }

    if (!strcmp(ip_str, "pwd"))
    {
        getcwd(buf, 100);
        printf("%s\n" RESET, buf);
    }
    else if (!strncmp(ip_str, "cd ", 3) && ip_str[3] != '~')
    {
        int i, count = 0;
        for (i = 2; ip_str[i] == ' '; i++)
            count++;

        if (!chdir(ip_str + count + 2))
            ;
        else
            perror("Error");
    }
    else if (!strcmp(ip_str, "clear"))
    {
        system("clear");
    }
    else if (!strncmp(ip_str, "cd", 2))
    {
        char *b = getenv("HOME");

        if (b)
            chdir(b);
    }
    else if (strncmp(ip_str, "echo", 4) == 0)
    {
        echo(ip_str, status);
    }
    else if (!strcmp(ip_str, "jobs"))
    {
        display_jobs(head);
    }
    else if (!strcmp(ip_str, "fg"))
    {
        if (head)
        {
            printf("%s\n", head->cmnd);

            kill(head->pid, SIGCONT);
            waitpid(head->pid, NULL, WUNTRACED);

            list *temp = head;
            head = head->next;
            free(temp);
        }
        else
            printf("fg: current: no such job\n");
    }
    else if (!strcmp(ip_str, "bg"))
    {
        if (head)
        {
            printf("%s\n", head->cmnd);

            kill(head->pid, SIGCONT);

            list *temp = head;
            head = head->next;
            free(temp);
        }
        else
            printf("bg: current: no such job\n");
    }
}
