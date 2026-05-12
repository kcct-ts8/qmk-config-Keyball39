/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)
Copyright 2024 MochuKeeb

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// ============================================================
// タッピング・ホールド設定
// ============================================================
#undef  TAPPING_TERM
#define TAPPING_TERM 200
#undef  QUICK_TAP_TERM
#define QUICK_TAP_TERM 100
#undef  HOLD_ON_OTHER_KEY_PRESS
#define HOLD_ON_OTHER_KEY_PRESS

#include QMK_KEYBOARD_H

// ============================================================
// レイヤー定義 (0〜5)
//   0: Base   QWERTY + Home Row Mods + LNG2
//   1: Mouse  マウス操作・メディア (L5ホールドで起動)
//   2: Symbol 記号
//   3: RGB    RGB・各種設定
//   4: Number 数字・ファンクション
//   5: Numpad テンキー
// ============================================================
enum keyball39_layers {
    LAYER_BASE   = 0,
    LAYER_MOUSE  = 1,
    LAYER_SYMBOL = 2,
    LAYER_RGB    = 3,
    LAYER_NUMBER = 4,
    LAYER_NUMPAD = 5,
    LAYER_GAME   = 6,
};

// ============================================================
// ユーザー設定 (EEPROMに保存)
// is_oled_enabled: OLEDの表示モード切替 (TInfoキーで切替)
// ============================================================
typedef union {
    uint32_t raw1;
    struct {
        bool is_oled_enabled : 1;
    };
} user_config_t;

static user_config_t user_config;

// ============================================================
// カスタムキーコード
// ============================================================
enum my_keycodes {
    MY_DUMMY = SAFE_RANGE,
    RGB_SPL,   // Splash (波紋) モードに切替
    RGB_MSP,   // MultiSplash (連波紋) モードに切替
    USR_DRG,   // 独自ドラッグスクロール (momentary、ホールド中のみ)
    USR_DTG,   // 独自ドラッグスクロール (toggle、押すたびにON/OFF)
};

// ============================================================
// コンボ (Combo) 設定について
//
// このVialフォークではVialが key_combos[] 配列を常時定義する仕様のため、
// keymap.c で静的にコンボを定義するとリンクエラーになります。
//
// コンボの追加・編集は Vial GUI から行ってください:
//   1. Vialアプリを起動
//   2. 上部メニュー: Combos タブ
//   3. 空きスロットにキー組合せと出力キーコードを設定
//      例: KC_J + KC_K → KC_ESC
//   4. 保存 (Vialアプリ内で自動保存)
//
// 注意: Vialのコンボ設定はEEPROMに保存されるため、
// EE_CLR を実行すると消えます。その場合は再度設定が必要です。
// ============================================================

