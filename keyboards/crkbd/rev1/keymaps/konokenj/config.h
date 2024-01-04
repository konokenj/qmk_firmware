/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

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

#pragma once

//#define USE_MATRIX_I2C

/* Select hand configuration */

// USBを左右どちらに接続しても良いように、EEPROMから設定を読み込む
// これを動作させるためには、flash時のブートローダーを左右個別に指定する
// $ qmk flash -kb crkbd/rev1 -km konokenj -bl dfu-split-left
// $ qmk flash -kb crkbd/rev1 -km konokenj -bl dfu-split-right
// https://docs.qmk.fm/#/config_options?id=split-keyboard-options
// #define MASTER_LEFT
// #define MASTER_RIGHT
#define EE_HANDS

#define QUICK_TAP_TERM 100
#define TAPPING_TERM 185

#ifdef RGBLIGHT_ENABLE
    #define RGBLIGHT_EFFECT_BREATHING
    #define RGBLIGHT_EFFECT_RAINBOW_MOOD
    #define RGBLIGHT_EFFECT_RAINBOW_SWIRL
    #define RGBLIGHT_EFFECT_SNAKE
    #define RGBLIGHT_EFFECT_KNIGHT
    #define RGBLIGHT_EFFECT_CHRISTMAS
    #define RGBLIGHT_EFFECT_STATIC_GRADIENT
    #define RGBLIGHT_EFFECT_RGB_TEST
    #define RGBLIGHT_EFFECT_ALTERNATING
    #define RGBLIGHT_EFFECT_TWINKLE
    #define RGBLIGHT_LIMIT_VAL 120
    #define RGBLIGHT_HUE_STEP 10
    #define RGBLIGHT_SAT_STEP 17
    #define RGBLIGHT_VAL_STEP 17
    #define RGBLIGHT_SLEEP  // enable rgblight_suspend() and rgblight_wakeup() in keymap.c
    #define RGBLIGHT_TIMEOUT 60000  // ms to wait until rgblight time out, 900K ms is 15min.
    #define WS2812_DI_PIN D3
    #define RGBLIGHT_DEFAULT_MODE RGBLIGHT_MODE_STATIC_LIGHT
    #define RGBLIGHT_DEFAULT_HUE 110
    #define RGBLIGHT_DEFAULT_SAT 255
    #define RGBLIGHT_DEFAULT_VAL 80
    #define RGBLIGHT_DEFAULT_ON false
#endif

#define OLED_FONT_H "keyboards/crkbd/lib/glcdfont.c"
