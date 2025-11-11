#include "shell.h"

// Global variables array
static variable_t variables[MAX_VARS];
static int var_count = 0;

// Initialize variables
void init_variables() {
    var_count = 0;
    for (int i = 0; i < MAX_VARS; i++) {
        variables[i].name = NULL;
        variables[i].value = NULL;
    }
}

// Check if command is a variable assignment
int is_variable_assignment(char* cmdline) {
    if (cmdline == NULL) return 0;
    
    char* equals = strchr(cmdline, '=');
    if (equals == NULL) return 0;
    
    // Check if there are no spaces around the =
    char* before_equals = cmdline;
    while (before_equals < equals) {
        if (*before_equals == ' ' || *before_equals == '\t') {
            return 0;
        }
        before_equals++;
    }
    
    return 1;
}

// Handle variable assignment
void handle_variable_assignment(char* cmdline) {
    char* equals = strchr(cmdline, '=');
    if (equals == NULL) return;
    
    // Extract variable name and value
    char var_name[256] = {0};
    char var_value[256] = {0};
    
    strncpy(var_name, cmdline, equals - cmdline);
    strcpy(var_value, equals + 1);
    
    // Remove quotes if present
    if (var_value[0] == '"' && var_value[strlen(var_value)-1] == '"') {
        memmove(var_value, var_value + 1, strlen(var_value) - 2);
        var_value[strlen(var_value) - 2] = '\0';
    }
    
    // Check if variable already exists
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, var_name) == 0) {
            free(variables[i].value);
            variables[i].value = strdup(var_value);
            return;
        }
    }
    
    // Add new variable
    if (var_count < MAX_VARS) {
        variables[var_count].name = strdup(var_name);
        variables[var_count].value = strdup(var_value);
        var_count++;
    }
}

// Expand variables in command line
char* expand_variables(char* cmdline) {
    if (cmdline == NULL) return NULL;
    
    char result[MAX_LEN] = {0};
    char* current = cmdline;
    
    while (*current != '\0') {
        if (*current == '$') {
            // Found a variable
            char var_name[256] = {0};
            char* var_start = current + 1;
            char* var_end = var_start;
            
            // Extract variable name
            while ((*var_end >= 'a' && *var_end <= 'z') || 
                   (*var_end >= 'A' && *var_end <= 'Z') ||
                   (*var_end >= '0' && *var_end <= '9') ||
                   *var_end == '_') {
                var_end++;
            }
            
            strncpy(var_name, var_start, var_end - var_start);
            
            // Look up variable value
            int found = 0;
            for (int i = 0; i < var_count; i++) {
                if (strcmp(variables[i].name, var_name) == 0) {
                    strcat(result, variables[i].value);
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                // Variable not found, keep the original
                strncat(result, current, var_end - current);
            }
            
            current = var_end;
        } else {
            // Regular character
            strncat(result, current, 1);
            current++;
        }
    }
    
    return strdup(result);
}

// Print all variables
void print_variables() {
    printf("Shell Variables:\n");
    for (int i = 0; i < var_count; i++) {
        printf("  %s=%s\n", variables[i].name, variables[i].value);
    }
    if (var_count == 0) {
        printf("  No variables defined\n");
    }
}

