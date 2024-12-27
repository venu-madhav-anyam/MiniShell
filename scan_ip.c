#include "msh.h"

extern int status;
pid_t child_pid;
extern char *external_cmds[200];

void scan_input(char *prompt, char *input_string)
{
    strcpy(prompt, "\x1b[32mMiniShell\x1b[34m~");

    extract_external_commands(external_cmds);

    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);
    signal(SIGCHLD, signal_handler);

    while (1)
    {
        if (!strncmp(prompt, "\x1b[32mMiniShell\x1b[34m~", 10))
        {
            printf(GREEN "%s" BLUE "%s", prompt, getcwd(NULL, 0));
            printf("\x1b[0m$ " RESET);
        }
        else
        {
            printf(GREEN "%s " RESET, prompt);
        }

        scanf("%[^\n]", input_string);
        getchar();

        if (strncmp(input_string, "\n", 1) == 0)
        {
            continue;
        }

        char *cmnd = get_command(input_string);
        int cmd_type = check_command_type(cmnd);

        { // prompt Validation
            char *ps1 = strstr(input_string, "PS1");
            if (ps1 && ps1[3] == '=' && ps1[4] != ' ')
            {
                strcpy(prompt, ps1 + 4);
                continue;
            }
        }

        if (cmd_type == BUILTIN)
            execute_internal_commands(input_string, prompt);
        else if (cmd_type == EXTERNAL)
        {
            int pid = fork();

            child_pid = pid;

            if (pid == 0)
            {
                signal(SIGINT, SIG_DFL);
                signal(SIGTSTP, SIG_DFL);
                signal(SIGCHLD, SIG_DFL);
                execute_external_commands(input_string);
                exit(0);
            }
            else
            {
                waitpid(pid, &status, WUNTRACED);
            }
        }
        else if (cmd_type == NO_COMMAND)
            printf(MAGENTA "Command Not Found\n" RESET);

        input_string[0] = '\n';
        input_string[1] = '\0';
    }
}