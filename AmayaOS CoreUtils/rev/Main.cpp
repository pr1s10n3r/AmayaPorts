/*
 * Copyright (C) 2017 Alvaro Stagg [alvarostagg@openmailbox.org]
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
#include <sys/stat.h>

#define VERSION "0.1"

static void show_help(void);
static size_t get_file_size(const char* fileName);

int main(int argc, const char* argv[])
{
    //TODO: Agregar soporte para STDIN
    if (argc < 2)
    {
        printf("Usage: '%s [OPCIONES...] [FICHEROS...]'\n", argv[0]);
        return 1;
    }

    int hflag = 0, vflag = 0;

    for (int i = 1; i < argc; i++)
    {
        if ((strcmp(argv[i], "--help") == 0) || (strcmp(argv[i], "-h") == 0))
        {
            hflag = 1;
            show_help();
            break;
        }
        else if ((strcmp(argv[i], "--version") == 0) || (strcmp(argv[i], "-v") == 0))
        {
            vflag = 1;
            printf("%s de AmayaOS Coreutil %s\n", argv[0], VERSION);
            break;
        }
    }

    if (hflag || vflag)
        return 0;

    int errno = 0;

    FILE* file = NULL;
    for (int i = 1; i < argc; i++)
    {
        if ((file = fopen(argv[i], "r")) == NULL)
        {
            errno  = 1;
            printf("%s: no se puede abrir %s: No existe el fichero o el directorio\n", argv[0], argv[i]);
            break;
        }
        else
        {
            size_t fileSize = get_file_size(argv[i]);
            if (fileSize < 0)
            {
                errno = 1;
                printf("%s: no se puede obtener el tamaño de \"%s\"\n", argv[0], argv[i]);
                break;
            }

            char content[fileSize];
            int n_bytes = fread(content, 1, sizeof(content), file);
            fclose(file);

            char line[n_bytes];
            int lp = 0;

            for (unsigned int x = 0; x < n_bytes; x++)
            {
                if (content[x] != '\n')
                {
                    line[lp] = content[x];
                    lp++;
                }
                else
                {
                    for (int r = lp - 1; r >= 0; r--)
                        printf("%c", line[r]);
                    
                    lp = 0;
                    printf("\n");
                }
            }
        }
    }

    return errno;
}

static void show_help(void)
{
    printf("Modo de empleo: rev [OPCIONES...] [FICHEROS...]\n\n"
           "Invierte líneas carácter a carácter.\n\n"
           "Opciones:\n"
           " -h, --help     muestra esta ayuda y sale\n"
           " -v, --version  muestra información y sale\n");
}

static size_t get_file_size(const char* fileName)
{
    struct stat st;
    if (stat(fileName, &st) < 0)
        return -1;

    return st.st_size;
}
