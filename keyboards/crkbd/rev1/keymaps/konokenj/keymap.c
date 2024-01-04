/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

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

#include QMK_KEYBOARD_H
#include "keycodes.h"
#include "keymap_extras/keymap_japanese.h"
#include <stdio.h>

#define _QWERTY 0
#define _LOWER 1
#define _RAISE 2
#define _ADJUST 4

enum custom_keycodes {
    QWERTY = SAFE_RANGE,
    LOWER,
    RAISE,
    ADJUST,
    ESCIMOFF, // Send EISU key when ESC is pressed
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_split_3x6_3(
KC_TAB , KC_Q, KC_W, KC_E, KC_R, KC_T, /***/ KC_Y, KC_U, KC_I   , KC_O  , KC_P   , KC_BSPC,
KC_LCTL, KC_A, KC_S, KC_D, KC_F, KC_G, /***/ KC_H, KC_J, KC_K   , KC_L  , KC_SCLN, KC_QUOT,
KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, /***/ KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, ESCIMOFF,
               KC_LGUI, LOWER, KC_SPC, /***/ KC_ENT, RAISE, KC_RALT
  ),

  [_LOWER] = LAYOUT_split_3x6_3(
KC_TAB , KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC, /***/ KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
KC_LCTL, KC_1   , KC_2 , KC_3   , KC_4  , KC_5   , /***/ KC_MINS, KC_EQL , KC_LBRC, KC_RBRC, KC_BSLS, KC_GRV,
KC_LSFT, KC_6   , KC_7 , KC_8   , KC_9  , KC_0   , /***/ KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
                        KC_LGUI, _______,  KC_SPC, /***/ KC_ENT, _______, KC_RALT
  ),

  [_RAISE] = LAYOUT_split_3x6_3(
KC_TAB , XXXXXXX   , XXXXXXX   , XXXXXXX      , XXXXXXX   , XXXXXXX   , /***/ C(KC_LEFT), C(KC_DOWN), KC_MCTL, C(KC_RGHT), KC_LPAD   , KC_BSPC,
KC_LCTL, XXXXXXX   , XXXXXXX   , XXXXXXX      , XXXXXXX   , XXXXXXX   , /***/ KC_LEFT   , KC_DOWN   , KC_UP  , KC_RGHT   , G(KC_UP)  , KC_DEL,
KC_LSFT, G(KC_MINS), G(KC_PLUS), G(S(C(KC_4))), G(S(KC_4)), G(S(KC_5)), /***/ KC_HOME   , KC_PGDN   , KC_PGUP, KC_END    , G(KC_DOWN), G(KC_BSPC),
                                         KC_LGUI, _______,  KC_SPC,     /***/ KC_ENT, _______, KC_RALT
  ),

  [_ADJUST] = LAYOUT_split_3x6_3(
KC_TAB , XXXXXXX, KC_F2 , KC_F3 , KC_F4 , KC_F5 , /***/ XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_BSPC,
KC_LCTL, KC_F6  , KC_F7 , KC_F8 , KC_F9 , KC_F10, /***/ KC_MPRV, KC_MPLY, KC_MNXT, KC_BRID, KC_BRIU, XXXXXXX,
KC_LSFT, KC_F11 , KC_F12, KC_F13, KC_F14, KC_F15, /***/ KC_MUTE, KC_VOLD, KC_VOLU, KC_CAPS, KC_INS , KC_SLEP,
                       KC_LGUI, _______,  KC_SPC, /***/ KC_ENT, _______, KC_RALT
  )
};
// clang-format on

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (!is_keyboard_master()) {
        return OLED_ROTATION_180; // flips the display 180 degrees if offhand
    }
    return rotation;
}

void oled_render_layer_state(void) {
    oled_write_P(PSTR("Layer: "), false);
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_ln_P(PSTR("QWERTY"), false);
            break;
        case _LOWER:
            oled_write_ln_P(PSTR("Lower"), false);
            break;
        case _RAISE:
            oled_write_ln_P(PSTR("Raise"), false);
            break;
        case _ADJUST:
            oled_write_ln_P(PSTR("Adjust"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undefined"), false);
    }
}

char keylog_str[24] = {};

const char code_to_name[60] = {' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\', '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
    char name = ' ';
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) || (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) {
        keycode = keycode & 0xFF;
    }
    if (keycode < 60) {
        name = code_to_name[keycode];
    }

    // update keylog
    snprintf(keylog_str, sizeof(keylog_str), "%dx%d, k%2d : %c", record->event.key.row, record->event.key.col, keycode, name);
}

