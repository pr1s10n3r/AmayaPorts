/*
 * Copyright (C) 2018 Alvaro Stagg [alvarostagg@protonmail.com]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int tree(const char*);
static int vflag = 0, hflag = 0;
static int aflag = 0, dflag = 0;

int main(const int argc, const char* argv[])
{
    int status = 0;
    int i = 1;
    char* path = NULL;

    for (; (i < argc) && !vflag && !hflag; i++)
    {
        if (strcmp(argv[i], "--help") == 0)
        {
            hflag = 1;
        }
        else if (strcmp(argv[i], "--version") == 0)
        {
            vflag = 1;
        }
        else if (strcmp(argv[i], "-a") == 0)
        {
            aflag = 1;
        }
        else if (strcmp(argv[i], "-d") == 0)
        {
            dflag = 1;
        }
        else
        {
            if (argv[i][0] == '-')
            {
                printf("%s: invalid option '%s'", argv[0], argv[i]);
                status = 1;
            }
            else
            {
                path = (char*)argv[i];
            }
        }
    }

    if (vflag || hflag)
        return status;

    if (!path)
    {
        path = (char*)malloc(2);
        strcpy(path, ".");
    }

    int fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        printf("%s: unable to open path '%s'.\n", argv[0], path);
        status = 1;
    }

    if (status == 0)
    {
        printf("%s\n", path);
        status = tree(path);
        printf("\n");
    }

    free(path);
    return status;
}

static int is_directory(const char* path)
{
    int is_dir = 0;
    struct stat st;

    if (stat(path, &st) == 0)
        if (S_ISDIR(st.st_mode))
            is_dir = 1;
    
    return is_dir;
}

static void print_wspaces(int spaces, const char* name)
{
    for (int s = 0; s < spaces; s++)
        printf(" ");
    printf("\\- %s\n", name);
}

static int tree(const char* path)
{
    DIR* directory = opendir(path);
    if (!directory)
        return 1;

    struct dirent* file;
    static int spaces = 4;
    static int status = 0;

    while ((file = readdir(directory)) != NULL)
    {
        if (aflag == 0 && file->d_name[0] == '.')
            continue;
        if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
            continue;

        if (!dflag)
            print_wspaces(spaces, file->d_name);
        
        const size_t full_path_len = strlen(path) + strlen(file->d_name) + 2;
        char* full_path = (char*)malloc(full_path_len);
        strcpy(full_path, path);
        
        if (path[strlen(path) - 1] != '/')
            strcat(full_path, "/");
        
        strcat(full_path, file->d_name);

        if (is_directory(full_path))
        {
            if (dflag)
                print_wspaces(spaces, file->d_name);

            spaces += 4;
            status = tree(full_path);
            spaces -= 4;
        }

        free(full_path);
    }

    closedir(directory);
    return status;
}
