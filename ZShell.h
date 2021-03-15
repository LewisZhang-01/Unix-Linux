/*
Student Name: Zhi Zhang
Student Number: 18210054
Email: zhi.zhang@ucdconnect.ie
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>

void init_shell();
void printDir();
void prompt();
void newPerror(char **input,char *cmd);
void cmdHandle(char** input);
void separateStr(char *buffer, char **output_str, char *by);
bool redirect(char **input);
void cd(char **input);
void ls(char **input);
void cat(char **input);
void str_replace(char *str,char *oldstr,char *newstr);
