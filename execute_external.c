#include "msh.h"

extern pid_t child_pid;

void execute_external_commands(char *ip_str)
{
    char *args[10];
    int arg_count = 0, k = 0;
    int pos[10];
    int pipe_count = 0;
    pos[k++] = 0;

    char *token = strtok(ip_str, " ");
    while (token != NULL)
    {
        if (strcmp(token, "|") == 0)
        {
            pos[k++] = arg_count + 1;
            args[arg_count++] = NULL;
            pipe_count++;
        }
        else
        {
            args[arg_count++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;

    int fd[2];
    pid_t pid;
    int prev_fd = -1;

    if (!pipe_count)
    {
        execvp(args[0], args);
        return;
    }

    for (int i = 0; i <= pipe_count; i++)
    {
        if (i < pipe_count)
            pipe(fd);

        pid = fork();

        if (pid == 0)
        {
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (i < pipe_count)
            {
                close(fd[0]);
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
            }

            execvp(args[pos[i]], args + pos[i]);
        }
        else if (pid > 0)
        {
            waitpid(pid, NULL, WUNTRACED);

            if (prev_fd != -1)
                close(prev_fd);

            if (i < pipe_count)
                close(fd[1]);

            prev_fd = fd[0];
        }
    }

    exit(0);
}