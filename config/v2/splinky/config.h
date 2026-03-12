/*
 * Copyright 2022 Charly Delay <charly@codesink.dev> (@0xcharly)
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

/* Handedness. */
//#define MASTER_RIGHT

// To use the handedness pin, resistors need to be installed on the adapter PCB.
// If so, uncomment the following code, and undefine MASTER_RIGHT above.
 #define SPLIT_HAND_PIN GP15
 #define SPLIT_HAND_PIN_LOW_IS_LEFT  // High -> right, Low -> left.

/* VBUS detection. */
#define USB_VBUS_PIN GP19

/* SPI & PMW3360 settings. */
#define SPI_DRIVER SPID0
#define SPI_SCK_PIN GP22
#define SPI_MISO_PIN GP20
#define SPI_MOSI_PIN GP23
#define PMW33XX_CS_PIN GP16

/* Reset. */
//#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
//#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP17
//#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U
#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_TX_PIN GP1
#define SERIAL_USART_RX_PIN GP0
// TX on master must connect to RX on slave, and vice versa
// This can be swapped in software, no physical PCB change needed
#define SERIAL_USART_PIN_SWAP

#define TAP_CODE_DELAY 10

#ifdef OLED_ENABLE
/* Ensure current layer state is available on slave via QMK split transport. */
#define SPLIT_LAYER_STATE_ENABLE
/* Ensure host LED state (caps/num/scroll) is available on slave via QMK split transport. */
#define SPLIT_LED_STATE_ENABLE
/* Ensure modifier state (normal, weak, oneshot) is available on slave via QMK split transport. */
#define SPLIT_MODS_ENABLE
/* Sync OLED on/off state between halves. */
#define SPLIT_OLED_ENABLE
/* Ensure current WPM is available on slave via QMK split transport. */
#define SPLIT_WPM_ENABLE
#define OLED_FONT_H "keyboards/qmk_config_keyball39/keymaps/vial/glcdfont.c"
#define OLED_TIMEOUT 50000
#endif

// Fix slave not waking from sleep
//#define FORCED_SYNC_THROTTLE_MS 100
#define CHARYBDIS_CONFIG_SYNC
