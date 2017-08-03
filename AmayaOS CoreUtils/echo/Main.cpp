/*
 * Copyright (C) 2017 Alvaro Stagg [alvarostagg@protonmail.com]
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

#define VERSION "0.6"

static void usage(void);
static void version(void);

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("\n");
        return 0;
    }

    int nflag, hflag, vflag;
    nflag = hflag = vflag = 0;

    int i = 0, j = 0;
    const char *echos[argc];

    // TODO: Opción -e, -E
    for (i = 1; i < argc; i++)
    {
        if (strcmp("--help", argv[i]) == 0)
        {
            hflag = 1;
            usage();
            break;
        }
        else if (strcmp("--version", argv[i]) == 0)
        {
            vflag = 1;
            version();
            break;
        }
        else if (strcmp("-n", argv[i]) == 0)
            nflag = 1;
        else
        {
            echos[j] = argv[i];
            j++;
        }
    }

    if (hflag || vflag)
        return 0;

    for (i = 0; i < j; i++)
    {
        if (i + 1 == j)
            printf("%s", echos[i]);
        else
            printf("%s ", echos[i]);
    }

    if (!nflag)
        printf("\n");

    return 0;
}

static void usage(void)
{
        printf("Modo de empleo: echo [OPCIÓN-CORTA]... [CADENA]...\n"
               "                echo OPCIÓN-LARGA\n\n"
               "  -n         no imprime una nueva línea\n\n"
               "  --help     imprime éste texto y finaliza\n"
               "  --version  muestra la versión y finaliza\n\n"

               "AmayaOS Coreutils %s (C) 2017 AmayaOS Team\n"
               "Licencia GNU GPL v3 <http://www.gnu.org/licenses/>.\n"
               "Reportar errores a través de http://bugs.amayaos.com o alvarostagg@protonmail.com\n", VERSION);
}

static void version(void)
{
    printf("echo (AmayaOS Coreutils) v%s\n"
           "Copyright © 2017 AmayaOS Team.\n"
           "Licencia GPLv3+: GPL de GNU versión 3 o posterior\n"
           "<http://gnu.org/licenses/gpl.html>.\n"
           "Esto es software libre: usted es libre de cambiarlo y redistribuirlo.\n"
           "No hay NINGUNA GARANTÍA, hasta donde permite la ley.\n\n", VERSION);
}
