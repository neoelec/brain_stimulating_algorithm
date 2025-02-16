#include <stdio.h>
#include <string.h>

#include <rcn/karprabn.h>

#define MAX_BUFFER 512

int main(int argc, char *argv[])
{
    char *file_name;
    FILE *fp;
    char text[MAX_BUFFER];
    char *pattern;
    size_t pattern_len;
    size_t line = 0;

    if (argc < 3) {
        printf("Usage: %s <file_name> <pattern>\n", argv[0]);
        return 1;
    }

    file_name = argv[1];
    pattern = argv[2];

    pattern_len = strlen(pattern);

    if ((fp = fopen(file_name, "r")) == NULL) {
        printf("Cannot open file: %s\n", file_name);
        return 1;
    }

    while (fgets(text, MAX_BUFFER, fp) != NULL) {
        ssize_t pos = karprabn(text, strlen(text), 0, pattern, pattern_len);

        line++;

        if (pos >= 0)
            printf("line: %zu, column: %zd : %s", line, pos + 1, text);
    }

    fclose(fp);

    return 0;
}