// ============================================================
// キーマップ定義
//
// v2/splinky (Charybdisベース) のLAYOUTマクロ:
//   39キー = 5列×3行×2面(30) + 親指左6+右3(9)
//
// Row 0: [Q][W][E][R][T] | [Y][U][I][O][P]
// Row 1: [A][S][D][F][G] | [H][J][K][L][;]  ← Home Row Mods
// Row 2: [Z][X][C][V][B] | [N][M][,][.][/]
// Thumb: [L1][L2][L3][L4][L5][L6] | [R1][R2][R3]
//
// 【左手Home Row Mods】A=GUI S=Alt D=Ctrl F=Shift
// 【右手Home Row Mods】J=Shift K=Ctrl L=Alt
//
// 【親指(左)】
//   L1: LT(RGB, BSPC)     タップ=BS     ホールド=Layer3(RGB)
//   L2: LT(SYM, ESC)      タップ=ESC    ホールド=Layer2(Symbol)
//   L3: TG(NUMBER)        押すたびに数字レイヤーON/OFF  ★真ん中
//   L4: LSFT_T(LNG2)      タップ=LNG2   ホールド=Shift
//   L5: LT(MOUSE, SPC)    タップ=Space  ホールド=Layer1(Mouse)
//   L6: KC_LALT           Alt
//
// 【親指(右)】
//   R1: LT(NUMPAD, MINS)  タップ=-      ホールド=Layer5(Numpad)
//   R2: LT(SYM, ENT)      タップ=Enter  ホールド=Layer2(Symbol)
//   R3: LT(RGB, LBRC)     タップ=[      ホールド=Layer3(RGB)
//
// 【精密モード/スクロール (マウスレイヤー上)】
//   SNIPING: ホールドで低DPI精密モード (Dキー)
//   DRGSCRL: ホールドでドラッグスクロール
// ============================================================
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // =========================================================
    // Layer 0: Base
    // =========================================================
    [LAYER_BASE] = LAYOUT(
        KC_Q,        KC_W,        KC_E,        KC_R,        KC_T,        KC_Y,        KC_U,        KC_I,        KC_O,        KC_P,

        ALT_T(KC_A), GUI_T(KC_S), CTL_T(KC_D), SFT_T(KC_F), KC_G,        KC_H,        SFT_T(KC_J), CTL_T(KC_K), GUI_T(KC_L), ALT_T(KC_SCLN),

        KC_Z,        KC_X,        KC_C,        KC_V,        KC_B,        KC_N,        KC_M,        KC_COMM,     KC_DOT,      KC_SLSH,

        TG(LAYER_RGB), TG(LAYER_GAME), LT(LAYER_NUMBER, KC_ESC), KC_TAB, MO(LAYER_MOUSE), LT(LAYER_NUMBER, KC_SPC),
        LT(LAYER_NUMPAD, KC_MINS), LT(LAYER_SYMBOL, KC_ENT), LT(LAYER_RGB, KC_LBRC)
    ),

    // =========================================================
    // Layer 1: Mouse / Media
    // L5ホールドで起動, L6位置のTG(MOUSE)でロック(ホールド解放後も維持)
    // SNIPING (Dキー) = 低DPI精密モード (ホールド中のみ)
    // DRGSCRL (Kキー位置) = ドラッグスクロールモード (ホールド)
    // =========================================================
    [LAYER_MOUSE] = LAYOUT(
        SGUI(KC_1), SGUI(KC_2), LCTL(SGUI(KC_3)), LCTL(SGUI(KC_4)), LCTL(KC_T),         KC_F17,  KC_F19,     KC_F18,  KC_F13,   KC_F12,

        KC_F22, _______, SNIPING, LCTL(SGUI(KC_D)), KC_F15,                             KC_BSPC, KC_MS_BTN1, USR_DRG, KC_MS_BTN2, KC_F16,

        KC_F23, _______, _______, _______, KC_F14,                                       KC_VOLD,KC_VOLU, KC_MS_BTN3, KC_MUTE, _______,

        _______, _______, _______, _______, _______, _______,        _______, _______, _______
    ),

    // =========================================================
    // Layer 2: Symbol & Arrows (Keyball 44から移植)
    //
    // 【左手】HJKLスタイル矢印 (ホームロー)
    //   A=←, S=↓, D=↑, F=→
    // 【左手内側列】
    //   T=] (Shiftで})
    //   G=\ (Shiftで|)
    // 【右手】Mac風矢印 (ホームロー)
    //   H=BS, J=←, K=↑, L=↓, ;=→
    // 【右手記号】
    //   Y=; (Shiftで:)
    //   U=_, I=&, O=@, P=^
    // 【右手数値演算】
    //   N=+, M=-, ,==, .=*
    // =========================================================
    [LAYER_SYMBOL] = LAYOUT(
        _______, _______, _______, _______, KC_RBRC,        KC_SCLN, S(KC_INT1), S(KC_7), S(KC_2), S(KC_6),

        KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, KC_BSLS,       KC_BSPC, KC_LEFT,    KC_UP,   KC_DOWN, KC_RIGHT,

        _______, _______, _______, _______, _______,        KC_PPLS, KC_PMNS,    KC_PEQL, KC_PAST, _______,

        _______, _______, _______, _______, _______, _______,        _______, _______, _______
    ),

    // =========================================================
    // Layer 3: RGB LED コントロール & 各種設定
    // Charybdisキーコード:
    //   DPI_MOD  = デフォルトDPI増加
    //   DPI_RMOD = デフォルトDPI減少
    //   S_D_MOD  = 精密(Sniping)DPI増加
    //   DRG_TOG  = ドラッグスクロールトグル
    //   T_SAVE   = ユーザー設定をEEPROMに保存
    //   EE_CLR   = EEPROM全クリア (DPI最小値変更後に1回押す)
    // =========================================================
    [LAYER_RGB] = LAYOUT(
        RGB_TOG,  RGB_M_P,  RGB_M_B,  RGB_M_R,  RGB_M_SW,        QK_BOOT,  _______,        TInfo,    _______,  T_SAVE,

        RGB_MOD,  RGB_HUI,  RGB_SAI,  RGB_VAI,  RGB_M_SN,        RGB_MSP,  S_D_MOD,        TG(LAYER_GAME),  _______,  USR_DTG,

        RGB_RMOD, RGB_HUD,  RGB_SAD,  RGB_VAD,  RGB_M_K,         DPI_RMOD, EE_CLR,         _______,  DPI_MOD,  _______,

        _______, _______, _______, _______, _______, _______,        _______, _______, _______
    ),

    // =========================================================
    // Layer 4: Number / Function
    // L3(TG(NUMBER))で出入り
    // =========================================================
    [LAYER_NUMBER] = LAYOUT(
        KC_1,  KC_2,  KC_3,  KC_4, KC_5,                  KC_6,  KC_7,  KC_8,  KC_9, KC_0,

        KC_F1, KC_F2, KC_F3, KC_F4,KC_F5,                 KC_F6, KC_F7, KC_F8, KC_F9, KC_F10,

        _______, _______, _______, _______,  _______,     _______, _______, _______, _______, _______, 

        _______, _______, _______, _______, _______, _______,      _______, _______, _______
    ),

    // =========================================================
    // Layer 5: Numpad (純粋なテンキー)
    // 右手側のみテンキー、左手側は透過 (Baseレイヤーの動作が継続)
    // =========================================================
    [LAYER_NUMPAD] = LAYOUT(
        _______, _______, _______, _______, _______,        KC_PEQL, KC_P1,   KC_P2,   KC_P3,   KC_PPLS,

        _______, _______, _______, _______, _______,        KC_PMNS, KC_P4,   KC_P5,   KC_P6,   KC_PMNS,

        _______, _______, _______, _______, _______,        KC_PAST, KC_P7,   KC_P8,   KC_P9,   KC_PSLS,

        _______, _______, _______, _______, _______, _______,   KC_P0,   KC_PDOT, _______
    ),

    // =========================================================
    // Layer 6: Game (ゲーム用 - HRMなしのQWERTY配列)
    //
    // 通常のBaseレイヤーはHomeRowMods(A/S/D/F/J/K/L/;)があるため
    // ゲーム中に "Shift+A" などが意図せず発火する問題がある
    // このレイヤーはHRMを除いた素直なQWERTY配列でゲームプレイ向け
    //
    // 入り方: Layer 3 (RGB) → Kキー位置の TG(LAYER_GAME) を押す
    // 抜け方: 左親指 L1 (TG_GAME) を押す
    //
    // 親指の割当:
    //   L1: TG(LAYER_GAME) [ゲーム終了]
    //   L2: KC_TAB         [インベントリ/メニュー]
    //   L3: KC_ESC         [一時停止/戻る]
    //   L4: KC_LSFT        [スプリント/全力疾走]
    //   L5: KC_SPC         [ジャンプ]
    //   L6: KC_LCTL        [しゃがみ]
    //   R1: KC_ENT         [決定]
    //   R2: KC_BSPC        [削除/戻る]
    //   R3: KC_LALT        [代替アクション]
    // =========================================================
    [LAYER_GAME] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,

        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,        KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,

        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,

        TG(LAYER_GAME), KC_TAB, KC_ESC, KC_LSFT, KC_SPC, KC_LCTL,        KC_ENT, KC_BSPC, KC_LALT
    ),
};
// ============================================================
// EEPROM初期化
// ============================================================
void eeconfig_init_user(void) {
    user_config.raw1            = 0;
    user_config.is_oled_enabled = 0;
    eeconfig_update_user(user_config.raw1);
}

