/*
 * Copyright 2020 Christopher Courtney <drashna@live.com> (@drashna)
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

#include "qmk_config_keyball39.h"

// clang-format off
#ifdef RGB_MATRIX_ENABLE
/**
 * \brief LEDs index.
     0     1   2   3   4  5                6      7    8   9   10  11
 *   0   17  34  51  68  85              102    119  136 153  170 187
 * ╭────────────────────────╮                 ╭────────────────────────╮
 *    0   7   8  15  16                         34    33  28  27  21       0   0
 * ├────────────────────────┤                 ├─────  ───────────────────┤
 *    1   6   9  14  17                         35    32  29  26  22       13   1
 * ├────────────────────────┤                 ├─────  ───────────────────┤
 *    2   5   10  13  18                        36    31  30  25  23       26   2
 * ├────────────────────────┤                 ├────────────────────────┤
 *    3   4   11  12  19                        37                24       39  3
 * ╰─────────────────────20╯              38╰────────────────────────╯      52
*
 *
 * Note: the LED config simulates 58 LEDs instead of the actual 56 to prevent
 * confusion when testing LEDs during assembly when handedness is not set
 * correctly.  Those fake LEDs are bound to the physical bottom-left corner.
 */
led_config_t g_led_config = { {
    /* Key Matrix to LED index. */
    // Left split.
    {      0,      7,     8,     15,     16,      NO_LED}, // Num row
    {      1,      6,     9,     14,     17,     NO_LED }, // Top row
    {      2,      5,     10,     13,     18,     NO_LED }, // Middle row
    {      3,      4,     11,     12,     19,     20}, // Bottom row
    // Right split.
    {     21,     27,       28,         32,         34,    NO_LED }, // Num row
    {     22,     26,       29,         32,         35,    NO_LED }, // Top row
    {     23,     25,       30,         31,         36,    NO_LED }, // Middle row
    {     24,     NO_LED,   NO_LED,     NO_LED,     37,    38  }, // Bottom row
}, {
    /* LED index to physical position. */
    // Left split.
    /* index=0  */ {   0,   0 },   {   0,  13 },   {   0,  26 },   {   0,  39 },  // col 1 (left most)
    /* index=4  */ {  17,  39 },   {  17,  26 },   {  17,   13 },  {   17,  0 }, // col 2
    /* index=8  */ {  34,   0 },   {  34,  13 },   {  34,  26 },   {  34,  39 },
    /* index=12 */ {  51,  39 },   {  51,  26 },   {  51,   13 },  {   51,  0 },
    /* index=16 */ {  68,   0 },   {  68,  13 },   {  68,  26 },   {  68,  39 },
    /* index=20 */ {  85,   52 },
    // Right split.
    /* index=37 */ { 187,   0 },   { 187,  13 },    { 187,  26 },   { 187,  39 },
    /* index=41 */ { 170,  26 },    { 170,  13 },   { 170,   0 },
    /* index=45 */ { 153,   0 },   { 153,  13 },    { 153,  26 },
    /* index=49 */ { 136,  26 },   {   136,  13 },{ 136,   0 },
                   { 119,   0 },  { 119,   13 }, { 119,   26 },{ 119,   39 },
                   { 102,   52 }

}, {
    /* LED index to flag. */
    // Left split.
    /* index=0  */ LED_FLAG_MODIFIER, LED_FLAG_MODIFIER, LED_FLAG_MODIFIER, LED_FLAG_MODIFIER, // col 1
    /* index=4  */ LED_FLAG_MODIFIER, LED_FLAG_MODIFIER, LED_FLAG_MODIFIER, LED_FLAG_MODIFIER, // col 2
    /* index=8  */ LED_FLAG_MODIFIER, LED_FLAG_MODIFIER, LED_FLAG_MODIFIER, LED_FLAG_MODIFIER,
    /* index=16 */ LED_FLAG_MODIFIER, LED_FLAG_MODIFIER, LED_FLAG_MODIFIER, LED_FLAG_MODIFIER,
    /* index=20 */ LED_FLAG_MODIFIER, LED_FLAG_MODIFIER, LED_FLAG_MODIFIER, LED_FLAG_MODIFIER,
    /* index=24 */ LED_FLAG_MODIFIER,
    // Right split.
    /* index=29 */ LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, // col 12
    /* index=33 */ LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,  // col 11
    /* index=37 */ LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
    /* index=41 */ LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,
    /* index=45 */ LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT, LED_FLAG_KEYLIGHT,  LED_FLAG_KEYLIGHT,
    /* index=49 */ LED_FLAG_KEYLIGHT
} };
#endif
// clang-format on



