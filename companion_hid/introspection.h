// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include QMK_KEYBOARD_H

#ifdef COMPANION_HID_TOUCHBOARD
#define COMPANION_HID_SAFE_RANGE QK_KB_3
#else
#define COMPANION_HID_SAFE_RANGE QK_KB_0
#endif

#ifdef COMPANION_HID_TOUCHBOARD
enum companioin_hid_touchbar_keys {
    TB_MOVE = QK_KB_0, // To place into three leftmost positions in vial User buttons
    TB_1,
    TB_2,
};
#endif

void companion_hid_report_press(uint32_t symbol, const char* fallback, keyrecord_t *record);
