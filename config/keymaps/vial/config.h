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


// JK同時押しの判定窓 (ms)
// 短くするとHRMと両立しやすい (TAPPING_TERM=200より十分小さく)
#define COMBO_TERM 40

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
#define CHARYBDIS_MINIMUM_DEFAULT_DPI 1600
#undef  CHARYBDIS_DEFAULT_DPI_CONFIG_STEP
#define CHARYBDIS_DEFAULT_DPI_CONFIG_STEP 200

// === Sniping DPI (精密モード - SNIPINGキーホールド時) ===
// 400から開始、S_D_MODで100ずつ増加
// 400 → 500 → 600 → 700 → 800 (Keyball44の精密値相当) → ...
#undef  CHARYBDIS_MINIMUM_SNIPING_DPI
#define CHARYBDIS_MINIMUM_SNIPING_DPI 400
#undef  CHARYBDIS_SNIPING_DPI_CONFIG_STEP
#define CHARYBDIS_SNIPING_DPI_CONFIG_STEP 100

// ============================================================
// スクロール速度をもう少し遅く・じっくりにする調整
// 
// パス: config/keymaps/vial/config.h に追記または既存定義を上書き
// ============================================================
 
// ── スクロール累積バッファ ──
// ボールの動きが BUFFER_SIZE 単位を超えたら 1 ノッチ分スクロールイベントを出す
// 大きいほど「重い・じっくり」、小さいほど「軽い・素早い」
//
// デフォルト: 6
// 推奨値:
//   10〜12 : やや遅め (おすすめスタート地点)
//   16〜20 : 明確にゆっくり (じっくり読書向け)
//   30以上 : かなり重い (繊細な調整用)
#define CHARYBDIS_DRAGSCROLL_BUFFER_SIZE 12
 
 
// ── スクロール時のセンサーDPI (補助的) ──
// 既存の設定。これも下げるとさらに遅くなる
// ただし下げすぎるとボールが「効かない」感じになる
//
// 推奨範囲: 10〜50
#undef  CHARYBDIS_MINIMUM_DRAGSCROLL_DPI
#define CHARYBDIS_MINIMUM_DRAGSCROLL_DPI 30     // 40→30 (若干下げ)

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

// ============================================================
// RGB Matrix Splash - 心地よい時間チューニング
//
// 人間の知覚閾値とHCI経験則からの逆算:
//   - 打鍵→発光開始: <50ms (意図的束縛が壊れない閾値)
//   - 波がキーボード端まで到達: 300〜600ms (≒水面波0.3〜1m/s相当)
//   - 波紋の完全減衰: 800〜1500ms (新旧波が干渉できる残光)
//   - 伝播速度: 1秒あたりキー4〜6個分 (水っぽさ)
//
// 既存の config/keymaps/vial/config.h に以下を追記してください
// (前回のSplash有効化設定の近く、最後の #endif の前)
// ============================================================

// ────────────────────────────────────────────────
// 速度 (0〜255、デフォルト127)
// scale16by8 で tick × (speed+1) / 256 が effect値になる
// 速度が低いほど波の伝播がゆっくり = 水っぽくなる
// ────────────────────────────────────────────────
#undef  RGB_MATRIX_DEFAULT_SPD
#define RGB_MATRIX_DEFAULT_SPD 64       // 50〜70 推奨 (水紋ライク)
                                        // 32  : スローモー水中
                                        // 64  : ★ ゆったり水面 (おすすめ)
                                        // 96  : やや遅い
                                        // 127 : QMKデフォルト
                                        // 200 : 電気的にガチャガチャ

// ────────────────────────────────────────────────
// LED更新レート上限 (1フレームあたりの最小時間 ms)
// 60Hz相当=16ms以下推奨。RP2040なら8msでも回せる
// ────────────────────────────────────────────────
#undef  RGB_MATRIX_LED_FLUSH_LIMIT
#define RGB_MATRIX_LED_FLUSH_LIMIT 16   // 約60Hz更新 (打鍵→発光<50msを保証)

#undef  RGB_MATRIX_LED_PROCESS_LIMIT
#define RGB_MATRIX_LED_PROCESS_LIMIT (RGB_MATRIX_LED_COUNT + 4) / 5

// ────────────────────────────────────────────────
// ヒット履歴の保持時間 (ms)
// 連打時に新旧の波が干渉して模様を作るために
// 波紋の完全減衰時間と同等に設定
// (Splashエフェクト内部で使われる reactive timeout)
// ────────────────────────────────────────────────
#undef  RGB_MATRIX_KEYREACTIVE_TIMEOUT
#define RGB_MATRIX_KEYREACTIVE_TIMEOUT 1200  // 800〜1500ms 推奨

// ────────────────────────────────────────────────
// デフォルト色相 / 彩度 / 明度
// 水っぽさは寒色 + やや低めの明度が映える
// ────────────────────────────────────────────────
#undef  RGB_MATRIX_DEFAULT_HUE
#define RGB_MATRIX_DEFAULT_HUE 170      // 青〜シアン (170≒シアン、128≒水色)

#undef  RGB_MATRIX_DEFAULT_SAT
#define RGB_MATRIX_DEFAULT_SAT 220      // やや彩度落とすと水っぽい

#undef  RGB_MATRIX_DEFAULT_VAL
#define RGB_MATRIX_DEFAULT_VAL 180      // 中明度 (上げすぎると蛍光的)

// ────────────────────────────────────────────────
// RGB全体の最大消費電力制限 (mA、USB 2.0 == 500mA)
// 高密度LEDで全点灯すると電源が落ちる可能性があるので念のため
// ────────────────────────────────────────────────
#undef  RGB_MATRIX_MAXIMUM_BRIGHTNESS
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 200   // 0〜255、255は眩しすぎ&電力消費大

// ============================================================
// 反映方法:
//   1. config.h に上記を追記してビルド・書き込み
//   2. Layer3 → EE_CLR (Uキー位置) を1回押してEEPROM初期化
//      (これがないと前回の保存値が優先されて見た目変わらない)
//   3. キーボードを再接続
//   4. Layer3 → RGB_MSP (Uキー位置) または起動時の自動設定で
//      MULTISPLASHモードに入って打鍵してみる
//
// 微調整のコツ:
//   - もっとゆっくりにしたい → RGB_MATRIX_DEFAULT_SPD を 48 → 32 へ
//   - 残光をもっと長く     → RGB_MATRIX_KEYREACTIVE_TIMEOUT を 1500 へ
//   - 暗いと感じる         → RGB_MATRIX_DEFAULT_VAL を 220 へ
//   - 色を変えたい         → RGB_MATRIX_DEFAULT_HUE
//                            0=赤, 43=黄, 85=緑, 128=シアン, 170=青, 213=マゼンタ
// ============================================================


#endif  // POINTING_DEVICE_ENABLE
