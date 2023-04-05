#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcre.h>

void process_log(FILE *log, const char *search_regex, const char *group_separator, int case_insensitive) {
    pcre *re, *search_re;
    const char *error;
    int erroffset;
    int ovector[30];
    char line[1024];
    int rc;

    re = pcre_compile(group_separator, 0, &error, &erroffset, NULL);
    if (!re) {
        printf("Group separator regex compilation failed at offset %d: %s\n", erroffset, error);
        return;
    }

    int search_options = case_insensitive ? PCRE_CASELESS : 0;
    search_re = pcre_compile(search_regex, search_options, &error, &erroffset, NULL);
    if (!search_re) {
        printf("Search regex compilation failed at offset %d: %s\n", erroffset, error);
        return;
    }

    char group[65536] = {0};
    int group_started = 0;

    while (fgets(line, sizeof(line), log)) {
        if ((rc = pcre_exec(re, NULL, line, (int)strlen(line), 0, 0, ovector, 30)) >= 0) {
            if (group_started && pcre_exec(search_re, NULL, group, (int)strlen(group), 0, 0, ovector, 30) >= 0) {
                printf("%s", group);
            }
            group_started = 1;
            strcpy(group, line);
        } else if (group_started) {
            strcat(group, line);
        }
    }

    if (group_started && pcre_exec(search_re, NULL, group, (int)strlen(group), 0, 0, ovector, 30) >= 0) {
        printf("%s", group);
    }

    pcre_free(re);
    pcre_free(search_re);
}

int main(int argc, char **argv) {
    if (argc < 3 || argc > 5) {
        printf("Usage: %s [-i] <search_regex> <log_file> [group_separator_regex]\n", argv[0]);
        return 1;
    }

    int case_insensitive = 0;
    int arg_offset = 0;

    if (strcmp(argv[1], "-i") == 0) {
        case_insensitive = 1;
        arg_offset = 1;
    }

    const char *search_regex = argv[1 + arg_offset];
    const char *log_file_path = argv[2 + arg_offset];
    const char *group_separator = (argc == 4 + arg_offset) ? argv[3 + arg_offset] : "^(\\d{4}-\\d{2}-\\d{2}T\\d{2}\\:\\d{2}\\:\\d{2}\\.\\d{6}\\+\\d{4},)";

    FILE *log_file = fopen(log_file_path, "r");
    if (!log_file) {
        printf("Error opening log file: %s\n", log_file_path);
        return 1;
    }

    process_log(log_file, search_regex, group_separator, case_insensitive);
    fclose(log_file);

    return 0;
}
