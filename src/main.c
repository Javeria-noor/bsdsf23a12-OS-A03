#include "shell.h"

int main() {
    char* cmdline;
    char** arglist;
    char* expanded_cmd = NULL;

    while ((cmdline = read_cmd(PROMPT, stdin)) != NULL) {

        // Handle history expansion (!n or !!) before adding to history
        if (is_history_command(cmdline)) {
            expanded_cmd = expand_history_command(cmdline);
            if (expanded_cmd != NULL) {
                free(cmdline);
                cmdline = malloc(strlen(expanded_cmd) + 1);
                if (!cmdline) {
                    perror("malloc failed");
                    continue;
                }
                strcpy(cmdline, expanded_cmd);
                printf("%s\n", cmdline);  // Show the expanded command
            } else {
                free(cmdline);
                continue;  // Skip if history expansion failed
            }
        }

        // Add non-empty commands to history
        if (cmdline[0] != '\0' && cmdline[0] != '\n') {
            add_to_history(cmdline);
        }

        // Tokenize the command
        if ((arglist = tokenize(cmdline)) != NULL) {

            // Check if it's a built-in command first
            if (handle_builtin(arglist) == 0) {
                // If not a built-in, execute as external command
                execute(arglist);
            }

            // Free tokenized memory
            for (int i = 0; arglist[i] != NULL; i++) {
                free(arglist[i]);
            }
            free(arglist);
        }

        // Free the original command line
        free(cmdline);
    }

    printf("\nShell exited.\n");
    return 0;
}
