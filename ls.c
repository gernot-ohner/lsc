#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include "ls.h"

int main(int argc, const char* argv[]) {

    if (argc > 2) printf("Usage: ls [optional: path/to/dir]");

    if (argc == 1) {
        char pwd[100];
        getcwd(pwd, 100);
        ls(pwd);
    }
    if (argc == 2) {
        ls(argv[1]);
    }
}

int ls(const char* path) {
    // region declarations & initializations
    DIR* dir;
    struct dirent* de;
    dir = opendir(path);
    // endregion

    int num_of_entries = count_entries(dir);

    struct dirent entries[num_of_entries];
    get_entries(dir, entries);
    rewinddir(dir);

    qsort(entries, num_of_entries, sizeof(struct dirent), compare);

    char entry_strings[num_of_entries][100];
    for (int j = 0; j < num_of_entries; j++)
        entry_to_string(&entries[j], path, entry_strings[j]);


    for (int j = 0; j < num_of_entries; j++)
        printf("%s\n", entry_strings[j]);

    // region finally
    (void) closedir(dir);

    return 0;
    // endregion
}

int compare(const void* d1, const void* d2) {
    struct dirent* m1 = (struct dirent*) d1;
    struct dirent* m2 = (struct dirent*) d2;
    return strcmp(m1->d_name, m2->d_name);
}

int count_entries(DIR* dir) {
    if (dir == NULL) return 0;

    int num_of_entries = 0;

    while (readdir(dir)) {
        num_of_entries++;
    }

    rewinddir(dir);
    return num_of_entries;
}

int get_entries(DIR* dir, OUT struct dirent entries[]) {
    if (dir == NULL) return -1;

    struct dirent* de;
    int i = 0;
    while ((de = readdir(dir))) {
        entries[i++] = *de;
    }

    return 0;
}

int entry_to_string(struct dirent* entry, const char* path, OUT char* result) {

    if (entry == NULL) return -1;

    char fullpath[MAX_LEN];
    struct stat stats;

    snprintf(fullpath, MAX_LEN, "%s/%s", path, entry->d_name);

    stat(fullpath, &stats);

    char* type;
    switch (entry->d_type) {
        case DT_REG: type = "F";
            break;
        case DT_DIR: type = "D";
            break;
        case DT_SOCK: type = "S";
            break;
        default: type = "*";
    }
    char* date_string = ctime(&stats.st_ctim.tv_sec);
    date_string[strlen(date_string) - 1] = ' ';

    snprintf(result, 100, "[%s] %s %s  %s %8ld  %s",
             type,
             getpwuid(stats.st_uid)->pw_name,
             getgrgid(stats.st_gid)->gr_name,
             date_string,
             stats.st_size,
             entry->d_name
    );

    return 0;
}
