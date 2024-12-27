#include "msh.h"

int cmd_count = 0;
char *external_cmds[200];
int status;
extern char prompt[100];
extern char input_str[100];

int check_command_type(char *command)
{
    char *builtins[] = {"clear", "echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
                        "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
                        "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", "fg", "bg", "jobs", NULL};

    for (int i = 0; builtins[i] != NULL; i++)
    {
        if (strcmp(builtins[i], command) == 0)
            return BUILTIN;
    }

    for (int i = 0; external_cmds[i] != NULL; i++)
    {
        if (strcmp(external_cmds[i], command) == 0)
            return EXTERNAL;
    }

    if (!strncmp(command, "./", 2))
    {
        return EXTERNAL;
    }

    return NO_COMMAND;
}

// To Seperate main cmnd from string
char *get_command(char *input_string)
{
    int i = 0;
    char *cmd = (char *)malloc(sizeof(char) * 20);

    if (!cmd)
    {
        perror(RED "Memory Allocation Failed\n" RESET);
        return NULL;
    }

    while (input_string[i] != ' ' && input_string[i] != '\0')
    {
        cmd[i] = input_string[i];
        i++;
    }

    cmd[i] = '\0';

    return cmd;
}

// Populating Commands
void extract_external_commands(char **external_commands)
{
    char c, data[100];
    int i, len;
    int fd = open("external_cmds.txt", O_RDONLY);

    if (fd == -1)
    {
        perror(RED "Error : File doesn't exist.\n" RESET);
        return;
    }

    while (1)
    {
        i = 0;

        while (read(fd, &c, 1) > 0 && c != '\n')
            data[i++] = c;

        if (i == 0 && (read(fd, &c, 1) <= 0))
            break;

        data[i] = '\0';

        len = strlen(data);

        external_commands[cmd_count] = (char *)malloc((len + 1) * sizeof(char));

        if (external_commands[cmd_count] == NULL)
        {
            perror(RED "Memory allocation failed\n" RESET);
            break;
        }

        strcpy(external_commands[cmd_count++], data);
    }
    external_cmds[cmd_count] = NULL;

    close(fd);
}

// Echo Cmnds
void echo(char *input_string, int status)
{
    if (strncmp("echo $?", input_string, 7) == 0)
    {
        printf("%d\n", status);
    }

    if (strncmp("echo $$", input_string, 7) == 0)
    {
        printf("%d\n", getpid());
    }

    if (strncmp("echo $SHELL", input_string, 11) == 0)
    {
        char *p = getenv("SHELL");
        printf("%s\n", p);
    }
}
