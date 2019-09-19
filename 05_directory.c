#include <stdio.h>
#include <dirent.h>

#define TYPE2STR(X) \
    ((X) == DT_FIFO ? "named pipe" : \
    (X) == DT_CHR   ? "character special" : \
    (X) == DT_DIR   ? "directory" : \
    (X) == DT_BLK   ? "block special" : \
    (X) == DT_REG   ? "regular" : \
    (X) == DT_LNK   ? "symbolic link" : \
    (X) == DT_SOCK  ? "socket" : \
    (X) == DT_WHT   ? "what" : \
    "unknown")

int main()
{
    DIR* dirp = opendir(".");
    if (dirp == NULL) {
        perror("cannot open directory");
        return -1;
    }

    struct dirent* dp = NULL;
    while ((dp = readdir(dirp)) != NULL)
        printf("%s : %s\n", dp->d_name, TYPE2STR(dp->d_type));

    closedir(dirp);

    return 0;
}