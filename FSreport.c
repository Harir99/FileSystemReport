#include "FSreport.h"

int main(int argc, char *argv[])
{
    char *fileDir = argv[2];
    int level = 1;
    int treeLevel = 1;
    if (argc != 3)
    {
        printf("Correct Fromat Choose Either:\n");
        printf("1- ./FSreport -tree  path/to/file \n");
        printf("2- ./FSreport -inode path/to/file\n");

        return 1;
    }

    if (strcmp(argv[1], "-tree") == 0)
    {
        printf("File System Report: Tree Directory Structure\n");
        tree(fileDir, treeLevel);
        
    }
    if (strcmp(argv[1], "-inode") == 0)
    {
        printf("File System Report: Inodes\n");
        inode(fileDir, level);
    }
}

int getIndex(char *string, char c)
{
    for (int i = strlen(string); i >= 0; i--)
    {
        if (string[i] == c)
        {
            return i;
        }
    }
    return -1;
}
char *strip(char *string, char *toRemove, int index)
{
    char *stripped = calloc(strlen(string) + 1, sizeof(char));
    for (int i = index; i <= strlen(string) - 1; i++)
    {
        stripped[i - index - 1] = string[i];
    }
    return stripped;
}
void treeDirectories(char *fileDir, int level)
{
    bool isDirFirst = false;
    struct dirent **namelist;
    struct stat fileinfo;
    int n;
    int i = 0;
    char *absolutePath = malloc(sizeof(char) * 1000);
    strcpy(absolutePath, fileDir);
    n = scandir(fileDir, &namelist, NULL, alphasort);

    if (n < 0)
        perror("scandir");

    else
    {
        while (i < n)
        {
            absolutePath = getAbsolutePath(absolutePath, fileDir, namelist, fileinfo, i);
            
            if (!stat(absolutePath, &fileinfo))
            {
                if (strcmp(namelist[i]->d_name, ".") != 0 && strcmp(namelist[i]->d_name, "..") != 0)
                {
                    if (S_ISDIR(fileinfo.st_mode))
                    {
                        FileOrDirectory(&isDirFirst,"Directories");
                        printFileProperties(fileinfo, namelist, i, 0, 1);
                    }
                }
            }
            ++i;
        }
        free(namelist);
    }

}
void tree(char *fileDir, int level)
{
    if (level == 1)
        printf("Level 1: %s\n", fileDir);
    int index = getIndex(fileDir, '/');
    char *stripped = strip(fileDir, "/", index);
    if (level != 1)
        printf("Level %d: %s\n", level, stripped);

    bool isFileFirst = false;

    struct dirent **namelist;
    struct stat fileinfo;
    int n;
    int i = 0;
    char *absolutePath = malloc(sizeof(char) * 1000);
    strcpy(absolutePath, fileDir);
    n = scandir(fileDir, &namelist, NULL, alphasort);

    if (n < 0)
        perror("scandir");

    else
    {
        treeDirectories(fileDir,level);

        ISFILE isFile = FileOrDirectory;
        treeInfo(&absolutePath, &fileDir, &namelist, &fileinfo, &i, &n, &level, isFile, &isFileFirst, "Files");

        FUNCPTR itself = tree;
        RecursiveLevels(&absolutePath, &fileDir, &namelist, &fileinfo, n, &level, itself);
        free(namelist);
    }
}
void RecursiveLevels(char **absolutePath, char **fileDir, struct dirent ***namelist, struct stat *fileinfo, int n, int *level, FUNCPTR func)
{
    for (int i = 0; i < n; i++)
    {
        *absolutePath = getAbsolutePath(*absolutePath, *fileDir, *namelist, *fileinfo, i);

        if (!stat(*absolutePath, fileinfo))
        {
            if (S_ISDIR((*fileinfo).st_mode))
            {
                if (strcmp((*namelist)[i]->d_name, ".") != 0 && strcmp((*namelist)[i]->d_name, "..") != 0)
                {
                    func(*absolutePath, *level + 1);
                }
            }
        }
    }
}
void FileOrDirectory(bool *check, char *string)
{
    if (*check == false)
        printf("%s\n", string);
    *check = true;
}
void treeInfo(char **absolutePath, char **fileDir, struct dirent ***namelist, struct stat *fileinfo, int *i, int *n, int *level, ISFILE isfile, bool *check, char *string)
{
    while (*i < *n)
    {
        *absolutePath = getAbsolutePath(*absolutePath, *fileDir, *namelist, *fileinfo, *i);

        if (!stat(*absolutePath, fileinfo))
        {
            if (strcmp((*namelist)[*i]->d_name, ".") != 0 && strcmp((*namelist)[*i]->d_name, "..") != 0)
            {
                if (strcmp(string, "Directories") == 0)
                {
                    if (S_ISDIR((*fileinfo).st_mode))
                    {
                        isfile(check, string);
                        printFileProperties(*fileinfo, *namelist, *i, 0, 1);
                    }
                }
                else
                {
                    if (S_ISREG((*fileinfo).st_mode))
                    {
                        isfile(check, string);
                        printFileProperties(*fileinfo, *namelist, *i, 0, 1);
                    }
                }
            }
        }
        *i = *i + 1;
    }
}
int inodesort(const struct dirent **a, const struct dirent **b)
{
    return MIN((*a)->d_ino, (*b)->d_ino);
}
void inode(char *fileDir, int level)
{
    struct dirent **namelist;
    struct stat fileinfo;
    int n;
    int i = 0;
    if (level == 1)
        printf("Level 1 Inodes: %s\n", fileDir);
    int index = getIndex(fileDir, '/');
    char *stripped = strip(fileDir, "/", index);
    if (level != 1)
        printf("Level %d Inodes: %s\n", level, stripped);

    // write in read me limit is 1000
    char *absolutePath = malloc(sizeof(char) * 1000);
    strcpy(absolutePath, fileDir);
    n = scandir(fileDir, &namelist, NULL, inodesort);

    if (n < 0)
        perror("scandir");

    else
    {
        while (i < n)
        {
            absolutePath = getAbsolutePath(absolutePath, fileDir, namelist, fileinfo, i);

            if (!stat(absolutePath, &fileinfo))
            {
                if (strcmp(namelist[i]->d_name, ".") != 0 && strcmp(namelist[i]->d_name, "..") != 0)
                {
                    printFileProperties(fileinfo, namelist, i, 1, 0);
                }
            }
            ++i;
        }

        FUNCPTR itself = inode;
        RecursiveLevels(&absolutePath, &fileDir, &namelist, &fileinfo, n, &level, itself);
        free(namelist);
    }
}
char *getAbsolutePath(char *absolutePath, char *fileDir, struct dirent **namelist, struct stat fileinfo, int i)
{
    strcpy(absolutePath, fileDir);
    if (fileDir[strlen(fileDir)] != '/')
    {
        absolutePath = strcat(absolutePath, "/");
    }
    absolutePath = strcat(absolutePath, namelist[i]->d_name);

    if ((stat(absolutePath, &fileinfo)) == -1)
    {
        printf("can't find %s\n", namelist[i]->d_name);
    }
    return absolutePath;
}
char *permissions(mode_t perm)
{
    char *modeval = malloc(sizeof(char) * 9 + 1);
    modeval[0] = (perm & S_IFDIR) ? 'd' : '-';
    modeval[1] = (perm & S_IRUSR) ? 'r' : '-';
    modeval[2] = (perm & S_IWUSR) ? 'w' : '-';
    modeval[3] = (perm & S_IXUSR) ? 'x' : '-';
    modeval[4] = (perm & S_IRGRP) ? 'r' : '-';
    modeval[5] = (perm & S_IWGRP) ? 'w' : '-';
    modeval[6] = (perm & S_IXGRP) ? 'x' : '-';
    modeval[7] = (perm & S_IROTH) ? 'r' : '-';
    modeval[8] = (perm & S_IWOTH) ? 'w' : '-';
    modeval[9] = (perm & S_IXOTH) ? 'x' : '-';
    modeval[10] = '\0';
    return modeval;
}
void printFileProperties(struct stat fileinfo, struct dirent **namelist, int i, int iMode, int tMode)
{
    if (iMode == 1)
    {
        printf("%ld:\t", fileinfo.st_ino);
        printf("%ld\t", fileinfo.st_size);
        printf("%ld\t", fileinfo.st_blocks);
        printf("%ld\t", fileinfo.st_size / 512);
        printf("%s\n", namelist[i]->d_name);
    }
    if (tMode == 1)
    {
        struct passwd *pwd = getpwuid(fileinfo.st_uid);
        struct group *grp = getgrgid(fileinfo.st_gid);
        printf("%s(%s)\t", pwd->pw_name, grp->gr_name);
        printf("%ld\t", fileinfo.st_ino);
        char *mode = permissions(fileinfo.st_mode);
        printf("%s\t", mode);
        printf("%ld\t", fileinfo.st_size);
        printf("%s\n", namelist[i]->d_name);

        printf("\t%s\t", ctime(&fileinfo.st_atime));
        printf("%s\n", ctime(&fileinfo.st_mtime));
    }
}