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
#include <stdlib.h>
#include <math.h>

#define VERSION "v6.0"

static void usage(const char* msg, const char* argv_o, const char* msg_c);
static void help(void);
static void version(void);
static void seq(int argc, char* argv[], int flags[3]);

static int flags[3];
static int hflag = 0, vflag = 0;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		usage("falta un operando", NULL, NULL);
		return 0;
	}

	for (int i = 0; i < 3; i++)
		flags[i] = 0; /* Falso todo */

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--help") == 0)
		{
			help();
			hflag = 1;
			break;
		}
		else if (strcmp(argv[i], "--version") == 0)
		{
			version();
			vflag = 1;
			break;
		}
		else if ((strcmp(argv[i], "-w") == 0) || (strcmp(argv[i], "--equal-width") == 0))
		{
			flags[0] = 1;
			continue;
		}
		/* TODO: Agregar las opciones, pensando la forma de cómo... */
	}
	
	if (argc > 5 && (!flags[0] || !flags[1] || !flags[2]))
	{
		usage("operando extra «", argv[argc - 1], "»");
		return 0;
	}

	if (hflag || vflag)
		return 0;
	else
		seq(argc, argv, flags);

	return 0;
}

static void seq(int argc, char* argv[], int flags[3])
{
	int first       = 1;                                    // default
	int last;
	char* separator = "\n";                                 // default, TODO: Agregar la opción "-s".
	int incremento  = 1;                                    // default
	int fCustom     = 0, lCustom     = 0, iCustom     = 0;  // first Custom -> False | last Custom -> False | incremento Custom -> False

	// buscar valores customizados
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--help") == 0)
			continue;
		else if (strcmp(argv[i], "--version") == 0)
			continue;
		else if ((strcmp(argv[i], "-w") == 0) || (strcmp(argv[i], "--equal-width") == 0))
			continue;
		else
		{
			/* FIRST */
			if (!fCustom)
			{
				if (argc > 2 && (!flags[0] || !flags[1] || !flags[2]) && argv[i + 1] != NULL)
				{
					first   = atoi(argv[i]);
					fCustom = 1;
					continue;
				}
			}

			/* INCREMENTO */
			if (!iCustom && argc >= 3)
			{
				if (!(flags[0] && flags[1] && flags[2]) && (argv[i - 1] != NULL && argv[i + 1] != NULL))
				{
					incremento = atoi(argv[i]);
					iCustom    = 1;	
					continue;
				}
			}

			/* LAST */
			if (!lCustom)
			{
				if (argc > 2)
				{
					last = atoi(argv[argc - 1]);
					lCustom = 1;
					continue;
				}
				else
				{
					last = atoi(argv[i]);
					lCustom = 1;
					continue;
				}
			}
		}
	}

	if (last < first)
		return;

	char* zeros = ""; // default
	size_t zeros_len;

	if (flags[0])
	{
		if (last)
		{
			zeros_len = floor(log10(abs((double)last)) + 1);
			zeros = new char[zeros_len + 2];
	
			for (int i = 0; i <= zeros_len; i++)
				zeros[i] = '0';
				
			zeros[zeros_len] = '\0';
		}
	}

	while (first <= last)
	{	
		if (flags[0])
		{
			int n_len = floor(log10(abs((double)first)) + 1);
			
			printf("n_len -> %d | zeros_len -> %d | ", n_len, zeros_len);
			
			if (zeros_len == n_len)
				zeros = "";
			else if (n_len < zeros_len)
			{
				int n = sizeof(zeros) / sizeof(zeros)[0];
				
				for (int i = n; i >= zeros_len - n_len; i--)
					zeros[i] = '\0';
			}
		}
		
		printf("%s%d%s", zeros, first, separator);
		first += incremento;
	}
}

static void usage(const char* msg, const char* argv_o, const char* msg_c)
{
	if (argv_o == NULL)
		argv_o = "";
	if (msg_c == NULL)
		msg_c = "";

	printf("seq: %s%s%s\n"
	       "Pruebe 'seq --help' para más información.\n", msg, argv_o, msg_c);
}

static void help(void)
{
	printf("Modo de empleo: seq [OPCIÓN]... ÚLTIMO\n"
	       "        o bien: seq [OPCIÓN]... PRIMERO ÚLTIMO\n"
	       "        o bien: seq [OPCIÓN]... PRIMERO INCREMENTO ÚLTIMO\n"
	       "Muestra los números desde PRIMERO hasta ÚLTIMO, en incrementos de INCREMENTO.\n\n"
	       "Los argumentos obligatorios para las opciones largas son también obligatorios para las opciones cortas.\n"
	       "  -w, --equal-width        iguala el ancho rellenando con ceros\n"
	       "      --help     muestra esta ayuda y finaliza\n"
	       "      --version  informa de la versión y finaliza\n\n");
	printf("ayuda en línea sobre Amaya CoreUtils: <https://wiki.amayaos.com/index.php/AmayaCoreutils/>\n");
}

static void version(void)
{
	printf("seq (AmayaOS CoreUtils) %s\n"
	       "Copyright © 2016 AmayaOS, Inc.\n"
	       "Licencia GPLv3+: GPL de GNU versión 3 o posterior\n"
	       "<http://gnu.org/licenses/gpl.html>.\n"
	       "Esto es software libre: usted es libre de cambiarlo y redistribuirlo.\n"
	       "No hay NINGUNA GARANTÍA, hasta donde permite la ley.\n\n"
	       "Portado a AmayaOS por Alvaro Stagg.\n", VERSION);
}
