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

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

#define MIN_STRING_CAPACITY 1000

// String representation
typedef struct String {
    char *content;
    size_t capacity;
    size_t full_capacity; // needed for null terminator
    size_t size;
} String;

// File representation
typedef struct File {
    FILE *fptr;
    String filename;
    String buffer;
} File;

// TODO: Create a function that prints a string with all the special characters

int get_words(String s);
int get_lines(File file);
void usage_error();
FILE* open_file(const char *filename, const char *mode);
char* get_file_contents(FILE *file);
long count_bytes(FILE *file);
String create_string(char *content);
String create_empty_string();
void append_char(String *s, char c);
void append(String *s, char *s_to_append);
bool is_empty(String s);

void usage_error() {
    printf("Usage: ccwc -c filename.txt\n");
    printf("-c Number of bytes in a file\n");
    printf("-l Number of lines in a file\n");
    exit(1);
}

bool is_empty(String s) {
    return s.size == 0;
}

String create_string(char *content) {
    /* Creates a string representation based of a C string */
    String s = (String) {
        .content = (char *) calloc(sizeof(char), (MIN_STRING_CAPACITY + 1)),
        .capacity = MIN_STRING_CAPACITY,
        .full_capacity = MIN_STRING_CAPACITY + 1,
        .size = 0,
    };

    append(&s, content);

    return s;
}

String create_empty_string() {
    /* Creates an empty string representation */
    // NOTE: I'm not sure if this creates '<backslash>0' string terminator by default. If it does, do we need the size to start from 1?
    String s = (String) {
        .content = (char *) calloc(sizeof(char), (MIN_STRING_CAPACITY + 1)),
        .capacity = MIN_STRING_CAPACITY, // + 1 for null terminator character
        .full_capacity = MIN_STRING_CAPACITY + 1,
        .size = 0,
    };

    return s;
}

void empty_string(String *s) {
    /* Empties the string representation */

    s->content = (char *) calloc(sizeof(char), (MIN_STRING_CAPACITY + 1));
    s->capacity = MIN_STRING_CAPACITY;
    s->full_capacity = MIN_STRING_CAPACITY + 1;
    s->size = 0;
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
int get_words(String s) {
    /* Parsing all the words out of the String */
    // TODO: Figure out why the word count is incorrect
    String current = create_empty_string();
    int words = 0;

    for (int i = 0; i < s.size; i++) {
        if (s.content[i] != ' ') {
            append_char(&current, s.content[i]);
        }
        else if (s.content[i] == ' ') {
            words++;
            empty_string(&current);
        }
    }

    return words;
}

int get_lines(File file) {
    // What if there is a single line that doesn't have new line character?

    int lines = 0;
    for (int i = 0; i < file.buffer.size; i++) {
        if (file.buffer.content[i] == '\n') {
            lines++;
        }
    }

    return (lines == 0 && file.buffer.size > 0) ? 1 : lines;
}

void append(String *s, char *s_to_append) {
    /* Appending the C string into String */
    
    // QUESTION: Do we care about new line characters in here?
    size_t append_length = strlen(s_to_append);

    while (append_length > (s->capacity - s->size)) {
        printf("[INFO] String to append length = %d; Current capacity = %d; Doubling capacity... Reallocating memory...\n", (int) append_length, (int) s->capacity);

        s->capacity *= 2;
        s->full_capacity = s->capacity + 1; // Actual capacity with a null terminator
        if ((s->content = (char *) realloc(s->content, sizeof(char) * s->full_capacity)) == NULL) {
            perror("Failed to allocate memory for the String");
        }
    }

    printf("[INFO] Capacity = %d\n", (int) s->capacity);

    // Concatenate two strings
    strcat(s->content, s_to_append);

    s->size += append_length;
}

void append_char(String *s, char c) {
    /* Basic character appending operation on a String */

    s->size++;

    if (s->size > s->capacity) {
        s->capacity *= 2;
        s->full_capacity = s->capacity + 1;
        if ((s->content = (char *) realloc(s->content, sizeof(char) * s->full_capacity)) == NULL) {
            perror("Failed to allocate memory for the String");
        } 
    }

    s->content[s->size - 1] = c;
}


void file_status(const File file) {
    printf("File <%s>:\n", file.filename.content);
    printf("----------------\n");

    if (file.fptr != NULL) {
        printf("Status: Open.\n");
    }
    else {
        printf("Status: NULL.\n");
    }

    printf("File size: %ld\n", file.buffer.size);
    // printf("File buffer: %s\n", file.buffer.content);
    printf("----------------\n");
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
    String buffer = create_string(get_file_contents(fptr));
    String sfilename = create_string((char *) filename);

    File opened_file = (File) {
        .fptr = fptr,
        .filename = sfilename,
        .buffer = buffer,
    };

    file_status(opened_file);

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
    else if (argc != 3) {
        usage_error();
    }

    char *filename = argv[2];
    File file = read_file(filename);

    if (strcmp("-c", argv[1]) == 0) {
        printf("%ld %s\n", file.buffer.size, filename);
    }
    else if (strcmp("-l", argv[1]) == 0) {
        printf("%d %s\n", (int) get_lines(file), filename);
    }
    else if (strcmp("-w", argv[1]) == 0) {
        printf("%d %s\n", (int) get_words(file.buffer), filename);
    }
    else {
        usage_error();
    }

    return 0;
}
