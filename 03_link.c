// link, symlink, unlink

/*
$ echo "hello" > hello_world

$ ln hello_world hello_hardlinked
$ stat hello_world
$ stat hello_hardlinked

$ ln -s hello_world hello_symlink
$ stat hell_symlink

*/

#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>

const char filepath_hardlink[] = "hello_hardlink";
const char filepath_softlink[] = "hello_symlink";

int main(int argc, char** argv)
{
    int fd;
    int ret;

    puts("## create new file");
#define TARGET_FILE "hello_world"
    fd = open(TARGET_FILE, O_WRONLY | O_CREAT, 0644);
    assert(fd != -1);
    write(fd, "hello world!\n", 14);
    close(fd);

    puts("## create hardlink");
    ret = link(TARGET_FILE, filepath_hardlink);
    assert(ret != -1);

    puts("## create symbolic link");
    ret = symlink(TARGET_FILE, filepath_softlink);
    assert(ret != -1);

    return 0;
}
