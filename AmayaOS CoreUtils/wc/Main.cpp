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

static void usage(void);
static void version(void);

static int read(int, char*);
static size_t get_file_size(const char*);

static int hflag, vflag;
        /* bytes, chars, lines, words, max-line */
static int cflag, mflag, lflag, wflag, mlflag;

int main(int argc, char* argv[])
{
    int ret = 0;

    int i = 1;
    for (; i < argc; i++)
    {
        if ((strcmp(argv[i], "--help")) == 0)
        {
            hflag = 1;
            usage();
            break;
        }
        else if ((strcmp(argv[i], "--version")) == 0)
        {
            vflag = 1;
            version();
            break;
        }
        else if ((strcmp(argv[i], "-c") == 0) || (strcmp(argv[i], "--bytes") == 0))
            cflag = 1;
        else if ((strcmp(argv[i], "-m") == 0) || (strcmp(argv[i], "--chars") == 0))
            mflag = 1;
        else if ((strcmp(argv[i], "-l") == 0) || (strcmp(argv[i], "--lines") == 0))
            lflag = 1;
        else if ((strcmp(argv[i], "-L") == 0) || (strcmp(argv[i], "--max-line-length") == 0))
            mlflag = 1;
        else
        {
            if (argv[i][0] == '-')
            {
                if (argv[i][1] == '-')
                    printf("%s: unrecognized option '%s'\n", argv[0], argv[i]);
                else
                {
                    printf("%s: invalid option -- '", argv[0]);
                    for (int j = 0; j <= strlen(argv[i]); j++)
                    {
                        if (argv[i][j] == '-')
                            continue;
                        else
                            printf("%c", argv[i][j]);
                    }

                    printf("'\n");
                }

                ret = -1;
            }
        }
    }

    if (ret < 0)
    {
        printf("Pruebe '%s --help' para más información.\n", argv[0]);
        return ret;
    }

    /* Si no hay ninguna flag -> formateamos la salida. */
    if (cflag + mflag + lflag + wflag + mlflag == 0)
    {
        printf(" ");
        lflag = wflag = cflag = 1;
    }

    if (hflag || vflag)
        return ret;

    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
            continue;
        else
        {
            ret = read(argc, argv[i]);
            if (ret < 0)
            {
                printf("%s: no se pudo abrir el archivo %s.\n", argv[0], argv[i]);
                break;
            }
        }
    }

    return ret;
}

static int read(int argc, char* argv)
{
    FILE *file = fopen(argv, "r");
    if (!file)
        return -1;

    char cnt[get_file_size(argv)];
    int n_bytes = fread(cnt, 1, sizeof(cnt), file);

    int lineas = 0, chars = 0, words = 0, tmp_max = 0, max_line = 0;
    for (unsigned int i = 0; i < n_bytes; i++)
    {
        // Aquí hay un problema cuando lee archivos binarios
        // No arroja el mismo resultado que el "wc" de GNU Coreutils.
        if (mlflag)
        {
            if (cnt[i] != '\n')
                tmp_max++;
            else
                tmp_max = 0;

            if (tmp_max > max_line)
                max_line = tmp_max - 1;
        }

        if (lflag)
            if (cnt[i] == '\n')
                lineas++;

        // No estoy seguro si es hasta -65.
        if (mflag)
            if (cnt[i] >= -65 && cnt[i] <= 127)
                chars++;
        /* TO-DO: Agregar wflag */
    }

    if (lflag)
        printf("%d ", lineas);
    if (wflag)
        printf("%d ", words);
    if (cflag)
        printf("%d ", n_bytes);
    if (mflag)
        printf("%d ", chars);
    if (mlflag)
        printf("%d ", max_line);

    printf("%s\n", argv);

    fclose(file);
    return 0;
}

static void usage(void)
{
    printf("Modo de empleo: wc [OPCIÓN]... [FICHERO]...\n\n"
           "Sin FICHERO, o cuando FICHERO es -, lee la entrada estándar.\n\n"
           "Las siguientes opciones pueden utilizarse para seleccionar cuáles cuentas se imprimen,"
           "siempre en el siguiente orden: newline, word, character, byte, maximum line length.\n"
           "  -c, --bytes            Imprime el número de bytes\n"
           "  -m, --chars            Imprime el número de caracteres\n"
           "  -l, --lines            Imprime el número de líneas\n"
           "  -L, --max-line-length  Imprime el tamaño máximo de línea\n"
           // "  -w, --words            Imprime el número de palabras\n\n"
           "      --help             Muestra ésta ayuda y finaliza\n"
           "      --version          Informa de la versión y finaliza\n\n"

           "ayuda en línea sobre AmayaOS CoreUtils: <https://wiki.amayaos.com/index.php/AmayaCoreutils>\n"
           "Informe de errores de traducción en wc a <alvarostagg@openmailbox.org>\n"
           "Documentación completa en: <https://wiki.amayaos.com/index.php/Wc>\n");
}

static void version(void)
{
    printf("wc (AmayaOS CodeUtils) %s\n"
           "Copyright © 2017 AmayaOS Team.\n"
           "Licencia GPLv3+: GPL de GNU versión 3 o posterior\n"
           "<http://gnu.org/licenses/gpl.html>.\n"
           "Esto es software libre: usted es libre de cambiarlo y redistribuirlo.\n"
           "No hay NINGUNA GARANTÍA, hasta donde permite la ley.\n\n", VERSION);
}

static size_t get_file_size(const char* fileName)
{
    struct stat st;
    if (stat(fileName, &st) < 0)
        return -1;
    else
        return st.st_size;
}