// ============================================================
// 起動時初期化
// ============================================================
void keyboard_post_init_user(void) {
    user_config.raw1 = eeconfig_read_user();

#ifdef RGB_MATRIX_ENABLE
    // 起動時にRGBを必ずMultisplash(連波紋)で開始する
    // _noeeprom版を使うのでEEPROMには書き込まれず、
    // ユーザーがRGB_MOD等で他のモードに切り替えてもセッション中のみ有効
    rgb_matrix_mode_noeeprom(RGB_MATRIX_MULTISPLASH);
    rgb_matrix_enable_noeeprom();
#endif
}

// ============================================================
// RGB Matrix 前方宣言
// ============================================================
#ifdef RGB_MATRIX_ENABLE
void rgb_matrix_update_pwm_buffers(void);
#endif

// ============================================================
// 独自ドラッグスクロールの状態 (process_record_user で更新される)
// ============================================================
static bool user_dragscroll_held     = false;
static bool user_dragscroll_toggled  = false;

// ============================================================
// キーイベント処理
// ============================================================
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {

        // OLEDモード切替 (cat / keyball info)
        case TInfo:
            if (record->event.pressed) {
                user_config.is_oled_enabled ^= 1;
            }
            return false;

        // ユーザー設定をEEPROMに保存
        case T_SAVE:
            if (record->event.pressed) {
                eeconfig_update_user(user_config.raw1);
            }
            return false;

#ifdef RGB_MATRIX_ENABLE
        // Splash (波紋) モードに切替
        case RGB_SPL:
            if (record->event.pressed) {
                rgb_matrix_mode(RGB_MATRIX_SPLASH);
            }
            return false;

        // MultiSplash (連波紋) モードに切替
        case RGB_MSP:
            if (record->event.pressed) {
                rgb_matrix_mode(RGB_MATRIX_MULTISPLASH);
            }
            return false;
#endif

        // ========================================================
        // 独自ドラッグスクロール
        // CharybdisのDRGSCRL/DRG_TOGの代替実装
        //
        // 利点: master/slave の位置に依存せず動作する
        //   (USB左右どちらに挿してもスクロール可能)
        // ========================================================
        case USR_DRG:
            // ホールド中だけON (momentary)
            user_dragscroll_held = record->event.pressed;
            return false;

        case USR_DTG:
            // 押すたびにON/OFF (toggle)
            if (record->event.pressed) {
                user_dragscroll_toggled = !user_dragscroll_toggled;
            }
            return false;
    }
    return true;
}

