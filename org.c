#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

void build_path(char *dest, const char *src, const char *file)
{
    strcpy(dest, src);
    strcat(dest, "/");
    strcat(dest, file);
    strcat(dest, "\0");
}

long take_input_timestamp(char *input, int size, char *prompt, bool is_req)
{
    printf("Enter %s timestamp in format (dd:mm:yyyy hh:mm:ss): ", prompt);
    fgets(input, size, stdin);
    input[strcspn(input, "\n")] = 0;

    if (is_req && input[0] == '\0')
    {
        fprintf(stderr, "Timestamp is required.\n");
        return -1;
    }

    if (!is_req && strcmp(input, "-") == 0)
    {
        return 0;
    }

    int dat, mon, yr, hr, min, sec;

    if (sscanf(input, "%d:%d:%d %d:%d:%d", &dat, &mon, &yr, &hr, &min, &sec) != 6)
    {
        fprintf(stderr, "Invalid timestamp format. It should be (dd:mm:yyyy hh:mm:ss)  \n");
        return -1;
    }

    struct tm time = {0};
    time.tm_mday = dat;
    time.tm_mon = mon - 1;
    time.tm_year = yr - 1900;
    time.tm_hour = hr;
    time.tm_min = min;
    time.tm_sec = sec;

    return mktime(&time);
}

void write_log(const char *original_path, const char *new_path)
{
    time_t now = time(NULL);

    char log_file_path[512];
    const char *home_dir = getenv("HOME");
    build_path(log_file_path, home_dir, ".file_org.txt");

    FILE *log_file = fopen(log_file_path, "a");
    if (log_file == NULL)
    {
        fprintf(stderr, "Error opening log file: %s: %s\n", log_file_path, strerror(errno));
        return;
    }

    fprintf(log_file, "%ld|%s|%s\n", now, original_path, new_path);
    fclose(log_file);
}

void organise(int args_count, char *args[])
{
    int count = 0;
    int count_dir = 0;

    printf("Organising files in %d directories. \n", args_count - 1);

    for (int i = 1; i < args_count; i++)
    {
        printf("Processing directory: %s. \n", args[i]);

        DIR *directory = opendir(args[i]);

        if (directory == NULL)
        {
            fprintf(stderr, "Error opening directory: %s: %s\n", args[i], strerror(errno));
            return;
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
            {
                count++;
                write_log(file_path, new_path);
            }
        }

        closedir(directory);
    }
    printf("Successfully organised %d files. \n", count);
    printf("Newly created %d directories. \n", count_dir);
}

void deorganise(int args_count, char *args[])
{
    int count = 0;

    const char *home_dir = getenv("HOME");
    char log_file_path[512];
    char temp_file_path[512];
    build_path(log_file_path, home_dir, ".file_org.txt");
    build_path(temp_file_path, home_dir, ".temp_org.txt");

    FILE *temp_file = fopen(temp_file_path, "w");
    FILE *log_file = fopen(log_file_path, "r");
    if (log_file == NULL || temp_file == NULL)
        return;

    long start_timestamp = 0, end_timestamp = 0;
    char input[100];

    start_timestamp = take_input_timestamp(input, sizeof(input), "start", true);
    if (start_timestamp == -1)
        return;

    end_timestamp = take_input_timestamp(input, sizeof(input), "end", false);
    if (end_timestamp == -1)
        return;

    char str[2048];

    printf("De-organising files...\n");

    while (fgets(str, sizeof(str), log_file) != NULL)
    {
        char copy_str[2048];
        strcpy(copy_str, str);

        char *timestamp = strtok(copy_str, "|");
        char *original_path = strtok(NULL, "|");
        char *new_path = strtok(NULL, "\n");

        if (timestamp == NULL || original_path == NULL || new_path == NULL)
            continue;

        long log_time = atol(timestamp);

        if (log_time >= start_timestamp && (end_timestamp == 0 || log_time <= end_timestamp))
        {
            if (rename(new_path, original_path) != 0)
            {
                fprintf(stderr, "Error moving file: %s to %s: %s\n", new_path, original_path, strerror(errno));
            }
            else
                count++;
        }
        else
        {
            fprintf(temp_file, "%s", str);
        }
    }

    fclose(log_file);
    fclose(temp_file);

    if (rename(temp_file_path, log_file_path) != 0)
    {
        fprintf(stderr, "Error updating log file: %s: %s\n", log_file_path, strerror(errno));
    }

    printf("Successfully de-organised %d files. \n", count);
}

int main(int args_count, char *args[])
{
    if (args_count < 2)
    {
        fprintf(stderr, "Usage: org <directory1> [<directory2> ...]\n");
        return 1;
    }
    if (!strcmp(args[1], "-u"))
    {
        deorganise(args_count, args);
        return 0;
    }
    organise(args_count, args);
    return 0;
}