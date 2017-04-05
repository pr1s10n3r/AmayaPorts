#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#define VERSION "0.1"

static void usage(void);
static void version(void);

static int read(int, char*);
static size_t get_file_size(const char*);

static int hflag, vflag;
        /* bytes, chars, lines, words */
static int cflag, mflag, lflag, wflag;

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
    if (cflag + mflag + lflag + wflag == 0)
        lflag = wflag = cflag = 1;

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

    unsigned char cnt[get_file_size(argv)];
    int n_bytes = fread(cnt, 1, sizeof(cnt), file);

    int lineas = 0, chars = 0, words = 0;
    for (unsigned int i = 0; i < n_bytes; i++)
    {
        if (lflag)
            if (cnt[i] == '\n')
                lineas++;
        /* TO-DO: Agregar wflag y mflag */
    }

    if (lflag)
        printf(" %d ", lineas);
    if (wflag)
        printf(" %d ", words); /* TO-DO */
    if (cflag)
        printf(" %d ", n_bytes);
    if (mflag)
        printf(" %d ", chars);

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
           "  -c, --bytes    Imprime el número de bytes\n"
           "  -m, --chars    Imprime el número de caracteres\n"
           "  -l, --lines    Imprime el número de líneas\n"
           "  -w, --words    Imprime el número de palabras\n\n"
           "      --help     Muestra ésta ayuda y finaliza\n"
           "      --version  Informa de la versión y finaliza\n\n"

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
