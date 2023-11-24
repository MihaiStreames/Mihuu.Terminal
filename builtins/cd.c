#include "../common.h"

void builtin_cd(char* path) {
    if (path == NULL) {
        path = getenv("HOME");
    }
    if (chdir(path) != 0) {
        perror("cd");
    }
}