# QMK Companion HID module

Companion HID module extends RAW_HID protocol with several HID commands and features.

It allows to send layers and caps_word state data to host computer.

QmkLayoutWidget is a reference implementation of Companion HID protocol which indicates current layer and caps word state in sysmtem tray.

https://github.com/bskaplou/QmkLayoutWidget

Motivation for this modules is here https://www.reddit.com/r/ErgoMechKeyboards/comments/1nofeb6/current_layer_tray_indicator_for_qmkvial_keyboards/

### Installation

#### Checkout module
For QMK
```
# Clone qmk or vial repository.
git clone https://github.com/qmk/qmk_firmware.git

# Clone module
git clone https://github.com/bskaplou/qmk_modules.git qmk_firmware/modules/bskaplou
```

For Vial
```
# Clone qmk or vial repository.
git clone https://github.com/vial-kb/vial-qmk.git

# Clone module
git clone https://github.com/bskaplou/qmk_modules.git vial-qmk/modules/bskaplou
```

#### Edit keymap.json of your keymap

Add 'bskaplou/companion_hid' module to keyboards/<your_keyboard>/keymaps/<your_keymap>/keymap.json

After addition it should contain root key 'modules' similar to listed below.

```
"modules": ["bskaplou/companion_hid"]
```

NB is your keymap has no keymap.json, just create it as follows

```
{
  "keyboard": "<your_keyboard_name>",
  "keymap": "<your_keymap_name>",
  "layout": "<your_layout_name>",
  "modules": ["bskaplou/companion_hid"]
}
```

All done, just compile your keymap with companion_hid support as usual, flash your keyboard and start widget!

