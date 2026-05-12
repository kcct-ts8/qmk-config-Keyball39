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
};

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

        TG(LAYER_RGB), TG(LAYER_SYMBOL), LT(LAYER_NUMBER, KC_ESC), KC_TAB, MO(LAYER_MOUSE), LT(LAYER_NUMBER, KC_SPC),
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

        KC_F22, _______, SNIPING, LCTL(SGUI(KC_D)), KC_F15,                             KC_BSPC, KC_MS_BTN1, DRGSCRL, KC_MS_BTN2, KC_F16,

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
        RGB_TOG,  RGB_M_P,  RGB_M_B,  RGB_M_R,  RGB_M_SW,        QK_BOOT,  _______,  TInfo,    _______,  T_SAVE,

        RGB_MOD,  RGB_HUI,  RGB_SAI,  RGB_VAI,  RGB_M_SN,        RGB_MSP,  S_D_MOD,  _______,  _______,  DRG_TOG,

        RGB_RMOD, RGB_HUD,  RGB_SAD,  RGB_VAD,  RGB_M_K,         DPI_RMOD, EE_CLR,   _______,  DPI_MOD,  _______,

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
    // Layer 5: Numpad
    // =========================================================
    [LAYER_NUMPAD] = LAYOUT(
        _______, _______, _______, _______, _______,            KC_PEQL, KC_P1,   KC_P2,   KC_P3,   KC_PPLS,

        _______, _______, _______, _______, _______,            KC_PMNS, KC_P4,   KC_P5,   KC_P6,   KC_PMNS,

        _______, _______, _______, _______, _______,            KC_PAST, KC_P7,   KC_P8,   KC_P9,   KC_PSLS,

        _______, _______, _______, _______, _______, _______,   KC_P0,   KC_PDOT, _______
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
    }
    return true;
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
// 神戸高専ロボティクス ロゴデータ (32×32, 3フレーム)
// マスター(横長)とスレーブ(縦長)両方で使用
// ============================================================
#define KKR_LOGO_FRAME_SIZE 128

static const char PROGMEM kkr_up[KKR_LOGO_FRAME_SIZE] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
    0xfc, 0xfc, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8f, 0xdf, 0xff, 0xbc, 0x3f, 0x3f, 0x3f,
    0x3f, 0x3f, 0x3f, 0x3f, 0xbc, 0xff, 0xdf, 0x8f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const char PROGMEM kkr_mid[KKR_LOGO_FRAME_SIZE] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40,
    0xc0, 0xc0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8f, 0xdf, 0xff, 0xbc, 0x3f, 0x3f, 0x3f,
    0x3f, 0x3f, 0x3f, 0x3f, 0xbc, 0xff, 0xdf, 0x8f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
static const char PROGMEM kkr_down[KKR_LOGO_FRAME_SIZE] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc8,
    0xf8, 0xf8, 0xc8, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8f, 0xdf, 0xff, 0xbc, 0x3f, 0x3f, 0x3f,
    0x3f, 0x3f, 0x3f, 0x3f, 0xbc, 0xff, 0xdf, 0x8f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// ============================================================
// マスター側OLED (横長 128×32) ロボット表示
//   ロボット 32×32 は左右に動く (x = 0 〜 64)
//   腕の動作速度は WPM に比例 (打鍵が速いほど腕も速い)
//   右側32px (char col 16-21) には WPM と Layer 名を表示
// ============================================================
static uint32_t robot_pos_timer = 0;
static int16_t  robot_x         = 0;
static int8_t   robot_dir       = 1;
static uint32_t robot_arm_timer = 0;
static uint8_t  robot_arm_step  = 0;

static void render_robot_master(void) {
    uint16_t wpm = get_current_wpm();

    // ロボット位置: WPMが高いほど速く移動 (200ms間隔 → 80ms間隔)
    uint16_t move_interval = 200 - (wpm / 2);
    if (move_interval < 80) move_interval = 80;
    if (timer_elapsed32(robot_pos_timer) > move_interval) {
        robot_pos_timer = timer_read32();
        robot_x += robot_dir;
        if (robot_x >= 64) { robot_x = 64; robot_dir = -1; }
        else if (robot_x <= 0) { robot_x = 0; robot_dir = 1; }
    }

    // 腕アニメーション: WPMが高いほど速く (400ms → 80ms)
    int32_t arm_interval = 400 - ((int32_t)wpm * 3);
    if (arm_interval < 80)  arm_interval = 80;
    if (arm_interval > 500) arm_interval = 500;
    if (timer_elapsed32(robot_arm_timer) > (uint32_t)arm_interval) {
        robot_arm_timer = timer_read32();
        robot_arm_step  = (robot_arm_step + 1) & 0x3;
    }

    // 画面をクリア
    oled_clear();

    // フレーム選択 (ピンポン: 0,1,2,1)
    static const uint8_t bounce_seq[4] = {0, 1, 2, 1};
    uint8_t f = bounce_seq[robot_arm_step & 0x3];
    const char* frame_data;
    switch (f) {
        case 0:  frame_data = kkr_up;   break;
        case 2:  frame_data = kkr_down; break;
        default: frame_data = kkr_mid;  break;
    }

    // 32列×4ページのロボット画像を (robot_x, 0) に描画
    // 横長OLEDのバイト配置: index = page * 128 + col
    for (uint8_t page = 0; page < 4; page++) {
        for (uint8_t col = 0; col < 32; col++) {
            uint16_t src_index = page * 32 + col;
            uint16_t dst_index = page * 128 + robot_x + col;
            if (dst_index < 4 * 128) {
                oled_write_raw_byte(pgm_read_byte(&frame_data[src_index]), dst_index);
            }
        }
    }

    // 右側に WPM と Layer 名を表示 (char col 16 = pixel col 96)
    char buf[8];

    oled_set_cursor(16, 0);
    oled_write("WPM ", false);

    oled_set_cursor(16, 1);
    snprintf(buf, sizeof(buf), "%3u ", wpm);
    oled_write(buf, false);

    oled_set_cursor(16, 2);
    oled_write("Lyr ", false);

    oled_set_cursor(16, 3);
    switch (get_highest_layer(layer_state)) {
        case LAYER_BASE:   oled_write("Base", false); break;
        case LAYER_MOUSE:  oled_write("Mous", false); break;
        case LAYER_SYMBOL: oled_write("Sym ", false); break;
        case LAYER_RGB:    oled_write("RGB ", false); break;
        case LAYER_NUMBER: oled_write("Num ", false); break;
        case LAYER_NUMPAD: oled_write("NPad", false); break;
        default:           oled_write("?   ", false); break;
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
