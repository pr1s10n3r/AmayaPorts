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

#include "stdlib.h"

/* 
 * A_ = ASCII 
 * 
 * https://www.ibm.com/support/knowledgecenter/SSLTBW_2.1.0/com.ibm.zos.v2r1.bpxbd00/ra64l.htm
 *
 */
#define A_DOT   46
#define A_SLASH 47
#define A0      48
#define AA      64
#define Aa      97

long a64l(const char* s)
{
  int digit, value = 0;
  long shift = 0;
  
  for (int i = 0; *s != '\0' && i < 6; i++, s++)
  {
    if (*s <= A_SLASH)
      digit = *s - A_SLASH + 1; // "+ 1" Porque siempre devolverá un bit menor
    else if (*s <= A0 + 9)      // "+ 9" Porque son los números del 0 al 9
      digit = *s - A0 + 2;
    else if (*s <= AA + 25)     // "+ 25" 25 letras del abecedario...
      digit = *s - AA + 12;
    else                        // Las letras minisculas
      digit = *s - Aa + 38;

    value |= digit << shift;    // Siempre movemos 6 bits a la izquierda (i = 0 -> shit = 0)
    shift += 6;
  }

  return value;
}
