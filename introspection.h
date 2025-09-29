// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include QMK_KEYBOARD_H
#include "raw_hid.h"

#define COMPANION_HID_VERSION        0x01 
#define COMPANION_HID_MESSAGE_ERROR  0xFF

#define HID_MESSAGE_LENGTH        32

// via uses commands 0x01-0x13
// vial uses 0xFE
// via declares 0xFF as unhandled
// so companion_hid uses 0x88 for commands and 0x89 for reponses
enum companion_hid_command_id {
    id_companion_hid_in                     = 0x88,
    id_companion_hid_out                    = 0x89,
    id_hid_press_out                        = 0x90,
    id_get_version                          = 0x00,
    id_get_layer_state                      = 0x01,
    id_set_report_change                    = 0x02,
    id_invert_layer                         = 0x03,
};