// ============================================================
// ポインティングデバイス処理 (毎ボール更新で呼ばれる)
//
// USR_DRG/USR_DTGが有効なときX/Y入力をH/Vスクロールに変換する
// バッファ蓄積方式で滑らかに、CHARYBDIS_DRAGSCROLL_BUFFER_SIZE で
// スクロール速度を調整 (大きいほど遅く=重く)
// ============================================================
#ifndef CHARYBDIS_DRAGSCROLL_BUFFER_SIZE
#    define CHARYBDIS_DRAGSCROLL_BUFFER_SIZE 6
#endif

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    bool dragscroll_active = user_dragscroll_held || user_dragscroll_toggled;

    if (dragscroll_active) {
        // X/Y → H/V 変換 (累積バッファで滑らかに)
        static int16_t scroll_buffer_x = 0;
        static int16_t scroll_buffer_y = 0;

        scroll_buffer_x += mouse_report.x;
        scroll_buffer_y += mouse_report.y;

        mouse_report.h = scroll_buffer_x / CHARYBDIS_DRAGSCROLL_BUFFER_SIZE;
#ifdef CHARYBDIS_DRAGSCROLL_REVERSE_Y
        // macOSの「ナチュラルスクロール」と同じ向き
        mouse_report.v = -(scroll_buffer_y / CHARYBDIS_DRAGSCROLL_BUFFER_SIZE);
#else
        mouse_report.v = scroll_buffer_y / CHARYBDIS_DRAGSCROLL_BUFFER_SIZE;
#endif

        // 余りを保持して次回に持ち越し
        scroll_buffer_x -= mouse_report.h * CHARYBDIS_DRAGSCROLL_BUFFER_SIZE;
#ifdef CHARYBDIS_DRAGSCROLL_REVERSE_Y
        scroll_buffer_y -= -mouse_report.v * CHARYBDIS_DRAGSCROLL_BUFFER_SIZE;
#else
        scroll_buffer_y -= mouse_report.v * CHARYBDIS_DRAGSCROLL_BUFFER_SIZE;
#endif

        // X/Yはゼロにしてカーソルが動かないように
        mouse_report.x = 0;
        mouse_report.y = 0;
    }
    return mouse_report;
}


// ============================================================
// OLED表示 (Keyball 44から移植)
// ============================================================
#ifdef OLED_ENABLE


uint32_t anim_timer         = 0;
uint8_t  current_idle_frame = 0;
uint8_t  current_tap_frame  = 0;

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return rotation;
    } else {
        return OLED_ROTATION_270;
    }
}

void render_space(void) { oled_write_P(PSTR("     "), false); }

void render_logo(void) {
    oled_write_P(PSTR("Track"), false);
    oled_write_P(PSTR("ball "), false);
    oled_write_P(PSTR("39   "), false);
}

