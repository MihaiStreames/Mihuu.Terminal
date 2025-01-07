#include "common.h"

void run_tests() {
    char* test_commands[] = {
        "ls",
        "which echo",
        "echo Hello, World!",
        "echo $PATH",
        "touch testfile.txt",
        "echo 'Sample content' > testfile.txt",
        "cat testfile.txt",
        NULL
    };

    for (int i = 0; test_commands[i] != NULL; i++) {
        //printf("Testing command: %s\n", test_commands[i]);
        word_t* parsed_input = parser(test_commands[i]);

        if (parsed_input == NULL) {
            printf("Parser returned NULL for command: %s\n", test_commands[i]);
            continue;
        }

        handle_input(parsed_input);
        free_word_list(parsed_input);

    }

    printf("All tests passed, good for release! :)\n");
}

int main() {
    //run_tests(); // Uncomment if testing

    signal(SIGINT, signal_handler);

    char input[1024];
    char cwd[PATH_MAX];

    while (1) {
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            strcpy(cwd, "unknown");
        }

        printf("TerminalMihuu> [%s]: ~'%s'$ ", getenv("USER"), cwd);
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin)) {
            if (feof(stdin)) {
                break;
            }
            continue;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            break;
        }

        word_t* parsed_input = parser(input);
        handle_input(parsed_input);
        free_word_list(parsed_input);
    }

    printf("Exiting...\n");
    return 0;
}