#include "transactions.h"
#include <string.h>

#ifdef CONSOLE_ENABLE
#    include "print.h"
#endif // CONSOLE_ENABLE

#ifdef POINTING_DEVICE_ENABLE
#    ifndef CHARYBDIS_MINIMUM_DEFAULT_DPI
#        define CHARYBDIS_MINIMUM_DEFAULT_DPI 400
#    endif // CHARYBDIS_MINIMUM_DEFAULT_DPI

#    ifndef CHARYBDIS_DEFAULT_DPI_CONFIG_STEP
#        define CHARYBDIS_DEFAULT_DPI_CONFIG_STEP 200
#    endif // CHARYBDIS_DEFAULT_DPI_CONFIG_STEP

#    ifndef CHARYBDIS_MINIMUM_SNIPING_DPI
#        define CHARYBDIS_MINIMUM_SNIPING_DPI 200
#    endif // CHARYBDIS_MINIMUM_SNIPING_DPI

#    ifndef CHARYBDIS_SNIPING_DPI_CONFIG_STEP
#        define CHARYBDIS_SNIPING_DPI_CONFIG_STEP 100
#    endif // CHARYBDIS_SNIPING_DPI_CONFIG_STEP


typedef union {
    uint32_t raw;
    struct {
        uint16_t pointer_dragscroll_dpi : 9; // 0-511
        uint8_t pointer_default_dpi : 4; // 16 steps available.
        uint8_t pointer_sniping_dpi : 2; // 4 steps available.
        bool    is_dragscroll_enabled : 1;
        bool    is_sniping_enabled : 1;
    } __attribute__((packed));
} charybdis_config_t;

static charybdis_config_t g_charybdis_config = {0};

#    ifndef CHARYBDIS_DRAGSCROLL_BUFFER_SIZE
#        define CHARYBDIS_DRAGSCROLL_BUFFER_SIZE 6
#    endif // !CHARYBDIS_DRAGSCROLL_BUFFER_SIZE



static void read_charybdis_config_from_eeprom(charybdis_config_t* config) {
    config->raw                   = eeconfig_read_kb();
    config->is_dragscroll_enabled = false;
    config->is_sniping_enabled    = false;
}


static void write_charybdis_config_to_eeprom(charybdis_config_t* config) {
    eeconfig_update_kb(config->raw);
}

/** \brief Return the current value of the pointer's default DPI. */
static uint16_t get_pointer_default_dpi(charybdis_config_t* config) {
    return (uint16_t)config->pointer_default_dpi * CHARYBDIS_DEFAULT_DPI_CONFIG_STEP + CHARYBDIS_MINIMUM_DEFAULT_DPI;
}

/** \brief Return the current value of the pointer's sniper-mode DPI. */
static uint16_t get_pointer_sniping_dpi(charybdis_config_t* config) {
    return (uint16_t)config->pointer_sniping_dpi * CHARYBDIS_SNIPING_DPI_CONFIG_STEP + CHARYBDIS_MINIMUM_SNIPING_DPI;
}


/** \brief Set the appropriate DPI for the input config. */
static void maybe_update_pointing_device_cpi(charybdis_config_t* config) {
    if (config->is_dragscroll_enabled) {
        pointing_device_set_cpi(config->pointer_dragscroll_dpi);
    } else if (config->is_sniping_enabled) {
        pointing_device_set_cpi(get_pointer_sniping_dpi(config));
    } else {
        pointing_device_set_cpi(get_pointer_default_dpi(config));
    }
}

/**
 * The increment/decrement steps are equal to CHARYBDIS_DEFAULT_DPI_CONFIG_STEP.
 */
static void step_pointer_default_dpi(charybdis_config_t* config, bool forward) {
    config->pointer_default_dpi += forward ? 1 : -1;
    maybe_update_pointing_device_cpi(config);
}
/**
 * The increment/decrement steps are equal to CHARYBDIS_SNIPING_DPI_CONFIG_STEP.
 */