void render_mod_status_gui_alt(uint8_t modifiers) {
    static const char PROGMEM gui_off_1[] = {0x85, 0x86, 0}; static const char PROGMEM gui_off_2[] = {0xa5, 0xa6, 0};
    static const char PROGMEM gui_on_1[]  = {0x8d, 0x8e, 0}; static const char PROGMEM gui_on_2[]  = {0xad, 0xae, 0};
    static const char PROGMEM alt_off_1[] = {0x87, 0x88, 0}; static const char PROGMEM alt_off_2[] = {0xa7, 0xa8, 0};
    static const char PROGMEM alt_on_1[]  = {0x8f, 0x90, 0}; static const char PROGMEM alt_on_2[]  = {0xaf, 0xb0, 0};
    static const char PROGMEM off_off_1[] = {0xc5, 0}; static const char PROGMEM off_off_2[] = {0xc6, 0};
    static const char PROGMEM on_off_1[]  = {0xc7, 0}; static const char PROGMEM on_off_2[]  = {0xc8, 0};
    static const char PROGMEM off_on_1[]  = {0xc9, 0}; static const char PROGMEM off_on_2[]  = {0xca, 0};
    static const char PROGMEM on_on_1[]   = {0xcb, 0}; static const char PROGMEM on_on_2[]   = {0xcc, 0};
    oled_write_P((modifiers & MOD_MASK_GUI) ? gui_on_1 : gui_off_1, false);
    if      ((modifiers & MOD_MASK_GUI) && (modifiers & MOD_MASK_ALT)) oled_write_P(on_on_1, false);
    else if  (modifiers & MOD_MASK_GUI)                                 oled_write_P(on_off_1, false);
    else if  (modifiers & MOD_MASK_ALT)                                 oled_write_P(off_on_1, false);
    else                                                                 oled_write_P(off_off_1, false);
    oled_write_P((modifiers & MOD_MASK_ALT) ? alt_on_1 : alt_off_1, false);
    oled_write_P((modifiers & MOD_MASK_GUI) ? gui_on_2 : gui_off_2, false);
    if      ((modifiers & MOD_MASK_GUI) && (modifiers & MOD_MASK_ALT)) oled_write_P(on_on_2, false);
    else if  (modifiers & MOD_MASK_GUI)                                 oled_write_P(on_off_2, false);
    else if  (modifiers & MOD_MASK_ALT)                                 oled_write_P(off_on_2, false);
    else                                                                 oled_write_P(off_off_2, false);
    oled_write_P((modifiers & MOD_MASK_ALT) ? alt_on_2 : alt_off_2, false);
}

void render_mod_status_ctrl_shift(uint8_t modifiers) {
    static const char PROGMEM ctrl_off_1[]  = {0x89, 0x8a, 0}; static const char PROGMEM ctrl_off_2[]  = {0xa9, 0xaa, 0};
    static const char PROGMEM ctrl_on_1[]   = {0x91, 0x92, 0}; static const char PROGMEM ctrl_on_2[]   = {0xb1, 0xb2, 0};
    static const char PROGMEM shift_off_1[] = {0x8b, 0x8c, 0}; static const char PROGMEM shift_off_2[] = {0xab, 0xac, 0};
    static const char PROGMEM shift_on_1[]  = {0xcd, 0xce, 0}; static const char PROGMEM shift_on_2[]  = {0xcf, 0xd0, 0};
    static const char PROGMEM off_off_1[]   = {0xc5, 0}; static const char PROGMEM off_off_2[]   = {0xc6, 0};
    static const char PROGMEM on_off_1[]    = {0xc7, 0}; static const char PROGMEM on_off_2[]    = {0xc8, 0};
    static const char PROGMEM off_on_1[]    = {0xc9, 0}; static const char PROGMEM off_on_2[]    = {0xca, 0};
    static const char PROGMEM on_on_1[]     = {0xcb, 0}; static const char PROGMEM on_on_2[]     = {0xcc, 0};
    oled_write_P((modifiers & MOD_MASK_CTRL) ? ctrl_on_1 : ctrl_off_1, false);
    if      ((modifiers & MOD_MASK_CTRL) && (modifiers & MOD_MASK_SHIFT)) oled_write_P(on_on_1, false);
    else if  (modifiers & MOD_MASK_CTRL)                                   oled_write_P(on_off_1, false);
    else if  (modifiers & MOD_MASK_SHIFT)                                  oled_write_P(off_on_1, false);
    else                                                                    oled_write_P(off_off_1, false);
    oled_write_P((modifiers & MOD_MASK_SHIFT) ? shift_on_1 : shift_off_1, false);
    oled_write_P((modifiers & MOD_MASK_CTRL) ? ctrl_on_2 : ctrl_off_2, false);
    if      ((modifiers & MOD_MASK_CTRL) && (modifiers & MOD_MASK_SHIFT)) oled_write_P(on_on_2, false);
    else if  (modifiers & MOD_MASK_CTRL)                                   oled_write_P(on_off_2, false);
    else if  (modifiers & MOD_MASK_SHIFT)                                  oled_write_P(off_on_2, false);
    else                                                                    oled_write_P(off_off_2, false);
    oled_write_P((modifiers & MOD_MASK_SHIFT) ? shift_on_2 : shift_off_2, false);
}

