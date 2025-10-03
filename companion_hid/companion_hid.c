// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "raw_hid.h"

#include "introspection.h"

// by default layers/presses are not reported on change, can be altered with related subcommands
uint8_t companion_hid_report_change_flag = 0;
uint8_t companion_hid_report_press_flag  = 0;

void companion_hid_fill_state(uint8_t *data, layer_state_t state) {
    data[0] = id_companion_hid_out_state;
    data[1] = get_highest_layer(state);

#ifdef CAPS_WORD_ENABLE
    data[2] = is_caps_word_on() ? 1 : 0;
#else
    data[2] = 0;
#endif

    data[3] = companion_hid_report_change_flag;
    data[4] = companion_hid_report_press_flag;
}

void companion_hid_report_state(layer_state_t state) {
    if(companion_hid_report_change_flag) {
        uint8_t response[HID_MESSAGE_LENGTH];
        memset(response, 0, HID_MESSAGE_LENGTH);
        companion_hid_fill_state(response, state);
        raw_hid_send(response, HID_MESSAGE_LENGTH);
    }
}

void companion_hid_report_press(uint32_t symbol, const char* fallback) {
    if(companion_hid_report_press_flag) {
        uint8_t response[HID_MESSAGE_LENGTH];
        memset(response, 0, HID_MESSAGE_LENGTH);
        response[0] = id_companion_hid_out_press;
        response[1] = ((uint8_t*) &symbol)[0];
        response[2] = ((uint8_t*) &symbol)[1];
        response[3] = ((uint8_t*) &symbol)[2];
        response[4] = ((uint8_t*) &symbol)[3];
        raw_hid_send(response, HID_MESSAGE_LENGTH);
    } else {
        send_string(fallback);
    }
}


layer_state_t layer_state_set_companion_hid(layer_state_t state) {
    companion_hid_report_state(state);
    return state;
}

#ifdef CAPS_WORD_ENABLE
void caps_word_set_user(bool active) {
    // NB layer_state is globally defined in upper code
    companion_hid_report_state(layer_state);
}
#endif

void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    // hid messages are 32 bytes long so it's always safe to read first/second/third bytes
    uint8_t command = data[0];

    if(command == id_companion_hid_in) {
        uint8_t subcommand = data[1];
        uint8_t argument   = data[2];

        // response rewrites inout data because via sends data back after func completion
        memset(data, 0, length);

        switch(subcommand) {
            case id_get_version:
                data[0] = id_companion_hid_out_version;
                data[1] = COMPANION_HID_VERSION;
                break;
            case id_get_layer_state:
                companion_hid_fill_state(data, layer_state);
                break;
            case id_set_report_change:
                companion_hid_report_change_flag = argument;
                companion_hid_fill_state(data, layer_state);
                break;
            case id_set_report_press:
                companion_hid_report_press_flag = argument;
                companion_hid_fill_state(data, layer_state);
                break;
            case id_invert_layer:
                if(argument > 0) {
                    uint8_t save_report_state = companion_hid_report_change_flag;
                    companion_hid_report_change_flag = 0;
                    layer_invert(argument);
                    companion_hid_report_change_flag = save_report_state;
                    companion_hid_fill_state(data, layer_state);
                } else {
                    data[0] = id_companion_hid_out_error;
                }
                break;
            default:
                data[0] = id_companion_hid_out_error;
        }
    }
}
