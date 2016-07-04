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
#include <sys/stat.h>
#include <math.h>

#define VERSION "6.0"

static void usage(void);
static void version(void);
static void error_message(char* errorMessage, char* extArg, char* cMsg);

static int binario_a_decimal(char* n_binario);
static char* decimal_a_binario(int n_decimal);

static void base64_encode(char* word);
static void base64_decode(char* word);

static int hflag = 0, vflag = 0;
static int flags[2];
static int is_a_file(char* fileName);
static int error_exit = 0;

static int FileChecked = 0;

int main(int argc, char* argv[])
{
  /* printf("%s\n", decimal_a_binario(77)); */
  printf("-> %d\n", binario_a_decimal("10011"));
  
  for (int i = 0; i < 2; i++)
    flags[i] = 0;
  
  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--decode") == 0)
      flags[0] = 1;
    else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--ignore-garbage") == 0)
      flags[1] = 1;
    else if (strcmp(argv[i], "--help") == 0)
    {
      hflag = 1;
      usage();
      break;
    }
    else if (strcmp(argv[i], "--version") == 0)
    {
      vflag = 1;
      version();
      break;
    }
  }
  
  if (hflag || vflag)
    return 0;
  else /* Busca errores de sintaxis */
  {
    for (int i = 1; i < argc; i++)
    {
      if (is_a_file(argv[i]) && !FileChecked)
	  FileChecked = 1;
      else
      {
	error_message("operando extra «", argv[i], "»");
	error_exit = 1;
	break;
      }
    }  
  }

  if (error_exit)
    return -1;
  else /* Sin errores de sintaxis */
  {
    /* Check flags */
    if (flags[0])
    {

    }
    else
    {
      
    }
  }
  
  return 0;
}

static void base64_encode(char* word)
{

}

static void base64_decode(char* word)
{

}

static void error_message(char* errorMessage, char* extArg, char* cMsg)
{
  if (extArg == NULL)
    extArg = "";
  if (cMsg == NULL)
    cMsg = "";
  
  printf("base64: %s%s%s\n", errorMessage, extArg, cMsg);
  printf("Pruebe 'base64 --help' para más información.\n");
}

static void usage(void)
{
  printf("Usage: base64 [OPCIÓN]... [FICHERO]\n"
	 "Base64 codifica o descodifica FICHERO, o standard input, a standard output.\n\n"
	 "Sin FICHERO, o cuando FICHERO es -, lee la entrada estándar.\n\n"
	 "Los argumentos obligatorios para las opciones largas son también obligatorios para las opciones cortas.\n"
	 "  -d, --decode          descodifica datos\n"
	 "  -i, --ignore-garbage  al descodificar, descarta los caracteres no alfabéticos\n"
	 "      --help     muestra esta ayuda y finaliza\n"
	 "      --version  informa de la versión y finaliza\n");
}

static void version(void)
{
  printf("base64 (AmayaOS CoreUtils) %s\n"
	 "Copyright © 2016 AmayaOS, Inc.\n"
	 "Licencia GPLv3+: GPL de GNU versión 3 o posterior\n"
	 "<http://gnu.org/licenses/gpl.html>.\n"
	 "Esto es software libre: usted es libre de cambiarlo y redistribuirlo.\n"
	 "No hay NINGUNA GARANTÍA, hasta donde permite la ley.\n\n"
	 "Port a AmayaOS por Alvaro Stagg.\n", VERSION);
}

/* 0 = FALSE | 1 = TRUE */
static int is_a_file(char* fileName)
{
  struct stat st;

  if (stat(fileName, &st) < 0)
    return 0;
  else
    return 1;
}

static int binario_a_decimal(char* n_binario)
{
  int decimal = 1;
  char _table[11];

  for (int i = 0; i <= strlen(n_binario); i++)
  {
    if (n_binario[i] == '1')
    {
      _table[i] = '1';
      decimal ^= 2;
    }
    else
      _table[i] = '0';
  }
  
  return decimal;
}

static char* decimal_a_binario(int n_decimal)
{
  char binario[8];
  int i = 1, j = 1, n_binario_2 = n_decimal, rts;
  
  while (i <= n_decimal)
  {
    n_binario_2 /= 2;

    if (n_binario_2 == 1)
      break;

    i++;
  }
  
  while (j <= i)
  {
    rts = n_decimal % 2;

    if (rts == 1)
      binario[j] = '1';
    else if (rts == 0)
      binario[j] = '0';
    
    n_decimal /= 2;
    j++;
  }

  return binario;
}
