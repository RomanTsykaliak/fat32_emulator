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
#include <stdlib.h> // memory allocation
#define FILE_SIZE_IN_MB 20 // 20MB
#define MAX_INPUT_LENGTH 8 // length+2
#define FILE_NAMES_BUFFER_SIZE 50
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
int format(const char *path) {
  char command[FILE_NAMES_BUFFER_SIZE]=
                             "/sbin/mkfs.vfat ";
  strncat(command, path,
         // -1 accounts for the null terminator
         // already in command
         sizeof(command) - strlen(command) - 1);
  int result= system(command);
  if(result == 0) {
    printf("%s completed successfully.\n",
           command);
  } else {
    printf("Error executing %s\n", command);
  }
  return 1;
}
char* create_unique_temp_folder() {
  // POSIX compliant systems like Linux
  const char *tmp="/tmp/fsXXXXXX";
  char* template= malloc(strlen(tmp) + 1);
  strcpy(template, tmp);
  char* directory_name= mkdtemp(template);
  if(directory_name == NULL) {
    perror("Couldn't created a folder for "
           "mounting.\n");
    free(template);
    return NULL;
  }
  return directory_name;
}
void delete_temp_folder(char *directory_name) {
  if(rmdir(directory_name) == -1)
    perror("Couldn't remove temporary "
           "folder.\n");
  free(directory_name);
}
int mount(const char *path, const char *dir) {
  char command[FILE_NAMES_BUFFER_SIZE]=
                               "mount -o loop ";
  strncat(command, path,
         sizeof(command) - strlen(command) - 1);
  strncat(command, " ",
         sizeof(command) - strlen(command) - 1);
  strncat(command, dir,
         sizeof(command) - strlen(command) - 1);
  int result= system(command);
  if(result == 0) {
    printf("%s completed successfully.\n",
           command);
    return 1;
  } else {
    printf("Error executing %s\n", command);
    return 0;
  }
}
int unmount(const char *dir) {
  char command[FILE_NAMES_BUFFER_SIZE]=
                                      "umount ";
  strncat(command, dir,
          sizeof(command) -strlen(command) - 1);
  int result= system(command);
  if(result == 0) {
    printf("%s completed successfully.\n",
           command);
    return 1;
  } else {
    printf("Error executing %s\n", command);
    return 0;
  }
}

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
    // Check commands
    if(        strcmp(input, "exit"  ) == 0) {
      break;
    } else if (strcmp(input, "cd"    ) == 0) {
      printf("You entered cd\n");
    } else if (strcmp(input, "format") == 0) {
      //format(argv[1]);
      char* tmp= create_unique_temp_folder();
      if(tmp == NULL)
        return 1;
      if(mount(argv[1], tmp)) {
        if(unmount(tmp))
          delete_temp_folder(tmp);
      } else {
        delete_temp_folder(tmp);
      }
      
    } else if (strcmp(input, "ls"    ) == 0) {
      printf("You entered ls\n");
    } else if (strcmp(input, "mkdir" ) == 0) {
      printf("You entered mkdir\n");
    } else if (strcmp(input, "touch" ) == 0) {
      printf("You entered touch\n");
    } else {
      printf("Unknown command.  Available "
             "commands are exit, cd, format, "
             "ls, mkdir, touch\n");
  } }
  printf("Exiting program.\n");
  return 0;
}
