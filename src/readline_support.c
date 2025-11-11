#include "shell.h"

#if !USE_READLINE
// Define dummy globals once
char* rl_readline_name = "";
int rl_completion_query_items = 100;
char** (*rl_attempted_completion_function)(const char*, int, int) = NULL;
#endif

// Minimal command completion generator
char* command_generator(const char* text, int state) {
    static int list_index, len;
    const char* name;
    static const char* commands[] = {
        "cd", "exit", "help", "history", "jobs",
        "ls", "pwd", "whoami", "echo", "cat", NULL
    };

    if (!state) {
        list_index = 0;
        len = strlen(text);
    }

    while ((name = commands[list_index]) != NULL) {
        list_index++;
        if (strncmp(name, text, len) == 0) {
            return strdup(name);
        }
    }
    return NULL;
}

char** custom_completion(const char* text, int start, int end) {
    (void)end;
    if (start == 0) {
        return rl_completion_matches(text, command_generator);
    }
    return rl_completion_matches(text, rl_filename_completion_function);
}

// Readline command reader
char* read_cmd_readline(const char* prompt) {
    char* line = readline(prompt);
    if (line && *line) {
        add_history(line);
    }
    return line;
}

// Initialize readline
void initialize_readline() {
    rl_readline_name = "myshell";
    rl_attempted_completion_function = custom_completion;
    rl_completion_query_items = 100;
}
