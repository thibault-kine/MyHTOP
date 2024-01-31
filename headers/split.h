#ifndef SPLIT_H
#define SPLIT_H

int is_space(char c);

int count_letters(char* str, int i);

int count_words(char *str);

char** split(char* str);

#endif