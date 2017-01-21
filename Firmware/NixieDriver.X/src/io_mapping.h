/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2014 Sergey Sharybin <sergey@blender.org>
 * All rights reserved.
 *
 * Contributor(s): none yet.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#ifndef __IO_MAPPING_H__
#define __IO_MAPPING_H__

/* We've optimized the routes on PCB, so now we suffer in the firmware by
 * writing to the individual bits
 */

/* Decimal code which is passed to the digital decoder. */
// RIP one of the IO pins...
#define BIN_CODE_0 PORTBbits.RB1
//#define BIN_CODE_0 PORTAbits.RA5
#define BIN_CODE_1 PORTCbits.RC7
#define BIN_CODE_2 PORTCbits.RC6
#define BIN_CODE_3 PORTBbits.RB0

#define BIN_CODE_SEND_CODE(a, b, c, d) { \
    BIN_CODE_0 = a; \
    BIN_CODE_1 = b; \
    BIN_CODE_2 = c; \
    BIN_CODE_3 = d; \
  } (void) 0

#define BIN_CODE_SEND_DIGIT(digit) { \
    BIN_CODE_SEND_CODE(digit & 1, \
                       (digit >> 1) & 1, \
                       (digit >> 2) & 1, \
                       (digit >> 3) & 1);\
  } (void) 0

#define BIN_CODE_SEND_CLOSE() \
  BIN_CODE_SEND_CODE(1, 1, 1, 1)

/* Digit number, for multiplexing, defines which transistor gate is open. */
// RIP one more of the IO pins...
#define DIG_GATE_0 PORTBbits.RB2
//#define DIG_GATE_0 PORTBbits.RB5
#define DIG_GATE_1 PORTBbits.RB3
#define DIG_GATE_2 PORTBbits.RB4

#define DIG_GATE_OPEN(gate) DIG_GATE_ ## gate = 1
#define DIG_GATE_CLOSE(gate) DIG_GATE_ ## gate = 0

#endif  /* __IO_MAPPING_H__ */
