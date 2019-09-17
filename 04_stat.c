#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
    struct stat st;

    if (argc < 2) {
        puts("usage : ./a.out {filename}");
        return -1;
    }

    if (stat(argv[1], &st) == -1) {
        printf("stat failed for %s", argv[1]);
        return -1;
    }

    printf("file name : %s\n", argv[1]);
    printf("file size : %d\n", st.st_size);
    printf("file type : ");
    if (S_ISFIFO(st.st_mode)) puts("named pipe");
    if (S_ISCHR(st.st_mode)) puts("character special");
    if (S_ISDIR(st.st_mode)) puts("directory");
    if (S_ISBLK(st.st_mode)) puts("block special");
    if (S_ISREG(st.st_mode)) puts("regular");
    if (S_ISLNK(st.st_mode)) puts("symbolic link");
    if (S_ISSOCK(st.st_mode)) puts("socket");

    return 0;    
}