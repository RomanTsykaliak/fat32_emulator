// fat32_emulator.c
/*
Copyright 2024 Roman Tsykaliak
  This program is free software: you can
redistribute it and/or modify it under the terms
of the GNU General Public License as published
by the Free Software Foundation, either version
3 of the License, or (at your option) any later
version.
  This program is distributed in the hope that
it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the GNU General Public License for
more details.
  You should have received a copy of the GNU
General Public License along with this program.
If not, see <https://www.gnu.org/licenses/>.
*/
#include <stdio.h> // file manipulation
#include <string.h>
#include <unistd.h> // system calls
#include <fcntl.h>
#define FILE_SIZE_IN_MB 20 // 20MB
#define MAX_INPUT_LENGTH 8 // length+2
#define NUM_ELEM(x) (sizeof(x) / sizeof((x)[0]))
int createFileOfSize(const char *path,
                     off_t size) {
  // For a POSIX compliant operating system
  int fd;
  // Open a file for reading/writing
  fd= open(path, O_WRONLY | O_CREAT,
           S_IRUSR | S_IWUSR);
  if(fd == -1) {
    perror("Error opening file.");
    return -1;
  }
  // Truncate the file to the specified size
  if(ftruncate(fd, size) == -1) {
    perror("Error setting file size.");
    close(fd);
    return -1;
  }
  close(fd); // Close the file
  return 1;
}
// Function declarations
void cd()     {printf("You entered cd\n");}
void format() {printf("You entered format\n");}
void ls()     {printf("You entered ls\n");}
void mkdir()  {printf("You entered mkdir\n");}
void touch()  {printf("You entered touch\n");}

int main(int argc, char *argv[]) {
  if(argc < 2) {
    printf("Please provide a file path.\n");
    return 1;
  }
  // Check if the file does not exist
  if(access(argv[1], F_OK) == -1) {
    printf("The file '%s' does not exist.\n",
           argv[1]);
    off_t size= FILE_SIZE_IN_MB * 1024 * 1024;
    printf("Creating a file of size %ld bytes "
           "...\n", size);
    if(createFileOfSize(argv[1], size) == -1)
      return 1;
    printf("Successfully created the file of "
           "size %ld bytes.\n", size);
  }
  char input[MAX_INPUT_LENGTH];
  while(1) {
    printf("promt>"); // Print the prompt
    // Read user input
    if(fgets(input, MAX_INPUT_LENGTH, stdin) ==
                                         NULL) {
      perror("Error reading input.\n");
      break;
    }
    // Remove the newline character from the end
    // (if present)
    input[strcspn(input, "\n")]= '\0';
    // List of commands
    const char *commands[5];
    commands[0]= "cd";
    commands[1]= "format";
    commands[2]= "ls";
    commands[3]= "mkdir";
    commands[4]= "touch";
    // Array of function pointers initialization
    void(*function_pointers[5])()=
                 {cd, format, ls, mkdir, touch};
    // Check commands
    unsigned i;
    for(i= 0; i < NUM_ELEM(commands); ++i) {
      if(strcmp(input, commands[i]) == 0) {
        function_pointers[i]();
        break; // found command
    } }
    if(i == NUM_ELEM(commands)) { // not found
      printf("Unknown command.  Available "
             "commands are ");
      for(i= 0; i < NUM_ELEM(commands); ++i)
        printf("%s, ", commands[i]);
      puts(""); // printf("\n");
    }
  }
  printf("Exiting program.\n");
  return 0;
}
