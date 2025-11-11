#include "shell.h"

// Parse if-then-else-fi statement
int parse_if_statement(char* cmdline) {
    // Simple check for if statement
    if (strstr(cmdline, "if") != NULL && 
        (strstr(cmdline, "then") != NULL || strstr(cmdline, "fi") != NULL)) {
        return 1;
    }
    return 0;
}

// Execute if-then-else statement
int execute_if_statement(char* condition_cmd, char* then_cmd, char* else_cmd) {
    int condition_status;
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process - execute condition command
        char* args[MAXARGS];
        char* saveptr;
        int i = 0;
        char* token = strtok_r(condition_cmd, " \t\n", &saveptr);
        
        while (token != NULL && i < MAXARGS - 1) {
            args[i++] = token;
            token = strtok_r(NULL, " \t\n", &saveptr);
        }
        args[i] = NULL;
        
        if (args[0] != NULL) {
            execvp(args[0], args);
            perror("execvp");
        }
        exit(1);
    } else if (pid > 0) {
        // Parent process - wait for condition
        waitpid(pid, &condition_status, 0);
        
        if (WIFEXITED(condition_status)) {
            int exit_code = WEXITSTATUS(condition_status);
            
            if (exit_code == 0) {
                // Condition succeeded - execute then block
                printf("Then: %s\n", then_cmd);
                
                // Simple execution for then command
                if (then_cmd != NULL && then_cmd[0] != '\0') {
                    char* then_args[MAXARGS];
                    char* saveptr2;
                    int j = 0;
                    char* token2 = strtok_r(then_cmd, " \t\n", &saveptr2);
                    
                    while (token2 != NULL && j < MAXARGS - 1) {
                        then_args[j++] = token2;
                        token2 = strtok_r(NULL, " \t\n", &saveptr2);
                    }
                    then_args[j] = NULL;
                    
                    if (then_args[0] != NULL) {
                        if (handle_builtin(then_args)) {
                            return 0;
                        }
                        return execute(then_args);
                    }
                }
            } else {
                // Condition failed - execute else block
                if (else_cmd != NULL && else_cmd[0] != '\0') {
                    printf("Else: %s\n", else_cmd);
                    
                    char* else_args[MAXARGS];
                    char* saveptr3;
                    int k = 0;
                    char* token3 = strtok_r(else_cmd, " \t\n", &saveptr3);
                    
                    while (token3 != NULL && k < MAXARGS - 1) {
                        else_args[k++] = token3;
                        token3 = strtok_r(NULL, " \t\n", &saveptr3);
                    }
                    else_args[k] = NULL;
                    
                    if (else_args[0] != NULL) {
                        if (handle_builtin(else_args)) {
                            return 0;
                        }
                        return execute(else_args);
                    }
                }
            }
        }
        return 0;
    } else {
        perror("fork");
        return -1;
    }
}

