#include "shell.h"

/* read a line from fp, return malloc'd string (caller frees) */
char* read_cmd(char* prompt, FILE* fp) {
    if (prompt) printf("%s", prompt);
    char *buf = malloc(MAX_LEN);
    if (!buf) return NULL;

    if (fgets(buf, MAX_LEN, fp) == NULL) {
        free(buf);
        return NULL; /* EOF (Ctrl+D) */
    }
    /* strip newline */
    size_t L = strlen(buf);
    if (L > 0 && buf[L-1] == '\n') buf[L-1] = '\0';
    return buf;
}

/* simple whitespace tokenizer, returns malloc'd array (caller frees tokens and array) */
char** tokenize(char* cmdline) {
    if (cmdline == NULL) return NULL;
    /* quick check: empty/only spaces */
    char *p = cmdline;
    while (*p == ' ' || *p == '\t') p++;
    if (*p == '\0') return NULL;

    char** args = malloc((MAXARGS+1) * sizeof(char*));
    if (!args) return NULL;
    int argc = 0;
    char *tok = strtok(cmdline, " \t");
    while (tok != NULL && argc < MAXARGS) {
        args[argc] = malloc(strlen(tok)+1);
        strcpy(args[argc], tok);
        argc++;
        tok = strtok(NULL, " \t");
    }
    args[argc] = NULL;
    return args;
}
