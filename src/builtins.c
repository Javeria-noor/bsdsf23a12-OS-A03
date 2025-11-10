#include "shell.h"

// Built-in command: exit
int builtin_exit(char** arglist) {
    printf("Shell terminated.\n");
    exit(0);
}

// Built-in command: cd
int builtin_cd(char** arglist) {
    char* dir = arglist[1] ? arglist[1] : getenv("HOME");
    if (!dir) {
        fprintf(stderr, "cd: HOME environment variable not set\n");
        return 1;
    }
    if (chdir(dir) != 0) {
        perror("cd");
        return 1;
    }
    return 0;
}

// Built-in command: help
int builtin_help(char** arglist) {
    printf("Built-in commands:\n");
    printf("  cd <directory>    - Change current working directory\n");
    printf("  exit              - Terminate the shell\n");
    printf("  help              - Display this help message\n");
    printf("  history           - Display command history\n");
    printf("  jobs              - Display background jobs (not yet implemented)\n");
    return 0;
}

// Built-in command: jobs (placeholder)
int builtin_jobs(char** arglist) {
    printf("Job control not yet implemented.\n");
    return 0;
}

// Built-in command: history
int builtin_history(char** arglist) {
    print_history();
    return 0;
}

// Main built-in command handler
int handle_builtin(char** arglist) {
    if (!arglist[0]) return 0;

    if (strcmp(arglist[0], "exit") == 0) {
        builtin_exit(arglist);
        return 1;
    } else if (strcmp(arglist[0], "cd") == 0) {
        builtin_cd(arglist);
        return 1;
    } else if (strcmp(arglist[0], "help") == 0) {
        builtin_help(arglist);
        return 1;
    } else if (strcmp(arglist[0], "jobs") == 0) {
        builtin_jobs(arglist);
        return 1;
    } else if (strcmp(arglist[0], "history") == 0) {
        builtin_history(arglist);
        return 1;
    }

    return 0; // Not a built-in command
}

