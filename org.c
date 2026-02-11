#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

void build_path(char *dest, const char *src, const char *file)
{
    strcpy(dest, src);
    strcat(dest, "/");
    strcat(dest, file);
    strcat(dest, "\0");
}

int main(int args_count, char *args[])
{
    int count = 0;
    int count_dir = 0;
    if (args_count < 2)
    {
        fprintf(stderr, "Usage: %s <at least one directory> \n", args[0]);
        return 1;
    }

    printf("Organising files in %d directories. \n", args_count - 1);

    for (int i = 1; i < args_count; i++)
    {
        printf("Processing directory: %s. \n", args[i]);

        DIR *directory = opendir(args[i]);

        if (directory == NULL)
        {
            fprintf(stderr, "Error opening directory: %s: %s\n", args[i], strerror(errno));
            return 1;
        }

        struct dirent *curr_file;
        struct stat file_status;

        while ((curr_file = readdir(directory)) != NULL)
        {
            if (strcmp(curr_file->d_name, ".") == 0 || strcmp(curr_file->d_name, "..") == 0)
                continue;

            char file_path[2048];
            build_path(file_path, args[i], curr_file->d_name);

            if (stat(file_path, &file_status) == -1)
                continue;

            if (!S_ISREG(file_status.st_mode))
                continue;

            char *dot_idx = strrchr(curr_file->d_name, '.');
            char *extension = dot_idx ? dot_idx + 1 : "others";

            char target_dir[2048];
            build_path(target_dir, args[i], extension);

            if (mkdir(target_dir, 0755) == -1 && errno != EEXIST)
            {
                fprintf(stderr, "Error creating directory: %s: %s\n", target_dir, strerror(errno));
                continue;
            }
            else if (errno != EEXIST)
                count_dir++;

            char new_path[2048];
            build_path(new_path, target_dir, curr_file->d_name);

            if (rename(file_path, new_path) != 0)
            {
                fprintf(stderr, "Error moving file: %s to %s: %s\n", file_path, new_path, strerror(errno));
            }
            else
                count++;
        }

        closedir(directory);
    }
    printf("Successfully organised %d files. \n", count);
    printf("Newly created %d directories. \n", count_dir);

    return 0;
}