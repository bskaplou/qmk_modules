// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "raw_hid.h"

#include "introspection.h"

// by default layers are not reported on change, can be altered with id_set_report_change subcommand
uint8_t companion_hid_report = 0;

void companion_hid_fill_state(uint8_t *data, layer_state_t state) {
    data[1] = get_highest_layer(state);

#ifdef CAPS_WORD_ENABLE
    data[2] = is_caps_word_on() ? 1 : 0;
#else
    data[2] = 0;
#endif

    data[3] = companion_hid_report;
}

void companion_hid_report_press(layer_state_t state) {
    uint8_t response[HID_MESSAGE_LENGTH];
    memset(response, 0, HID_MESSAGE_LENGTH);
    response[0] = id_companion_hid_out;
    response[1] = id_hid_press_out;
    raw_hid_send(response, HID_MESSAGE_LENGTH);
}

void companion_hid_report_state(layer_state_t state) {
    if(companion_hid_report) {
        uint8_t response[HID_MESSAGE_LENGTH];
        memset(response, 0, HID_MESSAGE_LENGTH);
        response[0] = id_companion_hid_out;
        companion_hid_fill_state(response, state);
        raw_hid_send(response, HID_MESSAGE_LENGTH);
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
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
        data[0] = id_companion_hid_out;
        // error will be returned in case of no subcommand match and success
        data[1] = COMPANION_HID_MESSAGE_ERROR;

        switch(subcommand) {
            case id_get_version:
                data[1] = COMPANION_HID_VERSION;
                break;
            case id_set_report_change:
                companion_hid_report = argument;
            // active layer returned for set_report_change to avoid additional commands from consumer
            case id_get_layer_state:
                companion_hid_fill_state(data, layer_state);
                break;
            case id_invert_layer:
                if(layer_state < DYNAMIC_KEYMAP_LAYER_COUNT) {
                    uint8_t save_report_state = companion_hid_report;
                    companion_hid_report = 0;
                    layer_invert(argument);
                    companion_hid_report = save_report_state;
                    companion_hid_fill_state(data, layer_state);
                }
                break;
        }
    }
}
