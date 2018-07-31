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

#define VERSION "0.1"

static int tree(const char*);
static int vflag = 0, hflag = 0;
static int aflag = 0, dflag = 0;
static int fflag = 0;

static void show_help(void);
static void show_version(void);

int main(const int argc, const char* argv[])
{
    int status = 0;
    int i = 1, path_set = 0;
    char* path = NULL;

    for (i = 1; (i < argc) && !vflag && !hflag; i++)
    {
        if (strcmp(argv[i], "--help") == 0)
        {
            hflag = 1;
            show_help();
        }
        else if (strcmp(argv[i], "--version") == 0)
        {
            vflag = 1;
            show_version();
        }
        else if (strcmp(argv[i], "-a") == 0)
        {
            aflag = 1;
        }
        else if (strcmp(argv[i], "-d") == 0)
        {
            dflag = 1;
        }
        else if (strcmp(argv[i], "-f") == 0)
        {
            fflag = 1;
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
                if (path_set == 0)
                {
                    path = (char*)argv[i];
                    path_set = 1;
                }
                else
                {
                    printf("%s: multiple paths not supported.\n", argv[0]);
                    status = 1;
                }
            }
        }
    }

    if (vflag || hflag)
        return status;

    if (!path_set)
    {
        path = (char*)malloc(sizeof(char) * 2);
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

    if (!path_set)
    {
        free(path);
    }

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
        
        const size_t full_path_len = (sizeof(char) * strlen(path)) + (sizeof(char) * strlen(file->d_name)) + 2;
        char* full_path = (char*)malloc(full_path_len);
        strcpy(full_path, path);
        
        if (path[strlen(path) - 1] != '/')
            strcat(full_path, "/");
        
        strcat(full_path, file->d_name);

        if (!dflag)
        {
            if (fflag)
                print_wspaces(spaces, full_path);
            else
                print_wspaces(spaces, file->d_name);
        }

        if (is_directory(full_path))
        {
            if (dflag)
            {
                if (fflag)
                    print_wspaces(spaces, full_path);
                else
                    print_wspaces(spaces, file->d_name);
            }

            spaces += 4;
            status = tree(full_path);
            spaces -= 4;
        }

        free((char*)full_path);
    }

    closedir(directory);
    return status;
}

static void show_version()
{
    printf("tree (AmayaOS Coreutils) v%s\n"
           "Copyright © 2018 AmayaOS Team.\n"
           "Licencia GPLv3+: GPL de GNU versión 3 o posterior\n"
           "<http://gnu.org/licenses/gpl.html>.\n"
           "Esto es software libre: usted es libre de cambiarlo y redistribuirlo.\n"
           "No hay NINGUNA GARANTÍA, hasta donde permite la ley.\n\n", VERSION);
}

static void show_help(void)
{
    printf("tree [-adf] [--version] [--help] [directory...]\n");
}
