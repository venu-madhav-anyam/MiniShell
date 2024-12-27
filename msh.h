#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

#define BUILTIN 1
#define EXTERNAL 2
#define NO_COMMAND 3
#define SUCCESS 0
#define FAILURE -1

#define RED     "\x1b[1;31m"
#define GREEN   "\x1b[1;32m"
#define YELLOW  "\x1b[1;33m"
#define BLUE    "\x1b[1;34m"
#define MAGENTA "\x1b[1;35m"
#define CYAN    "\x1b[1;36m"
#define RESET   "\x1b[0m"

void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);

int check_command_type(char *command);
void echo(char *input_string, int status);
void execute_internal_commands(char *input_string, char *prompt);
void execute_external_commands(char *input_string);
void signal_handler(int sig_num);
void extract_external_commands(char **external_commands);

typedef struct list
{
    struct list *next;
    pid_t pid;
    char cmnd[20];
}list;


int insert_at_first(list **head, pid_t pid, char *cmnd);
void display_jobs(list *head);

#endif