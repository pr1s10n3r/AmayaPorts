/*
 * Copyright (C) 2016 Alvaro Stagg [alvarostagg@openmailbox.org]
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

#define VERSION "v0.6"

static void usage(const char* msg);
static void version(void);
static void help(void);
static int GetArgSize(char* argv[]);
static void dirname(int argc, char* argv[]);

static int hflag = 0, vflag = 0, zflag = 0;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        usage("dirname: falta un operando");
        return 0;
    }

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0)
        {
            help();
            hflag = 1;
            break;
        }
        else if(strcmp(argv[i], "--version") == 0)
        {
            version();
            vflag = 1;
            break;
        }
        else if((strcmp(argv[i], "-z") == 0) || (strcmp(argv[i], "--zero") == 0))
        {
            zflag = 1;
            break;
        }
    }

    if (hflag || vflag)
        return 0;
    else
        dirname(argc, argv);

    return 0;
}

static void dirname(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0)
            continue;
        else if(strcmp(argv[i], "--version") == 0)
            continue;
        else if((strcmp(argv[i], "-z") == 0) || (strcmp(argv[i], "--zero") == 0))
            continue;
        else
        {
            size_t nSize = strlen(argv[i]);
            int sChecked = 0;

            for (int j = 0; j <= nSize; j++)
            {
                if (argv[i][j] == '/' && !sChecked)
                {
                    printf("%c", argv[i][j]);
                    sChecked = 1;
                    continue;
                }
                else
                {
                    if (argv[i][j] != '/')
                    {
                        printf("%c", argv[i][j]);

                        if (argv[i][j + 1] == '/')
                            break;
                    }
                }
            }
            
            if (!zflag)
                printf("\n");
        }
    }
}

static void usage(const char* msg)
{
    printf("%s\n", msg);
    printf("Pruebe: 'dirname --help' para más información.\n");
}

static void version(void)
{
    printf("dirname (AmayaOS CoreUtils) %s\n"
           "Copyright © 2016 AmayaOS, Inc.\n"
           "Licencia GPLv3+: GPL de GNU versión 3 o posterior\n"
           "<http://gnu.org/licenses/gpl.html>.\n"
           "Esto es software libre: usted es libre de cambiarlo y redistribuirlo.\n"
           "No hay NINGUNA GARANTÍA, hasta donde permite la ley.\n\n", VERSION);
    printf("Portado a AmayaOS por Alvaro Stagg.\n");
}

static void help(void)
{
    printf("Modo de empleo: dirname [OPCIÓN] NOMBRE...\n"
           "Muestra NOMBRE eliminando las barras finales y la última componente que no sea una barra;"
           "si NOMBRE no tiene /'s, el resultado es '.' (representado el directorio actual).\n\n"
           "  -z, --zero     termina cada línea de salida con NUL, no con nueva línea\n\n"
           "      --help     muestra esta ayuda y finaliza\n"
           "      --version  informa de la versión y finaliza\n\n"
           "Ejemplos:\n"
           "  dirname /usr/bin/          -> \"/usr\"\n"
           "  dirname dir1/str dir2/str  -> \"dir1\" seguido por \"dir2\"\n"
           "  dirname stdio.h            -> \".\"\n\n"
           "ayuda en línea sobre AmayaOS CoreUtils: <https://wiki.amayaos.com/index.php/AmayaCoreutils>\n");
}