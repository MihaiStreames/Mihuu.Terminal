#ifndef TERMINALMIHUU_COMMON_H
#define TERMINALMIHUU_COMMON_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>

#include <sys/wait.h>
#include <sys/stat.h>

typedef struct word_t {
    char* word;
    struct word_t* next;
} word_t;

void free_word_list(word_t* head);
void handle_input(word_t* parsed_input);
void signal_handler(int sig);
word_t* parser(char* input);

#endif //TERMINALMIHUU_COMMON_H