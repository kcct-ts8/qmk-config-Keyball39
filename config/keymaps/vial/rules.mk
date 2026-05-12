VIA_ENABLE = yes
VIAL_ENABLE = yes
VIALRGB_ENABLE = yes
VIAL_INSECURE = yes

OLED_ENABLE         = yes
EXTRAKEY_ENABLE     = yes
WPM_ENABLE = yes
ENCODER_ENABLE = no
ENCODER_MAP_ENABLE = no

VIAL_COMBO_ENABLE = no    # Vialの動的コンボを無効化
COMBO_ENABLE = yes        # QMK標準のコンボシステムを有効化(これは元から有効かも)

# Vialのkey_combos定義を強制的に抑止 (フォークがVIAL_COMBO_ENTRIESを使う場合用)
OPT_DEFS += -DVIAL_COMBO_ENTRIES=0
