/*
 * Copyright (C) 2012 Felipe Cabrera, 2015 Dan Rulos, 2016 Alvaro Stagg [alvarostagg@openmailbox.org]
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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIA     86400
#define HORA    3600
#define MINUTO  60

static void usage(void);
static void version(void);
static int hflag = 0, vflag = 0;

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("sleep: falta un operando\n"
               "Pruebe 'sleep --help' para más información.\n");
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++)
    {
      if (strcmp(argv[i], "--help") == 0)
      {
        usage();
        hflag = 1;
        break;
      }
      else if (strcmp(argv[i], "--version") == 0)
      {
        version();
        vflag = 1;
        break;
      }
    }

    if (hflag || vflag)
      return EXIT_SUCCESS;
    else
    {
      switch(argv[1][strlen(argv[1]) - 1])
      {
        case 'd':
          sleep(atoi(&argv[1][0]) * DIA);
          break;
        case 'h':
          sleep(atoi(&argv[1][0]) * HORA);
          break;
        case 'm':
          sleep(atoi(&argv[1][0]) * MINUTO);
          break;
        case 's':
        default:
          sleep(atoi(&argv[1][0]));
          break;
      }
    }

    printf("\n");

    return EXIT_SUCCESS;
}

static void version(void)
{
  printf("sleep (AmayaOS CoreUtils) 0.5.9\n"
         "Copyright © 2016 AmayaOS, Inc.\n"
         "Licencia GPLv3+: GPL de GNU versión 3 o posterior\n"
         "<http://gnu.org/licenses/gpl.html>.\n"
         "Esto es software libre: usted es libre de cambiarlo y redistribuirlo.\n"
         "No hay NINGUNA GARANTÍA, hasta donde permite la ley.\n\n"
         "Escrito por Felipe Cabrera, Dan Rulos y Alvaro Stagg.\n");
}

static void usage(void)
{
  printf("Modo de empleo: sleep NÚMERO[SUFIJO]...\n"
         "        o bien: sleep OPCIÓN\n\n");
  printf("Hace una pausa de NÚMERO segundos. El SUFIJO puse ser 's' para segundos\n"
         "(predeterminado), 'm' para minutos, 'h' para horas o 'd' para días.\n\n");
  printf("      --help     muestra esta ayuda y finaliza\n"
         "      --version  informa de la versión y finaliza\n");
}
