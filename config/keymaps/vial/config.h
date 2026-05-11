/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
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

#ifdef VIA_ENABLE
/* VIA configuration. */
#define DYNAMIC_KEYMAP_LAYER_COUNT 10
#define DYNAMIC_KEYMAP_MACRO_COUNT 32
#ifdef VIAL_ENABLE
/** Vial-specific configuration. */

#define VIAL_KEYBOARD_UID { 0x6D, 0xA5, 0xCD, 0x8D, 0xC7, 0x3D, 0x7B, 0xA8 }
#endif // VIAL_ENABLE
#endif     // VIA_ENABLE

/* Charybdis-specific features. */

#ifdef POINTING_DEVICE_ENABLE
// Automatically enable the pointer layer when moving the trackball.  See also:
// - `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS`
// - `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD`
 #define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
// Rotation angle. Only supports -127 to 127.
#undef ROTATIONAL_TRANSFORM_ANGLE
#define ROTATIONAL_TRANSFORM_ANGLE 90
// Swap x,y data
//#define POINTING_DEVICE_ROTATION_90
//#define POINTING_DEVICE_ROTATION_90_RIGHT

// Double trackball build.
//#undef POINTING_DEVICE_RIGHT
//#define POINTING_DEVICE_LEFT
// Invert Y axis data
#define POINTING_DEVICE_INVERT_Y
#undef POINTING_DEVICE_INVERT_X

// To reverse horizontal scroll, define CHARYBDIS_DRAGSCROLL_REVERSE_X:
//#define CHARYBDIS_DRAGSCROLL_REVERSE_X
// To reverse vertical scroll (macOS “natural” scrolling), define CHARYBDIS_DRAGSCROLL_REVERSE_Y
#define CHARYBDIS_DRAGSCROLL_REVERSE_Y


// ============================================================
// Pointer DPI設定 (Keyball 44参考)
//
// このファイルは config/keymaps/vial/config.h に追記してください
// (既存のconfig.hの末尾、最後の #endif の前に追加)
//
// 反映方法:
//   1. ファームウェアを書き込み
//   2. EE_CLRキーで一度EEPROMをクリア (Vialから一時的に割当て)
//   3. キーボード再起動
// ============================================================

// === Default DPI (通常モード) ===
// 1200から開始、DPI_MODで200ずつ増加
// 1200 → 1400 → 1600 → 1800 → 2000 → 2200 → 2400 → ...
#undef  CHARYBDIS_MINIMUM_DEFAULT_DPI
#define CHARYBDIS_MINIMUM_DEFAULT_DPI 1200
#undef  CHARYBDIS_DEFAULT_DPI_CONFIG_STEP
#define CHARYBDIS_DEFAULT_DPI_CONFIG_STEP 200

// === Sniping DPI (精密モード - SNIPINGキーホールド時) ===
// 400から開始、S_D_MODで100ずつ増加
// 400 → 500 → 600 → 700 → 800 (Keyball44の精密値相当) → ...
#undef  CHARYBDIS_MINIMUM_SNIPING_DPI
#define CHARYBDIS_MINIMUM_SNIPING_DPI 400
#undef  CHARYBDIS_SNIPING_DPI_CONFIG_STEP
#define CHARYBDIS_SNIPING_DPI_CONFIG_STEP 100

// === Dragscroll DPI (DRGSCRLキーホールド時のスクロール速度) ===
// 値が小さいほどスクロールが繊細になる
#undef  CHARYBDIS_MINIMUM_DRAGSCROLL_DPI
#define CHARYBDIS_MINIMUM_DRAGSCROLL_DPI 40
#undef  CHARYBDIS_DRAGSCROLL_DPI_CONFIG_STEP
#define CHARYBDIS_DRAGSCROLL_DPI_CONFIG_STEP 10
// 参考値:
//   30  : かなり遅い (細かい調整向け)
//   40  : 推奨 (ゆっくり目)
//   60  : 中速
//   100 : デフォルト (元の値)

// ============================================================
// RGB Matrix - 波紋(Splash)エフェクト有効化
//
// このファイルは config/keymaps/vial/config.h に追記してください
// (前回のDPI設定と同じファイル、最後の #endif の前に追加)
// ============================================================

// キー反応(reactive)系エフェクトの有効化
// これがないと Splash 等が動かないので必須
#define RGB_MATRIX_KEYREACTIVE_ENABLED

// 波紋エフェクト4種を有効化 (お好みで取捨選択可能)
#define ENABLE_RGB_MATRIX_SPLASH              // 色相付き波紋 (単発)
#define ENABLE_RGB_MATRIX_MULTISPLASH         // 色相付き波紋 (連発) ★おすすめ
#define ENABLE_RGB_MATRIX_SOLID_SPLASH        // 単色波紋 (単発)
#define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH   // 単色波紋 (連発)

// 参考: 他にも便利な反応系エフェクトがあります
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE    // 押したキーだけ光る
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE           // 押したキーが現在色で光る
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE      // 押したキーと周辺が光る
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE // ↑の複数同時版
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS     // 押したキーから十字に光る
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS     // 押したキーから星型に光る

// ============================================================
// 起動時のデフォルトを Multisplash にしたい場合、keymap.c の
// keyboard_post_init_user() に以下を追加:
//
//   rgb_matrix_mode_noeeprom(RGB_MATRIX_MULTISPLASH);
//
// EEPROM保存版にしたい場合は noeeprom なしで:
// rgb_matrix_mode(RGB_MATRIX_MULTISPLASH);
// ============================================================

#endif  // POINTING_DEVICE_ENABLE
