/*
 * Copyright 2021 Quentin LEBASTARD <qlebastard@gmail.com>
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 * Copyright 2026 MochuKeeb
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
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

#pragma once

#include "quantum.h"

#ifdef POINTING_DEVICE_ENABLE
#    ifndef NO_CHARYBDIS_KEYCODES
enum charybdis_keycodes {
    POINTER_DEFAULT_DPI_FORWARD = QK_KB_0,
    POINTER_DEFAULT_DPI_REVERSE,
    POINTER_SNIPING_DPI_FORWARD,
    POINTER_SNIPING_DPI_REVERSE,
    SNIPING_MODE,
    SNIPING_MODE_TOGGLE,
    DRAGSCROLL_MODE,
    DRAGSCROLL_MODE_TOGGLE,
    POINTER_DRAGSCROLL_DPI_FORWARD,
    POINTER_DRAGSCROLL_DPI_REVERSE,
    AUTO_MODE_TOGGLE,
    AUTO_TIME_50,
    AUTO_TIME_50R,
    AUTO_TIME_100,
    AUTO_THRESHOLD,
    TInfo,
    T_SAVE,
};

#define DPI_MOD POINTER_DEFAULT_DPI_FORWARD
#define DPI_RMOD POINTER_DEFAULT_DPI_REVERSE
#define S_D_MOD POINTER_SNIPING_DPI_FORWARD
#define S_D_RMOD POINTER_SNIPING_DPI_REVERSE
#define SNIPING SNIPING_MODE
#define SNP_TOG SNIPING_MODE_TOGGLE
#define DRGSCRL DRAGSCROLL_MODE
#define DRG_TOG DRAGSCROLL_MODE_TOGGLE
#define DRG_MOD POINTER_DRAGSCROLL_DPI_FORWARD
#define DRG_RMOD POINTER_DRAGSCROLL_DPI_REVERSE
#define AUTO_TOG AUTO_MODE_TOGGLE
#endif // !NO_CHARYBDIS_KEYCODES

uint16_t charybdis_get_pointer_default_dpi(void);
void charybdis_cycle_pointer_default_dpi(bool forward);
void charybdis_cycle_pointer_default_dpi_noeeprom(bool forward);
uint16_t charybdis_get_pointer_sniping_dpi(void);
void charybdis_cycle_pointer_sniping_dpi(bool forward);
void charybdis_cycle_pointer_sniping_dpi_noeeprom(bool forward);
bool charybdis_get_pointer_sniping_enabled(void);
void charybdis_set_pointer_sniping_enabled(bool enable);
bool charybdis_get_pointer_dragscroll_enabled(void);
void charybdis_set_pointer_dragscroll_enabled(bool enable);
void keyball_oled_info(void);
void keyball_oled_default(void);
#endif // POINTING_DEVICE_ENABLE