// ============================================================
// マスター側OLED (128×32横長) - 背景シーン + 2軸アームロボット
//
// 背景: ポートタワー (左) + 六甲山 (中) + 海 (下)
// ロボット: モバイルマニピュレータ
//   - 移動: 左右に往復 (8pxステップ、x = 0〜96)
//   - 2軸アーム: 肩関節 + 肘関節
//   - エンドエフェクタ: 円形ボール (半径2)
//   - 5ポーズのアーム動作: 低左 → 上左 → 直上 → 上右 → 低右
// WPM応答: 高WPMほど速く動く・腕も速くパタつく
// ============================================================
#define MASTER_W 128
#define MASTER_PAGES 4
#define MASTER_BYTES 512
#define ROBOT_W 32
#define ROBOT_PAGES 4
#define ROBOT_BYTES 128
#define ROBOT_FRAME_COUNT 5
#define ROBOT_BOUNCE_LEN 8

// 背景シーン (静止、PROGMEM): ポートタワー + 六甲山 + 海
static const char PROGMEM master_scenery[512] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x80, 0x80, 0x40, 0x20, 0x20, 0x10, 0x20, 0x20, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x80,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x78, 0x06, 0x45, 0x01, 0x46, 0x38,
    0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x20, 0x10, 0x08, 0x08, 0x04, 0x02,
    0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x02,
    0x04, 0x04, 0x08, 0x08, 0x10, 0x08, 0x08, 0x04, 0x04, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x01, 0x02, 0x02, 0x04, 0x04, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x80, 0x80,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x50, 0x50, 0x5c, 0x57, 0x50, 0x54, 0x50, 0x54, 0x50, 0x54, 0x50,
    0x54, 0x53, 0x5c, 0x50, 0x50, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x60, 0x50,
    0x50, 0x48, 0x44, 0x44, 0x42, 0x41, 0x41, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x41, 0x42, 0x42, 0x44, 0x44, 0x48, 0x48, 0x50, 0x50, 0x60, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
    0x42, 0x42, 0x52, 0x12, 0x00, 0x00, 0x40, 0x40, 0x52, 0x12, 0x12, 0x12, 0x40, 0x40, 0x40, 0x00,
    0x12, 0x12, 0x52, 0x52, 0x40, 0x00, 0x00, 0x00, 0x52, 0x52, 0x52, 0x12, 0x00, 0x00, 0x40, 0x40,
    0x52, 0x12, 0x12, 0x12, 0x40, 0x40, 0x40, 0x00, 0x12, 0x12, 0x52, 0x52, 0x40, 0x00, 0x00, 0x00,
    0x52, 0x52, 0x52, 0x12, 0x00, 0x00, 0x40, 0x40, 0x52, 0x12, 0x12, 0x12, 0x40, 0x40, 0x40, 0x00,
    0x12, 0x12, 0x52, 0x52, 0x40, 0x00, 0x00, 0x00, 0x52, 0x52, 0x52, 0x12, 0x00, 0x00, 0x40, 0x40,
    0x52, 0x12, 0x12, 0x12, 0x40, 0x40, 0x40, 0x00, 0x12, 0x12, 0x52, 0x52, 0x40, 0x00, 0x00, 0x00,
    0x52, 0x52, 0x52, 0x12, 0x00, 0x00, 0x40, 0x40, 0x52, 0x12, 0x12, 0x12, 0x40, 0x40, 0x40, 0x00,
    0x12, 0x12, 0x52, 0x52, 0x40, 0x00, 0x00, 0x00, 0x52, 0x52, 0x52, 0x12, 0x00, 0x00, 0x40, 0x40,
};

