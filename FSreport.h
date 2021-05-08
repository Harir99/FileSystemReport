#ifndef FSREPORT_H
#define FSREPORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef void (*ISFILE)(bool* check, char* string);
typedef void (*FUNCPTR)(char *fileDir, int level);
typedef char* (*FUNCPTR2)(char *fileDir, int level);

typedef struct Chart {
    char *alpha;
    struct dirent **namelist;
}Chart;

int inodesort (const struct dirent **a, const struct dirent **b);
char* permissions(mode_t perm);
char** getPath(char *fileDir);
void tree(char *fileDir, int level);
int getIndex(char *string, char c);
void treeDirectories(char *fileDir, int level);
char *strip(char *string, char *toRemove,int index);
void printFileProperties(struct stat file,struct dirent **namelist,int i,int iMode, int tMode);
void inode(char *fileDir, int level);
bool inodeInfo(char** absolutePath,char **fileDir,struct dirent ***namelist,struct stat *fileinfo,int *i, int *n, int* level);
char *getAbsolutePath(char *absolutePath, char *fileDir, struct dirent **namelist, struct stat fileinfo, int i);
void FileOrDirectory(bool* check, char* string);
void treeInfo(char **absolutePath, char **fileDir, struct dirent ***namelist, struct stat *fileinfo, int *i, int *n, int *level,ISFILE isfile, bool* check, char* string);
void RecursiveLevels(char **absolutePath, char **fileDir, struct dirent ***namelist, struct stat *fileinfo, int n, int *level, FUNCPTR func);

#endif