static void step_pointer_sniping_dpi(charybdis_config_t* config, bool forward) {
    config->pointer_sniping_dpi += forward ? 1 : -1;
    maybe_update_pointing_device_cpi(config);
}



uint16_t charybdis_get_pointer_default_dpi(void) {
    return get_pointer_default_dpi(&g_charybdis_config);
}

uint16_t charybdis_get_pointer_sniping_dpi(void) {
    return get_pointer_sniping_dpi(&g_charybdis_config);
}

void charybdis_cycle_pointer_default_dpi_noeeprom(bool forward) {
    step_pointer_default_dpi(&g_charybdis_config, forward);
}


void charybdis_cycle_pointer_default_dpi(bool forward) {
    step_pointer_default_dpi(&g_charybdis_config, forward);
    write_charybdis_config_to_eeprom(&g_charybdis_config);
}

void charybdis_cycle_pointer_sniping_dpi(bool forward) {
    step_pointer_sniping_dpi(&g_charybdis_config, forward);
    write_charybdis_config_to_eeprom(&g_charybdis_config);
}

bool charybdis_get_pointer_sniping_enabled(void) {
    return g_charybdis_config.is_sniping_enabled;
}


void charybdis_set_pointer_sniping_enabled(bool enable) {
    g_charybdis_config.is_sniping_enabled = enable;
    maybe_update_pointing_device_cpi(&g_charybdis_config);
}

bool charybdis_get_pointer_dragscroll_enabled(void) {
    return g_charybdis_config.is_dragscroll_enabled;
}

void charybdis_set_pointer_dragscroll_enabled(bool enable) {
    g_charybdis_config.is_dragscroll_enabled = enable;
    maybe_update_pointing_device_cpi(&g_charybdis_config);
}

void charybdis_cycle_pointer_sniping_dpi_noeeprom(bool forward) {
    step_pointer_sniping_dpi(&g_charybdis_config, forward);
}
/**
 * \brief Augment the pointing device behavior.
 *
 * Implement drag-scroll.
 */
static void pointing_device_task_charybdis(report_mouse_t* mouse_report) {
    static int16_t scroll_buffer_x = 0;
    static int16_t scroll_buffer_y = 0;
    if (g_charybdis_config.is_dragscroll_enabled) {
    #ifdef CHARYBDIS_DRAGSCROLL_REVERSE_X
        scroll_buffer_x -= mouse_report->x;
    #else
        scroll_buffer_x += mouse_report->x;
    #endif // CHARYBDIS_DRAGSCROLL_REVERSE_X

    #ifdef CHARYBDIS_DRAGSCROLL_REVERSE_Y
        scroll_buffer_y -= mouse_report->y;
    #else
        scroll_buffer_y += mouse_report->y;
    #endif // CHARYBDIS_DRAGSCROLL_REVERSE_Y
        mouse_report->x = 0;
        mouse_report->y = 0;
        if (abs(scroll_buffer_x) > CHARYBDIS_DRAGSCROLL_BUFFER_SIZE) {
            mouse_report->h = scroll_buffer_x > 0 ? 1 : -1;
            scroll_buffer_x = 0;
        }
        if (abs(scroll_buffer_y) > CHARYBDIS_DRAGSCROLL_BUFFER_SIZE) {
            mouse_report->v = scroll_buffer_y > 0 ? 1 : -1;
            scroll_buffer_y = 0;
        }
    }
}

report_mouse_t pointing_device_task_kb(report_mouse_t mouse_report) {
    if (is_keyboard_master()) {
        pointing_device_task_charybdis(&mouse_report);
        mouse_report = pointing_device_task_user(mouse_report);
    }
    return mouse_report;
}