void oled_render_keylog(void) {
    oled_write(keylog_str, false);
}

void render_bootmagic_status(bool status) {
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0};
    oled_write_P(crkbd_logo, false);
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_render_layer_state();
        oled_render_keylog();
    } else {
        oled_render_logo();
    }
    return false;
}
#endif // OLED_ENABLE

#ifdef RGBLIGHT_ENABLE
// RGB Lightingのオートスリープ
// https://docs.qmk.fm/#/custom_quantum_functions?id=keyboard-housekeeping
static uint32_t key_timer;           // timer for last keyboard activity, use 32bit value and function to make longer idle time possible
static void refresh_rgb(void);       // refreshes the activity timer and RGB, invoke whenever any activity happens
static void check_rgb_timeout(void); // checks if enough time has passed for RGB to timeout
bool is_rgb_timeout = false;         // store if RGB has timed out or not in a boolean

void refresh_rgb(void) {
    key_timer = timer_read32(); // store time of last refresh
    if (is_rgb_timeout)
    {
        is_rgb_timeout = false;
        rgblight_wakeup();
    }
}
void check_rgb_timeout(void) {
    if (!is_rgb_timeout && timer_elapsed32(key_timer) > RGBLIGHT_TIMEOUT) // check if RGB has already timeout and if enough time has passed
    {
        rgblight_suspend();
        is_rgb_timeout = true;
    }
}
/* Then, call the above functions from QMK's built in post processing functions like so */
/* Runs at the end of each scan loop, check if RGB timeout has occured or not */
void housekeeping_task_user(void) {
#ifdef RGBLIGHT_TIMEOUT
    check_rgb_timeout();
#endif
}
/* Runs after each key press, check if activity occurred */
void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef RGBLIGHT_TIMEOUT
    if (record->event.pressed)
        refresh_rgb();
#endif
}
/* Runs after each encoder tick, check if activity occurred */
void post_encoder_update_user(uint8_t index, bool clockwise) {
#ifdef RGBLIGHT_TIMEOUT
    refresh_rgb();
#endif
}
#endif // RGBLIGHT_ENABLE

// https://gist.github.com/okapies/5d13a174cbb13ce34dbd9faede9d0b71#file-keymap-c-L99-L164
static bool     lower_pressed      = false;
static uint16_t lower_pressed_time = 0;
static bool     raise_pressed      = false;
static uint16_t raise_pressed_time = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        set_keylog(keycode, record);
    }

    switch (keycode) {
        // Send EISU key when ESC is pressed
        case ESCIMOFF:
            if (record->event.pressed) {
                register_code(KC_ESC);
                register_code(KC_LNG2);
                register_code(JP_MHEN);
            } else {
                unregister_code(KC_ESC);
                unregister_code(KC_LNG2);
                unregister_code(JP_MHEN);
            }
            return false;
            break;

        case LOWER:
            if (record->event.pressed) {
                lower_pressed      = true;
                lower_pressed_time = record->event.time;

                layer_on(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);

                if (lower_pressed && (TIMER_DIFF_16(record->event.time, lower_pressed_time) < TAPPING_TERM)) {
                    register_code(KC_LNG2); // for macOS
                    register_code(JP_MHEN);
                    unregister_code(JP_MHEN);
                    unregister_code(KC_LNG2);
                }
                lower_pressed = false;
            }
            return false;
            break;

        case RAISE:
            if (record->event.pressed) {
                raise_pressed      = true;
                raise_pressed_time = record->event.time;

                layer_on(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);

                if (raise_pressed && (TIMER_DIFF_16(record->event.time, raise_pressed_time) < TAPPING_TERM)) {
                    register_code(KC_LNG1); // for macOS
                    register_code(JP_HENK);
                    unregister_code(JP_HENK);
                    unregister_code(KC_LNG1);
                }
                raise_pressed = false;
            }
            return false;
            break;

        case ADJUST:
            if (record->event.pressed) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            return false;
            break;
        default:
            if (record->event.pressed) {
                // reset the flags
                lower_pressed = false;
                raise_pressed = false;
            }
            break;
    }
    return true;
}
