/* https://codingchallenges.fyi/challenges/challenge-wc/ 
 * https://www.geeksforgeeks.org/input-output-system-calls-c-create-open-close-read-write/
 *
 * 1. Be able to create files
 * 2. Open and read files
 * 3. Open and write in files
 * 4. Complete the step 1 of the coding challenge
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int create_and_open_file(char *filename) {
    char filepath[50];
    sprintf(filepath, "./%s", filename);

    printf("Current path: %s\n", filepath);
    return open(filepath, O_CREAT | O_RDWR);
}

int main(int argc, char *argv[])
{
    printf("Arguments:\n");
    for (int i = 1; i <= argc; i++) {
        printf("Argument %d: %s\n", i, *(argv + i - 1));
    }

    int fd = create_and_open_file("here.txt");
    if (fd < 0) {
        perror("Failed to create a file.");
        exit(1);
    }

    printf("File descriptor: %d\n", fd);


    return 0;
}