#    if defined(POINTING_DEVICE_ENABLE) && !defined(NO_CHARYBDIS_KEYCODES)
/** \brief Whether SHIFT mod is enabled. */
static bool has_shift_mod(void) {
#        ifdef NO_ACTION_ONESHOT
    return mod_config(get_mods()) & MOD_MASK_SHIFT;
#        else
    return mod_config(get_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT;
#        endif // NO_ACTION_ONESHOT
}
#    endif // POINTING_DEVICE_ENABLE && !NO_CHARYBDIS_KEYCODES

/**
 * \brief Outputs the Charybdis configuration to console.
 *
 * Prints the in-memory configuration structure to console, for debugging.
 * Includes:
 *   - raw value
 *   - drag-scroll: on/off
 *   - sniping: on/off
 *   - default DPI: internal table index/actual DPI
 *   - sniping DPI: internal table index/actual DPI
 */
#ifdef CONSOLE_ENABLE
static void debug_charybdis_config_to_console(charybdis_config_t* config) {
    dprintf("(charybdis) process_record_kb: config = {\n"
            "\traw = 0x%X,\n"
            "\t{\n"
            "\t\tis_dragscroll_enabled=%u\n"
            "\t\tis_sniping_enabled=%u\n"
            "\t\tdefault_dpi=0x%X (%u)\n"
            "\t\tsniping_dpi=0x%X (%u)\n"
            "\t}\n"
            "}\n",
            config->raw, config->is_dragscroll_enabled, config->is_sniping_enabled, config->pointer_default_dpi, get_pointer_default_dpi(config), config->pointer_sniping_dpi, get_pointer_sniping_dpi(config));
}
#endif // CONSOLE_ENABLE

bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
    if (!process_record_user(keycode, record)) {
#       ifdef CONSOLE_ENABLE
        debug_charybdis_config_to_console(&g_charybdis_config);
#       endif // CONSOLE_ENABLE
        return false;
    }
#    ifdef POINTING_DEVICE_ENABLE
#ifndef NO_CHARYBDIS_KEYCODES
    switch (keycode) {
        case POINTER_DEFAULT_DPI_FORWARD:
            if (record->event.pressed) {
                // Step backward if shifted, forward otherwise.
                charybdis_cycle_pointer_default_dpi(/* forward= */ !has_shift_mod());
            }
            break;
        case POINTER_DEFAULT_DPI_REVERSE:
            if (record->event.pressed) {
                // Step forward if shifted, backward otherwise.
                charybdis_cycle_pointer_default_dpi(/* forward= */ has_shift_mod());
            }
            break;
        case POINTER_SNIPING_DPI_FORWARD:
            if (record->event.pressed) {
                // Step backward if shifted, forward otherwise.
                charybdis_cycle_pointer_sniping_dpi(/* forward= */ !has_shift_mod());
            }
            break;
        case POINTER_SNIPING_DPI_REVERSE:
            if (record->event.pressed) {
                // Step forward if shifted, backward otherwise.
                charybdis_cycle_pointer_sniping_dpi(/* forward= */ has_shift_mod());
            }
            break;
        case SNIPING_MODE:
            charybdis_set_pointer_sniping_enabled(record->event.pressed);
            break;
        case SNIPING_MODE_TOGGLE:
            if (record->event.pressed) {
                charybdis_set_pointer_sniping_enabled(!charybdis_get_pointer_sniping_enabled());
            }
            break;


        case POINTER_DRAGSCROLL_DPI_FORWARD:
            if (record->event.pressed) {
                uint16_t new_value = g_charybdis_config.pointer_dragscroll_dpi + 100;
                g_charybdis_config.pointer_dragscroll_dpi = (new_value > 500) ? 100 : new_value;
                maybe_update_pointing_device_cpi(&g_charybdis_config);
                write_charybdis_config_to_eeprom(&g_charybdis_config);
            }
            break;
        case POINTER_DRAGSCROLL_DPI_REVERSE:
            if (record->event.pressed) {
                uint16_t new_value = g_charybdis_config.pointer_dragscroll_dpi - 100;
                g_charybdis_config.pointer_dragscroll_dpi = (new_value > 500) ? 500 : new_value;
                maybe_update_pointing_device_cpi(&g_charybdis_config);
                write_charybdis_config_to_eeprom(&g_charybdis_config);
            }
            break;
        case DRAGSCROLL_MODE:
            charybdis_set_pointer_dragscroll_enabled(record->event.pressed);
            break;
        case DRAGSCROLL_MODE_TOGGLE:
            if (record->event.pressed) {
                charybdis_set_pointer_dragscroll_enabled(!charybdis_get_pointer_dragscroll_enabled());
            }
            break;

    }
#        endif // !NO_CHARYBDIS_KEYCODES
#    endif     // POINTING_DEVICE_ENABLE
#ifdef CONSOLE_ENABLE
    if (IS_QK_KB(keycode) || IS_MOUSEKEY(keycode)) {
        debug_charybdis_config_to_console(&g_charybdis_config);
    }
#endif
    return true;
}

