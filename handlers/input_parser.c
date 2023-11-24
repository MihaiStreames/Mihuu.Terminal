#include "../common.h"

char* execute_subcommand(const char* subcmd) {
    FILE* pipe = popen(subcmd, "r");

    if (!pipe) {
        fprintf(stderr, "Failed to execute subcommand\n");
        return NULL;
    }

    char* result = NULL;
    size_t size = 0;
    size_t used = 0;
    size_t nread;

    while (1) {
        if (used + 512 > size) {
            size += 512;
            result = realloc(result, size);
            if (!result) {
                fprintf(stderr, "Failed to allocate memory\n");
                return NULL;
            }
        }

        nread = fread(result + used, 1, 512, pipe);
        used += nread;

        if (nread < 512) break;
    }

    pclose(pipe);
    result = realloc(result, used + 1); // Resize to actual used size + 1 for null terminator
    if (!result) {
        fprintf(stderr, "Failed to reallocate memory\n");
        return NULL;
    }
    result[used] = '\0';
    return result;
}

char* expand_environment_variables(const char* input) {
    size_t len = strlen(input);
    char* result = malloc(len + 1);
    size_t result_len = 0;

    for (size_t i = 0; i < len; ++i) {
        if (input[i] == '$') {
            char* end = input + i + 1;

            while (isalnum(*end) || *end == '_') end++;

            char var_name[256] = {0};
            strncpy(var_name, input + i + 1, end - (input + i + 1));

            char* var_value = getenv(var_name);

            if (var_value) {
                size_t var_value_len = strlen(var_value);
                result = realloc(result, result_len + var_value_len + (len - i));
                strcpy(result + result_len, var_value);
                result_len += var_value_len;
                i = end - input - 1;
            } else {
                result[result_len++] = input[i];
            }
        } else {
            result[result_len++] = input[i];
        }
    }

    result[result_len] = '\0';
    return result;
}

void remove_comments(char* input) {
    char* comment_start = strchr(input, '#');
    if (comment_start) {
        *comment_start = '\0';
    }
}

word_t* parser(char* input) {
    remove_comments(input);

    char* expanded_input = expand_environment_variables(input);

    char* token = strtok(expanded_input, " \t\n");
    word_t *head = NULL, *current = NULL;

    while (token) {
        word_t* new_word = malloc(sizeof(word_t));
        if (new_word == NULL) {
            fprintf(stderr, "Failed to allocate memory for new_word\n");
            break;
        }

        new_word->word = strdup(token);
        new_word->next = NULL;

        if (!head) {
            head = new_word;
        } else {
            current->next = new_word;
        }
        current = new_word;

        token = strtok(NULL, " \t\n");
    }

    return head;
}