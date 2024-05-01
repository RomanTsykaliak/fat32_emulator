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
#include <stdio.h>  // file manipulation
#include <string.h>
// system calls, chdir for cd
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>  // isspace
#define FILE_SIZE_IN_MB 20 // 20MB
#define MAX_INPUT_LENGTH 80 // length+2
#define NUM_ELEM(x) (sizeof(x) / sizeof((x)[0]))
// Suppress unused parameter warnings
#define UNUSED(x) (void)(x)
char PROMPT[MAX_INPUT_LENGTH]= "/"; // evil
int createFileOfSize(const char *path,
                     off_t size) {
  // For a POSIX compliant operating system
  int fd= open(path, O_WRONLY | O_CREAT,
           // Open a file for reading/writing
           S_IRUSR | S_IWUSR);
  if(fd == -1) {
    perror("Error opening file.");
    return -1;
  }
  // Truncate the file to the specified size
  if(ftruncate(fd, size) == -1) {
    perror("Error setting file size.");
    close(fd); // Close the file
    return -1; // failure
  }
  close(fd); // Close the file
  return 1; // SUCCESS
}
// Function declarations for prompt
void cd(const char *path) {
  // Only absolute path is allowed.
  if(path == NULL) {
    printf("You entered cd without path\n");
    strncpy(PROMPT,  "/", MAX_INPUT_LENGTH-1);
  } else {
    printf("You entered cd %s\n", path);
    strncpy(PROMPT, path, MAX_INPUT_LENGTH-1);
} }
void format(const char* na) {
  // keep functions' signature the same
  UNUSED(na);
  printf("You entered format\n");
}
void ls(const char *path) {
  if(path == NULL)
    printf("You entered ls without path\n");
  else
    printf("You entered ls %s\n", path);
}
void mkdir(const char *name) {
  if(name == NULL)
    printf("Please enter name\n");
  else
    printf("You entered mkdir %s\n", name);
}
void touch(const char *name) {
  if(name == NULL)
    printf("Please enter name\n");
  else
    printf("You entered touch %s\n", name);
}

int main(int argc, char *argv[]) {
  if(argc < 2) {
    printf("Please provide a file path.\n");
    return 1;
  }
  // Check if the file does not exist; create it
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
  // PROMPT
  char input[MAX_INPUT_LENGTH];
  const int delimiter= ' '; // a space
  char *firstSpace, *secondPart;
  // List of commands
  const char *commands[5];
  commands[0]= "cd";
  commands[1]= "format";
  commands[2]= "ls";
  commands[3]= "mkdir";
  commands[4]= "touch";
  // Array of function pointers initialization
  typedef void(*FunctionPointers)(const char*);
  FunctionPointers function_pointers[5]= {
    cd,
    format,
    ls,
    mkdir,
    touch
  };
  unsigned i, end= NUM_ELEM(commands); // loop
  while(1) {
    printf("%s>", PROMPT);
    // Read user input
    if(fgets(input, MAX_INPUT_LENGTH, stdin) ==
                                         NULL) {
      perror("Error reading input.\n");
      break;
    }
    // Remove the newline character from the end
    // (if present)
    input[strcspn(input, "\n")]= '\0';
    // Take only command from the input string
    firstSpace= strchr(input, delimiter);
    secondPart= NULL; // for the command only
    if(firstSpace != NULL) {
      *firstSpace= '\0'; // split the input
      secondPart= firstSpace + 1; //of the input
      // Skip leading white spaces
      while(isspace(*secondPart))
        ++secondPart;
      if(strlen(secondPart) == 0) // no 2nd part
        secondPart= NULL;
    }
    // Check commands
    for(i= 0; i < end; ++i) {
      if(strcmp(input, commands[i]) == 0) {
        function_pointers[i](secondPart);
        break; // found a command
    } }
    if(i == end) { // not found a command
      printf("Unknown command \"%s\".  There "
      "are %u commands available:\n", input, i);
      for(i= 0; i < end; ++i) {
        printf("%s\n", commands[i]);
  } } }
  printf("Exiting program.\n");
  return 0;
}