// ロボット5フレーム (腕の各ポーズ: 低左→上左→直上→上右→低右)
static const char PROGMEM master_robot_0[128] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xc0, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x81, 0xc0, 0xc1, 0xc1, 0xc1, 0xc6, 0xd8,
    0xe0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8f, 0xdf, 0xff, 0xbc, 0x3f, 0x3f, 0x3f,
    0x3f, 0x3f, 0x3f, 0x3f, 0xbc, 0xff, 0xdf, 0x8f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static const char PROGMEM master_robot_1[128] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1c, 0x3e, 0x1c, 0xe8, 0xc0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc3,
    0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8f, 0xdf, 0xff, 0xbc, 0x3f, 0x3f, 0x3f,
    0x3f, 0x3f, 0x3f, 0x3f, 0xbc, 0xff, 0xdf, 0x8f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static const char PROGMEM master_robot_2[128] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xce,
    0xff, 0x0e, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8f, 0xdf, 0xff, 0xbc, 0x3f, 0x3f, 0x3f,
    0x3f, 0x3f, 0x3f, 0x3f, 0xbc, 0xff, 0xdf, 0x8f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static const char PROGMEM master_robot_3[128] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xc0, 0xc0, 0x68, 0x1c, 0x3e, 0x1c, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xfc, 0xc3, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8f, 0xdf, 0xff, 0xbc, 0x3f, 0x3f, 0x3f,
    0x3f, 0x3f, 0x3f, 0x3f, 0xbc, 0xff, 0xdf, 0x8f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static const char PROGMEM master_robot_4[128] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xe0, 0xd8, 0xc7, 0xc1, 0xc1, 0xc1, 0xc0, 0x81, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8f, 0xdf, 0xff, 0xbc, 0x3f, 0x3f, 0x3f,
    0x3f, 0x3f, 0x3f, 0x3f, 0xbc, 0xff, 0xdf, 0x8f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// アニメ用テーブル: ロボットフレームへの参照
static const char* const PROGMEM robot_frames[ROBOT_FRAME_COUNT] = {
    master_robot_0, master_robot_1, master_robot_2, master_robot_3, master_robot_4,
};

// ピンポンシーケンス: 0→1→2→3→4→3→2→1→0... で滑らかに動く
static const uint8_t bounce_seq[ROBOT_BOUNCE_LEN] = {0, 1, 2, 3, 4, 3, 2, 1};

static uint32_t robot_pos_timer = 0;
static int16_t  robot_x         = 0;
static int8_t   robot_dir       = 1;
static uint32_t robot_arm_timer = 0;
static uint8_t  robot_arm_step  = 0;

static void render_robot_master(void) {
    uint16_t wpm = get_current_wpm();

    // 位置更新: WPMで速度可変 (300ms@WPM0 → 80ms@WPM100+)
    uint16_t move_interval = 300 - (wpm * 2);
    if (move_interval < 80)  move_interval = 80;
    if (move_interval > 400) move_interval = 400;
    if (timer_elapsed32(robot_pos_timer) > move_interval) {
        robot_pos_timer = timer_read32();
        robot_x += robot_dir * 8;  // 8pxステップで移動
        if (robot_x >= 96) { robot_x = 96; robot_dir = -1; }
        else if (robot_x <= 0) { robot_x = 0; robot_dir = 1; }
    }

    // 腕アニメ: WPMで速度可変 (400ms@WPM0 → 100ms@WPM100+)
    int32_t arm_interval = 400 - ((int32_t)wpm * 3);
    if (arm_interval < 100) arm_interval = 100;
    if (arm_interval > 500) arm_interval = 500;
    if (timer_elapsed32(robot_arm_timer) > (uint32_t)arm_interval) {
        robot_arm_timer = timer_read32();
        robot_arm_step  = (robot_arm_step + 1) & 0x7;
    }

    // 現在のロボットフレーム選択
    uint8_t f_idx = bounce_seq[robot_arm_step & 0x7];
    const char *robot_data = (const char *)pgm_read_ptr(&robot_frames[f_idx]);

    // 背景 + ロボットを合成して描画
    // 各ピクセル: scenery OR robot (ロボットの白がシーンに上書き)
    for (uint8_t page = 0; page < MASTER_PAGES; page++) {
        for (uint16_t col = 0; col < MASTER_W; col++) {
            uint16_t idx = page * MASTER_W + col;
            uint8_t byte = pgm_read_byte(&master_scenery[idx]);
            int16_t rcol = (int16_t)col - robot_x;
            if (rcol >= 0 && rcol < ROBOT_W) {
                uint16_t r_idx = page * ROBOT_W + rcol;
                byte |= pgm_read_byte(&robot_data[r_idx]);
            }
            oled_write_raw_byte(byte, idx);
        }
    }
}

