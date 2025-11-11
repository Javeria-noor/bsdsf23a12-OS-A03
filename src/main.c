#include "shell.h"

int main() {
    char* cmdline;
    char* expanded_cmd = NULL;
    pipeline_t pipeline;

    // Initialize Readline if available
    initialize_readline();

    while (1) {
        // Use readline if available, otherwise fallback
        cmdline = read_cmd_readline(PROMPT);
        
        if (cmdline == NULL) {
            break; // EOF (Ctrl+D)
        }

        // Handle command chaining (;) - NEW FEATURE 6
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
                    printf("%s\n", current_cmd);  // Show the expanded command
                } else {
                    free(current_cmd);
                    command_token = strtok_r(NULL, ";", &saveptr);
                    continue;  // Skip to next command if history expansion failed
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
