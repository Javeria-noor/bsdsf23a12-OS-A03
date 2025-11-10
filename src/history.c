#include "shell.h"

// Global history variables (circular buffer)
static char* history[HISTORY_SIZE];
static int history_count = 0;
static int history_start = 0;

// Add a command to history
void add_to_history(const char* cmd) {
    if (cmd == NULL || cmd[0] == '\0' || cmd[0] == '\n') return;

    // Skip if same as last command
    if (history_count > 0) {
        int last_index = (history_start + history_count - 1) % HISTORY_SIZE;
        if (strcmp(history[last_index], cmd) == 0) return;
    }

    // Allocate memory for new command
    char* cmd_copy = malloc(strlen(cmd) + 1);
    if (!cmd_copy) {
        perror("malloc failed");
        return;
    }
    strcpy(cmd_copy, cmd);

    // Add to circular buffer
    int index = (history_start + history_count) % HISTORY_SIZE;

    // If buffer is full, remove oldest command
    if (history_count == HISTORY_SIZE) {
        free(history[history_start]);
        history_start = (history_start + 1) % HISTORY_SIZE;
        history_count--;
    }

    history[index] = cmd_copy;
    history_count++;
}

// Print all history commands
void print_history() {
    for (int i = 0; i < history_count; i++) {
        int index = (history_start + i) % HISTORY_SIZE;
        printf("%d %s\n", i + 1, history[index]);
    }
}

// Get a specific history command by number
char* get_history_command(int n) {
    if (n < 1 || n > history_count) return NULL;
    int index = (history_start + n - 1) % HISTORY_SIZE;
    return history[index];
}

// Check if command starts with !
int is_history_command(const char* cmdline) {
    return (cmdline != NULL && cmdline[0] == '!');
}

// Expand !n or !! commands
char* expand_history_command(const char* cmdline) {
    if (!is_history_command(cmdline)) return NULL;

    // Handle !!
    if (cmdline[1] == '!' && cmdline[2] == '\0') {
        if (history_count == 0) {
            fprintf(stderr, "No previous command in history\n");
            return NULL;
        }
        return get_history_command(history_count);
    }

    // Handle !n
    if (cmdline[1] >= '0' && cmdline[1] <= '9') {
        char* endptr;
        int n = strtol(&cmdline[1], &endptr, 10);

        if (*endptr != '\0') {
            fprintf(stderr, "Invalid history number: %s\n", &cmdline[1]);
            return NULL;
        }

        char* hist_cmd = get_history_command(n);
        if (!hist_cmd) {
            fprintf(stderr, "No such history command: %d\n", n);
            return NULL;
        }

        return hist_cmd;
    }

    fprintf(stderr, "Invalid history syntax: %s\n", cmdline);
    fprintf(stderr, "Use !n for command number n, or !! for previous command\n");
    return NULL;
}

