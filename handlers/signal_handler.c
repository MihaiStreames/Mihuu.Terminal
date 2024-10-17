#include "../common.h"

void signal_handler(int sig) {
    if (sig == SIGINT) {
        printf("\nCaught signal SIGINT, cleaning up\n");
        exit(0);
    }
}