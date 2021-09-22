//
// Created by go on 21.09.21.
//

#ifndef LS_LS_H
#define LS_LS_H

#include <dirent.h>

#define OUT
#define MAX_LEN 100

int ls(const char* path);
int compare(const void* d1, const void* d2);
int count_entries(DIR* dir);
int get_entries(DIR* dir, OUT struct dirent entries[]);
int entry_to_string(struct dirent* entry, const char* pwd, OUT char* result);

#endif //LS_LS_H
