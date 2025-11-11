#include "shell.h"

int main() {
    char* cmdline;
    char* expanded_cmd = NULL;
    pipeline_t pipeline;

    // Initialize variables
    init_variables();
    
    // Initialize Readline if available
    initialize_readline();

    while (1) {
        // Use readline if available, otherwise fallback
        cmdline = read_cmd_readline(PROMPT);
        
        if (cmdline == NULL) {
            break; // EOF (Ctrl+D)
        }

        // Feature 8: Check for variable assignment first
        if (is_variable_assignment(cmdline)) {
            handle_variable_assignment(cmdline);
            free(cmdline);
            continue;
        }

        // Feature 8: Expand variables in command line
        char* expanded_with_vars = expand_variables(cmdline);
        if (expanded_with_vars != NULL) {
            free(cmdline);
            cmdline = expanded_with_vars;
        }

        // Feature 7: Check for if statement
        if (parse_if_statement(cmdline)) {
            // Simple if statement handling
            if (strstr(cmdline, "if") != NULL) {
                char* if_pos = strstr(cmdline, "if");
                char* then_pos = strstr(cmdline, "then");
                char* else_pos = strstr(cmdline, "else");
                char* fi_pos = strstr(cmdline, "fi");
                
                if (if_pos && then_pos && fi_pos) {
                    // Extract condition command (between if and then)
                    char condition[256] = {0};
                    strncpy(condition, if_pos + 3, then_pos - if_pos - 3);
                    
                    char then_cmd[256] = {0};
                    char else_cmd[256] = {0};
                    
                    if (else_pos) {
                        // Has else block
                        strncpy(then_cmd, then_pos + 4, else_pos - then_pos - 4);
                        strncpy(else_cmd, else_pos + 4, fi_pos - else_pos - 4);
                    } else {
                        // No else block
                        strncpy(then_cmd, then_pos + 4, fi_pos - then_pos - 4);
                    }
                    
                    execute_if_statement(condition, then_cmd, else_cmd);
                    free(cmdline);
                    continue;
                }
            }
        }

        // Handle command chaining (;)
        char* saveptr;
        char* command_token = strtok_r(cmdline, ";", &saveptr);
        
        while (command_token != NULL) {
            char* current_cmd = strdup(command_token);
            
            // Handle history expansion for this command
            if (is_history_command(current_cmd)) {
                expanded_cmd = expand_history_command(current_cmd);
                if (expanded_cmd != NULL) {
                    free(current_cmd);
                    current_cmd = strdup(expanded_cmd);
                    printf("%s\n", current_cmd);
                } else {
                    free(current_cmd);
                    command_token = strtok_r(NULL, ";", &saveptr);
                    continue;
                }
            }
            
            // Add non-empty commands to our internal history
            if (current_cmd[0] != '\0' && current_cmd[0] != '\n') {
                add_to_history(current_cmd);
            }

            // Parse for redirection and pipes
            if (parse_redirection_pipes(current_cmd, &pipeline) > 0) {
                // Execute the parsed command(s)
                execute_pipeline(&pipeline);
                
                // Free allocated memory
                free_pipeline(&pipeline);
            } else {
                // Fallback to old method for simple commands
                char** arglist = tokenize(current_cmd);
                if (arglist != NULL) {
                    if (handle_builtin(arglist) == 0) {
                        execute(arglist);
                    }
                    // Free the memory allocated by tokenize()
                    for (int i = 0; arglist[i] != NULL; i++) {
                        free(arglist[i]);
                    }
                    free(arglist);
                }
            }
            
            free(current_cmd);
            command_token = strtok_r(NULL, ";", &saveptr);
        }
        
        free(cmdline);
    }

    printf("\nShell exited.\n");
    return 0;
}
