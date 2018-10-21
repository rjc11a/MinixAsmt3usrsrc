#include <stdio.h>
#include <stdlib.h>

#define CHUNK 1024 /* read 1024 bytes at a time */
char buf[CHUNK];
FILE *file;
size_t nread;

int main()
{
    file = fopen("big.txt", "r");
    if (file) {
        while ((nread = fread(buf, 1, sizeof buf, file)) > 0)
            fwrite(buf, 1, nread, stdout);
        if (ferror(file)) {
            /* deal with error */
        }
        fclose(file);
    }
}