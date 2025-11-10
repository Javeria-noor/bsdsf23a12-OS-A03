#include "shell.h"

int main() {
    char* cmdline;
    char** arglist;

    while ((cmdline = read_cmd(PROMPT, stdin)) != NULL) {
        if ((arglist = tokenize(cmdline)) != NULL) {

            // Check if it's a built-in command first
            if (!handle_builtin(arglist)) {
                // If not a built-in, execute as external command
                execute(arglist);
            }

            // Free memory allocated by tokenize()
            for (int i = 0; arglist[i] != NULL; i++) {
                free(arglist[i]);
            }
            free(arglist);
        }

        free(cmdline);
    }

    printf("\nShell exited.\n");
    return 0;
}