void eeconfig_init_kb(void) {
    g_charybdis_config.raw = 0;
    write_charybdis_config_to_eeprom(&g_charybdis_config);
    maybe_update_pointing_device_cpi(&g_charybdis_config);
    eeconfig_init_user();
}

void matrix_init_kb(void) {
    read_charybdis_config_from_eeprom(&g_charybdis_config);
    matrix_init_user();
}

#ifdef CHARYBDIS_CONFIG_SYNC
void charybdis_config_sync_handler(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer, uint8_t target2initiator_buffer_size, void* target2initiator_buffer) {
    if (initiator2target_buffer_size == sizeof(g_charybdis_config)) {
        memcpy(&g_charybdis_config, initiator2target_buffer, sizeof(g_charybdis_config));
    }
}
#    endif

void keyboard_post_init_kb(void) {
    maybe_update_pointing_device_cpi(&g_charybdis_config);
    #ifdef CHARYBDIS_CONFIG_SYNC
        transaction_register_rpc(RPC_ID_KB_CONFIG_SYNC, charybdis_config_sync_handler);
    #endif
    keyboard_post_init_user();
}

#    ifdef CHARYBDIS_CONFIG_SYNC
void housekeeping_task_kb(void) {
    if (is_keyboard_master()) {
        // Keep track of the last state, so that we can tell if we need to propagate to slave.
        static charybdis_config_t last_charybdis_config = {0};
        static uint32_t           last_sync             = 0;
        bool                      needs_sync            = false;

        // Check if the state values are different.
        if (memcmp(&g_charybdis_config, &last_charybdis_config, sizeof(g_charybdis_config))) {
            needs_sync = true;
            memcpy(&last_charybdis_config, &g_charybdis_config, sizeof(g_charybdis_config));
        }
        // Send to slave every 500ms regardless of state change.
        if (timer_elapsed32(last_sync) > 500) {
            needs_sync = true;
        }

        // Perform the sync if requested.
        if (needs_sync) {
            if (transaction_rpc_send(RPC_ID_KB_CONFIG_SYNC, sizeof(g_charybdis_config), &g_charybdis_config)) {
                last_sync = timer_read32();
            }
        }
    }
    // No need to invoke the user-specific callback, as it's been called
    // already.
}
#    endif // CHARYBDIS_CONFIG_SYNC
#endif     // POINTING_DEVICE_ENABLE


bool shutdown_kb(bool jump_to_bootloader) {
    if (!shutdown_user(jump_to_bootloader)) {
        return false;
    }

#ifdef RGB_MATRIX_ENABLE
    void rgb_matrix_update_pwm_buffers(void);
    rgb_matrix_set_color_all(RGB_RED);
    rgb_matrix_update_pwm_buffers();
#endif // RGB_MATRIX_ENABLE
    return true;
}

void keyball_oled_default(void) {
    char count_default_str[6];
    snprintf(count_default_str, sizeof(count_default_str), "%d", charybdis_get_pointer_default_dpi());
    oled_write_P(PSTR(" M-DPI  :"), false);
    oled_write_ln(count_default_str, false);
}
void keyball_oled_info(void) {
    char is_sniping_str[2];
    snprintf(is_sniping_str, sizeof(is_sniping_str), "%d", charybdis_get_pointer_sniping_enabled());
    oled_write_P(PSTR("SNP-T:"), false);
    oled_write(is_sniping_str, false);

    oled_write_P(PSTR(" SNP-DPI:"), false);
    char count_sniping_str[6];
    snprintf(count_sniping_str, sizeof(count_sniping_str), "%d", charybdis_get_pointer_sniping_dpi());
    oled_write_ln(count_sniping_str, false);

    char is_dragscroll_str[2];
    snprintf(is_dragscroll_str, sizeof(is_dragscroll_str), "%d", charybdis_get_pointer_dragscroll_enabled());
    oled_write_P(PSTR("DRG-T:"), false);
    oled_write(is_dragscroll_str, false);

    oled_write_P(PSTR(" DRG-DPI:"), false);
    char count_dragscroll_str[6];
    snprintf(count_dragscroll_str, sizeof(count_dragscroll_str), "%d", g_charybdis_config.pointer_dragscroll_dpi);
    oled_write_ln(count_dragscroll_str, false);
}
