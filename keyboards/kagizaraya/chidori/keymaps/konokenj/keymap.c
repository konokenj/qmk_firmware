/* Copyright 2019 ENDO Katsuhiro <ka2hiro@kagizaraya.jp>
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
#include QMK_KEYBOARD_H

#include "board.h"
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
  [_QWERTY] = LAYOUT(
KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R , KC_T  , /***/ KC_Y  , KC_U , KC_I   , KC_O   , KC_P   , KC_BSPC,
KC_LCTL, KC_A   , KC_S   , KC_D   , KC_F , KC_G  , /***/ KC_H  , KC_J , KC_K   , KC_L   , KC_SCLN, KC_QUOT,
KC_LSFT, KC_Z   , KC_X   , KC_C   , KC_V , KC_B  , /***/ KC_N  , KC_M , KC_COMM, KC_DOT , KC_SLSH, ESCIMOFF,
XXXXXXX, XXXXXXX, KC_LALT, KC_LGUI, LOWER, KC_SPC, /***/ KC_ENT, RAISE, KC_RALT, XXXXXXX, XXXXXXX, XXXXXXX
  ),

  [_LOWER] = LAYOUT(
KC_TAB , KC_EXLM, KC_AT  , KC_HASH, KC_DLR , KC_PERC, /***/ KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
KC_LCTL, KC_1   , KC_2   , KC_3   , KC_4   , KC_5   , /***/ KC_MINS, KC_EQL , KC_LBRC, KC_RBRC, KC_BSLS, KC_GRV,
KC_LSFT, KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , /***/ KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
_______, _______, _______, _______, _______, _______, /***/ _______, _______, _______, _______, _______, _______
  ),

  [_RAISE] = LAYOUT(
KC_TAB , XXXXXXX   , XXXXXXX   , XXXXXXX      , XXXXXXX   , XXXXXXX   , /***/ C(KC_LEFT), C(KC_DOWN), KC_MCTL, C(KC_RGHT), KC_LPAD   , KC_BSPC,
KC_LCTL, XXXXXXX   , XXXXXXX   , XXXXXXX      , XXXXXXX   , XXXXXXX   , /***/ KC_LEFT   , KC_DOWN   , KC_UP  , KC_RGHT   , G(KC_UP)  , KC_DEL,
KC_LSFT, G(KC_MINS), G(KC_PLUS), G(S(C(KC_4))), G(S(KC_4)), G(S(KC_5)), /***/ KC_HOME   , KC_PGDN   , KC_PGUP, KC_END    , G(KC_DOWN), G(KC_BSPC),
_______, _______   , _______   , _______      , _______   , _______   , /***/ _______   , _______   , _______, _______   , _______   , _______
     ),

  [_ADJUST] = LAYOUT(
KC_TAB , XXXXXXX, KC_F2  , KC_F3  , KC_F4  , KC_F5  , /***/ XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_BSPC,
KC_LCTL, KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 , /***/ KC_MPRV, KC_MPLY, KC_MNXT, KC_BRID, KC_BRIU, XXXXXXX,
KC_LSFT, KC_F11 , KC_F12 , KC_F13 , KC_F14 , KC_F15 , /***/ KC_MUTE, KC_VOLD, KC_VOLU, KC_CAPS, KC_INS , KC_SLEP,
_______, _______, _______, _______, _______, _______, /***/ _______, _______, _______, _______, _______, _______
  )
};
// clang-format on

// layer_state_t layer_state_set_user(layer_state_t state) {
//     return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
// }

// https://gist.github.com/okapies/5d13a174cbb13ce34dbd9faede9d0b71#file-keymap-c-L99-L164
static bool     lower_pressed      = false;
static uint16_t lower_pressed_time = 0;
static bool     raise_pressed      = false;
static uint16_t raise_pressed_time = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
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
    }
    return true;
}

bool led_update_user(led_t led_state) {
    board_set_led_by_index(0, LED_YELLOW, led_state.caps_lock);
    board_set_led_by_index(1, LED_YELLOW, led_state.scroll_lock);

    return false;
}
