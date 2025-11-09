#include "shell.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * execute - handles builtins (cd, exit, help, jobs) and runs external commands.
 * Signature matches main.c: int execute(char **arglist)
 */
int execute(char* arglist[]) {
    if (arglist == NULL || arglist[0] == NULL) {
        return 0;
    }

    /* Built-in: cd */
    if (strcmp(arglist[0], "cd") == 0) {
        if (arglist[1] == NULL) {
            fprintf(stderr, "FCIT> cd: expected argument\n");
        } else {
            if (chdir(arglist[1]) != 0) {
                perror("FCIT> cd");
            }
        }
        return 1; /* handled */
    }

    /* Built-in: exit */
    if (strcmp(arglist[0], "exit") == 0) {
        /* clean exit of the shell */
        exit(0);
    }

    /* Built-in: help */
    if (strcmp(arglist[0], "help") == 0) {
        printf("FCIT> Built-in commands:\n");
        printf("  cd <dir>    - change current directory\n");
        printf("  exit        - exit the shell\n");
        printf("  help        - show this help message\n");
        printf("  jobs        - list background jobs (not yet implemented)\n");
        return 1;
    }

    /* Built-in: jobs (placeholder) */
    if (strcmp(arglist[0], "jobs") == 0) {
        printf("FCIT> Job control not yet implemented.\n");
        return 1;
    }

    /* Not a built-in: run external command */
    pid_t cpid = fork();
    if (cpid < 0) {
        perror("FCIT> fork failed");
        return -1;
    }
    if (cpid == 0) {
        /* child */
        execvp(arglist[0], arglist);
        /* execvp returns only on error */
        perror("FCIT> exec failed");
        exit(1);
    } else {
        /* parent waits for child to finish */
        int status;
        if (waitpid(cpid, &status, 0) == -1) {
            perror("FCIT> waitpid failed");
            return -1;
        }
        return 0;
    }
}
