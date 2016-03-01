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

#define VERSION "v0.5.8"

static void usage(void);
static void version(void);
static int flags[3] = {0, 0, 0};

int main(int argc, char *argv[])
{ 
  if (argc < 2)
    flags[2] = 1;

  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "--help") == 0)
    {
      usage();
      break;
    }
    else if (strcmp(argv[i], "--version") == 0)
    {
      version();
      break;
     }
   }

   if (flags[0] || flags[1])
     return 0;
   else
   {
     while (1)
     {
       if (flags[2])
	 printf("y");
       else
	 for (int i = 1; i < argc; i++)
	   printf("%s ", argv[i]);
	
       printf("\n");
     }
   }
  
  return 0;
}

static void usage(void)
{
  flags[0] = 1;
  printf("Modo de empleo: yes [CADENA]...\n"
	 "       o bien: yes OPCIÓN\n");
  printf("Muestra repetidamente una línea con todas las CADENA(s) especificadas, o 'y'.\n\n");

  printf("      --help     muestra esta ayuda y finaliza.\n");
  printf("      --version  informa de la versión y finaliza.\n\n");

  printf("AmayaOS Coreutils v0.5.8 (C) 2016 AmayaOS Team & Others\n"
	 "Licencia GNU GPL v3 <http://www.gnu.org/licenses/>.\n"
	 "Reportar errores a través de http://bugs.amayaos.com o amaya@amayaos.com.\n");
}

static void version(void)
{
  flags[1] = 1;
  printf("yes (AmayaOS CoreUtils) %s\n", VERSION);
  printf("Copyright © 2016 AmayaOS, Inc.\n"
	 "Licencia GPLv3+: GPL de GNU versión 3 o posterior\n"
	 "<http://gnu.org/licenses/gpl.html>.\n"
	 "Esto es software libre: usted es libre de cambiarlo y redistribuirlo.\n"
	 "No hay NINGUNA GARANTÍA, hasta donde permite la ley.\n\n");
  printf("Escrito por Alvaro Stagg.\n");
}

