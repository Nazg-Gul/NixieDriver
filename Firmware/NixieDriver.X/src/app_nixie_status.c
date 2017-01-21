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

#include "app_nixie_status.h"

#include <stdint.h>
#include <system.h>
#include <usb/usb_device.h>

static unsigned char digit_0 = 0,
                     digit_1 = 0,
                     digit_2 = 0;
static unsigned char current_digit = 0;

void APP_NixieUpdateStatus(void) {
#define FALL_TIME 50
#define RAISE_AND_DUTY_TIME 10
    if (current_digit == 0) {
      DIG_GATE_CLOSE(2);
      __delay_us(FALL_TIME);
      BIN_CODE_SEND_DIGIT(digit_0);
      DIG_GATE_OPEN(0);
      current_digit = 1;
    } else if (current_digit == 1) {
      DIG_GATE_CLOSE(0);
      __delay_us(FALL_TIME);
      BIN_CODE_SEND_DIGIT(digit_1);
      DIG_GATE_OPEN(1);
      current_digit = 2;
    } else if (current_digit == 2) {
      /* We do check here to balance the ticks counter. */
      DIG_GATE_CLOSE(1);
      __delay_us(FALL_TIME);
      BIN_CODE_SEND_DIGIT(digit_2);
      DIG_GATE_OPEN(2);
      current_digit = 0;
    }
  __delay_us(RAISE_AND_DUTY_TIME);
#undef FALL_TIME
#undef RAISE_AND_DUTY_TIME
}

void APP_NixieSetDigits(unsigned char a,
                        unsigned char b,
                        unsigned char c) {
  digit_0 = a;
  digit_1 = b;
  digit_2 = c;
}