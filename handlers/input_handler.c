#include "../common.h"
#include "../builtins/builtins.h"

void free_word_list(word_t* head) {
    while (head) {
        word_t* temp = head;
        head = head->next;
        free(temp);
    }
}

void handle_redirection(char* command, char* filename) {
    //printf("Redirection: Command: '%s', Filename: '%s'\n", command, filename);
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1) {
        fprintf(stderr, "Failed to open file '%s' for redirection\n", filename);
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        dup2(fd, STDOUT_FILENO);
        close(fd);

        execlp("sh", "sh", "-c", command, NULL);
        fprintf(stderr, "Failed to execute command: '%s'\n", command);
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        close(fd);
        wait(NULL);
    } else {
        fprintf(stderr, "Failed to fork for redirection\n");
    }
}

void handle_input(word_t* parsed_input) {
    if (parsed_input == NULL) return;

    char* command = parsed_input->word;
    word_t* arg_node = parsed_input->next;

    if (strcmp(command, "cd") == 0) {
        char* path = (arg_node != NULL) ? arg_node->word : NULL;
        builtin_cd(path);
        return;
    }

    char full_command[1024] = "";
    char* filename = NULL;
    int redirect = 0;

    while (parsed_input != NULL) {
        if (strcmp(parsed_input->word, ">") == 0) {
            redirect = 1;
            parsed_input = parsed_input->next;

            if (parsed_input != NULL) {
                filename = parsed_input->word;
            }
            break;
        } else {
            if (strlen(full_command) > 0) {
                strcat(full_command, " ");
            }
            strcat(full_command, parsed_input->word);
        }
        parsed_input = parsed_input->next;
    }

    if (redirect && filename != NULL) {
        //printf("Redirecting output to file: '%s'\n", filename);
        handle_redirection(full_command, filename);
    } else {
        char *args[1024];
        args[0] = strtok(full_command, " ");
        int i = 1;

        while ((args[i] = strtok(NULL, " ")) != NULL) {
            i++;
        }

        pid_t pid = fork();

        if (pid == 0) {         // Child
            execvp(args[0], args);
            fprintf(stderr, "Failed to execute command: %s\n", args[0]);
            exit(EXIT_FAILURE);
        } else if (pid > 0) {   // Parent
            wait(NULL);
        } else {
            fprintf(stderr, "Failed to fork for input handling\n");
        }
    }
}