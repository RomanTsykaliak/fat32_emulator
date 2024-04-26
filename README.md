<h1 align="center">FAT32 emulator</h1>

<h4 align="center">The FAT32 emulator program that manages FAT32 filesystem backed up by a file.</h4>

## Video ##

## Pictures ##

## Project Overview ##

#### Functional requirements ####

1. The emulator program works with a regular file that backs up FAT32 filesystem.
2. Path to the file is provided to the emulator as a command line parameter.
3. The emulator should create the file of size 20 MB if it does not exist.
4. The emulator provides CLI (command line interface) with commands: `cd`, `format`, `ls`, `mkdir`, `touch`.  These commands operate on FAT32 filesystem.
5. CLI shows a prompt `/path>` where `path` is the current directory.
6. Command `cd path` changes the current directory to the specified directory `path`.  Only absolute path is allowed.  If a path does not exist, then an error is shown, and the current directory is not changed.
7. Command `format` creates FAT32 filesystem inside the file.  All information is lost.
8. Command `ls path` shows files and directories inside the given directory `path`, or in the current directory if a path is not specified.
9. Command `mkdir name` creates a directory with a given name `name` in the current directory.
10. Command `touch name` creates an empty file with a given name `name` in the current directory.
11. If the backed file does not contain a valid FAT32 filesystem, then all commands (except for `format`) should fail and output an error.

#### Non-functional requirements ####

1. Programming language: C
2. It is allowed to use third-party libraries or code.  If you use one then you must comply with its license terms and conditions.
3. All other requirements and restrictions are up to your discretion.

#### Expectations ####

1. Source code of a FAT32 emulator written according to the requirements.  Source code should be in a plain ASCII text (it is not a joke—sometimes we receive source code in MS Word document).
2. Instruction how to build, configure and run the FAT32 emulator (including necessary files, like Makefile, etc.).
3. Description how you tested the FAT32 emulator.
4. Document, if your solution has additional (known to you) limitations or restrictions, or it has more features that it was requested.

## Usage Guide ##

```bash
./f32disk /home/user/test.disk
/>ls
Unknown disk format
/>format
Ok
/>ls
. ..
/>mkdir ttt
Ok
/>ls
. .. ttt
/>cd /ttt
/ttt>ls
. ..
.ttt>cd /
/>
```

## Verification ##

## License ##

GPL-3.0-or-later
