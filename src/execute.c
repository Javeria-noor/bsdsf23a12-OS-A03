#include "shell.h"

/* execute external command (if not builtin) */
int execute(char** arglist) {
    if (arglist == NULL || arglist[0] == NULL) return 0;

    /* if built-in, handle and return */
    if (handle_builtin(arglist)) return 0;

    pid_t cpid = fork();
    if (cpid < 0) {
        perror("fork");
        return 1;
    } else if (cpid == 0) {
        /* child */
        execvp(arglist[0], arglist);
        /* execvp returns only on error */
        perror("execvp");
        exit(1);
    } else {
        /* parent: wait for child */
        int status;
        if (waitpid(cpid, &status, 0) == -1) {
            perror("waitpid");
            return 1;
        }
        return WIFEXITED(status) ? WEXITSTATUS(status) : 0;
    }
}
