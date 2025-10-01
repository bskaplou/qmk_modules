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
// so companion_hid uses 0x88 for commands and 0x89+ for to-host messages
// to-host message first byte is message-type marker
// because some types are keyboard-sourced 
enum companion_hid_command_id {
    id_companion_hid_in                     = 0x88,
    id_companion_hid_out_state              = 0x89,
    id_companion_hid_out_press              = 0x90,
    id_companion_hid_out_version            = 0x91,
    id_companion_hid_out_error              = 0x92,
};

// Second byte of message should be subcommand to save command space, same as vial
enum companion_hid_subcommand_id {
    id_get_version                          = 0x00,
    id_get_layer_state                      = 0x01,
    id_set_report_change                    = 0x02,
    id_invert_layer                         = 0x03,
    id_set_report_press                     = 0x04,
};

void companion_hid_report_press(uint32_t symbol, const char* fallback);
