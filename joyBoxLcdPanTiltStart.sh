#!/bin/bash
#
# utils
# sudo systemctl daemon-reload
# sudo journalctl -u joy_box_panel.service
# sudo systemctl status joy_box_panel.service
# sudo systemctl enable joy_box_panel.service

# qt from source 23.10.2012
export QT_SELECT="Qt-5.15.2"
export DISPLAY=:0.0
export XDG_RUNTIME_DIR=/run/user/1000
/home/pi/joy_box_lcd_panel/joyBoxLcdPanTilt
