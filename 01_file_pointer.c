#include <stdio.h>
#include <string.h>

char filepath[] = "people";

struct person {
    char name[4];
    int age;
};

int write_to_file()
{
    FILE *fptr = NULL;

    struct person kim = {
        .name = "kim",
        .age = 20
    };

    struct person park = {
        .name = "lee",
        .age = 30
    };

    if (!(fptr = fopen(filepath, "w"))) {
        return -1;
    }

    if (!fwrite(&kim, sizeof(struct person), 1, fptr)) {
        goto error;
    }

    if (!fwrite(&park, sizeof(struct person), 1, fptr)) {
        goto error;
    }

    fclose(fptr);
    return 0;
error:
    if (fptr) {
        fclose(fptr);
    }
    return -1;
}

int read_from_file()
{
    FILE *fptr = NULL;
    struct person people[2];

    if (!(fptr = fopen(filepath, "r"))) {
        return -1;
    }

    if (fread(people, sizeof(struct person), 2, fptr) != 2) {
        goto error;
    }

    for (int i=0; i<2; i++)
        printf("name : %s, age : %d\n", people[i].name, people[i].age);

    fclose(fptr);
    return 0;
error:
    if (fptr) {
        fclose(fptr);
    }
    return -1;
}

// fopen
// getc, gets, fread
// putc, puts, fwrite
// fclose

int main()
{
    if (write_to_file()) {
        printf("fwrite error occured");
        return -1;
    }

    if (read_from_file()) {
        printf("fread error occured");
        return -1;
    }

    return 0;
}