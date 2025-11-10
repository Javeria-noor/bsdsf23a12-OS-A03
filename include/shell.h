#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_LEN 512
#define MAXARGS 10
#define ARGLEN 30
#define PROMPT "FCIT> "
#define HISTORY_SIZE 20  // Store last 20 commands

// Function prototypes
char* read_cmd(char* prompt, FILE* fp);
char** tokenize(char* cmdline);
int execute(char** arglist);
int handle_builtin(char** arglist);

// History function prototypes
void add_to_history(const char* cmd);            // Add a command to history
void print_history();                             // Print all commands in history
char* get_history_command(int n);                // Get nth command from history
int is_history_command(const char* cmdline);    // Check if command starts with '!'
char* expand_history_command(const char* cmdline); // Replace !n with actual command

#endif // SHELL_H