// ============================================================
// スレーブ側OLED (32×128縦長) - 情報表示
//   Row 0-2 : Track ball 39 ロゴ
//   Row 4-7 : Mod アイコン (GUI/Alt/Ctrl/Shift)
//   Row 8   : Layer 名
//   Row 9-12: RGB Mode / Hue / Sat / Val
//   Row 13  : デフォルト DPI
//   Row 14  : Sniping DPI
//   Row 15  : Dragscroll DPI
// ============================================================
static void slave_data(void) {
    render_logo();
    oled_set_cursor(0, 4);
    render_mod_status_gui_alt(get_mods());
    render_mod_status_ctrl_shift(get_mods());

    oled_set_cursor(0, 8);
    switch (get_highest_layer(layer_state)) {
        case LAYER_BASE:   oled_write("Base ", false); break;
        case LAYER_MOUSE:  oled_write("Mouse", false); break;
        case LAYER_SYMBOL: oled_write("Sym  ", false); break;
        case LAYER_RGB:    oled_write("RGB  ", false); break;
        case LAYER_NUMBER: oled_write("Num  ", false); break;
        case LAYER_NUMPAD: oled_write("Npad ", false); break;
        case LAYER_GAME:   oled_write("Game ", false); break;
        default:           oled_write("?????", false); break;
    }

    char buf[8];

#ifdef RGB_MATRIX_ENABLE
    // RGB Mode (row 9)
    oled_set_cursor(0, 9);
    if (rgb_matrix_is_enabled()) {
        snprintf(buf, sizeof(buf), "M:%03d", rgb_matrix_get_mode());
    } else {
        snprintf(buf, sizeof(buf), "M:OFF");
    }
    oled_write(buf, false);

    // Hue (row 10)
    oled_set_cursor(0, 10);
    snprintf(buf, sizeof(buf), "H:%03d", rgb_matrix_get_hue());
    oled_write(buf, false);

    // Saturation (row 11)
    oled_set_cursor(0, 11);
    snprintf(buf, sizeof(buf), "S:%03d", rgb_matrix_get_sat());
    oled_write(buf, false);

    // Value (row 12)
    oled_set_cursor(0, 12);
    snprintf(buf, sizeof(buf), "V:%03d", rgb_matrix_get_val());
    oled_write(buf, false);
#endif

#ifdef POINTING_DEVICE_ENABLE
    // Default DPI (row 13) - "DNNNN"
    oled_set_cursor(0, 13);
    snprintf(buf, sizeof(buf), "D%04u", charybdis_get_pointer_default_dpi());
    oled_write(buf, false);

    // Sniping DPI (row 14) - "PNNNN"
    oled_set_cursor(0, 14);
    snprintf(buf, sizeof(buf), "P%04u", charybdis_get_pointer_sniping_dpi());
    oled_write(buf, false);

    // Dragscroll DPI (row 15) - "CNNNN" (config.hのMINIMUM値を表示)
    oled_set_cursor(0, 15);
#if defined(CHARYBDIS_MINIMUM_DRAGSCROLL_DPI)
    snprintf(buf, sizeof(buf), "C%04u", (uint16_t)CHARYBDIS_MINIMUM_DRAGSCROLL_DPI);
#elif defined(CHARYBDIS_DRAGSCROLL_DPI)
    snprintf(buf, sizeof(buf), "C%04u", (uint16_t)CHARYBDIS_DRAGSCROLL_DPI);
#else
    snprintf(buf, sizeof(buf), "C----");
#endif
    oled_write(buf, false);
#endif
}

// ============================================================
// マスター側OLED (128×32横長)
//   通常: ロボット表示 (左右移動 + WPMで腕の速さ変化)
//   TInfoキー押下時: Keyball標準情報表示 (DPI等)
static void master_data(void) {
    if (user_config.is_oled_enabled) {
        oled_clear();
        keyball_oled_default();
        keyball_oled_info();
    } else {
        render_robot_master();
    }
}


bool oled_task_user(void) {
    if (is_keyboard_master()) {
        master_data();
    } else {
        slave_data();
    }
    return false;
}

#endif // OLED_ENABLE
