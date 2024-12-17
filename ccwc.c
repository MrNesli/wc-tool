/* https://codingchallenges.fyi/challenges/challenge-wc/ 
 * https://www.geeksforgeeks.org/input-output-system-calls-c-create-open-close-read-write/
 * https://cplusplus.com/reference/cstdio/fgets/
 *
 * 1. Be able to create files
 * 2. Open and read files
 * 3. Open and write in files
 * 4. Complete the step 1 of the coding challenge
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

typedef struct File {
    FILE *fptr;
    char *filename;
    char *buffer;
    long buffer_size;
} File;

long count_lines(File file);
void usage_error();
FILE* open_file(const char *filename, const char *mode);
long count_bytes(FILE *file);
char* get_file_contents(FILE *file);

void usage_error() {
    printf("Usage: ccwc -c filename.txt\n");
    printf("-c Number of bytes in a file\n");
    printf("-l Number of lines in a file\n");
    exit(1);
}

/*
* Approach:
*
* 1)
* We read characters up until a space.
* Once we found a space, we erase the string, and we increment the words counter (check also if the current string is not empty).
* 
* 2)
* We could split the buffer by spaces, and check if those values are not spaces, and increment words counter
*
*/
long count_words(File file) {
    long words = 0;
    char *word = "";
    for (int i = 0; i < file.buffer_size; i++) {
        if (file.buffer[i] == '\n') {

        }
    }

    return lines;
}

long count_lines(File file) {
    long lines = 0;
    for (int i = 0; i < file.buffer_size; i++) {
        if (file.buffer[i] == '\n') {
            lines++;
        }
    }

    return lines;
}

void file_status(const File file) {
    printf("File <%s>:\n", file.filename);

    if (file.fptr != NULL) {
        printf("  Status: Open.\n");
    }
    else {
        printf("  Status: NULL.\n");
    }

    printf("  File size: %ld\n", file.buffer_size);
    printf("  File buffer: %s\n", file.buffer);
}

FILE* open_file(const char *filename, const char *mode) {
    FILE *fptr = fopen(filename, mode);

    if (fptr == NULL) {
        perror("Error opening the file\n");
        exit(1);
    }

    return fptr;
}

File read_file(const char *filename) {
    FILE *fptr = open_file(filename, "r");
    long buffer_size = count_bytes(fptr);
    char *buffer = get_file_contents(fptr);

    File opened_file = (File) {
        .fptr = fptr,
        .filename = (char *) filename,
        .buffer = buffer,
        .buffer_size = buffer_size,
    };

    // file_status(opened_file);

    return opened_file;
}

long count_bytes(FILE *file) {
    long bytes = 0; // 1 character = 1 byte

    // Moving the cursor at the end of the line
    fseek(file, 0, SEEK_END); 

    // Number of bytes before the start of the file
    bytes = ftell(file); 

    // We need to return to the beginning of the file to be able to read it
    rewind(file);

    return bytes;
}

char* get_file_contents(FILE *file) {
    long fsize = count_bytes(file);
    char *buffer = (char *) malloc(sizeof(char) * fsize);

    if (buffer == NULL) {
        perror("Failed to allocate memory.\n");
        exit(1);
    }

    fread(buffer, sizeof(char), fsize, file);

    return buffer;
}

int main(int argc, char *argv[])
{
    if (argc > 3) {
        printf("Error: Too many arguments...\n");
        usage_error();
    }

    if (argc != 3) {
        usage_error();
    }

    if (strcmp("-c", argv[1]) == 0) {
        char *filename = argv[2];
        File file = read_file(filename);

        printf("%ld %s\n", file.buffer_size, filename);
    }
    else if (strcmp("-l", argv[1]) == 0) {
        char *filename = argv[2];
        File file = read_file(filename);

        printf("%ld %s\n", count_lines(file), filename);
    }
    else {
        usage_error();
    }

    return 0;
}
