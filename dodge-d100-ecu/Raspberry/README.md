## Raspberry PI ECU

#### Rotate Screen

Open `/boot/config.txt` and add the line `display_rotate=x` on the top, where x could be:

    - 1:    Rotate 90 degrees.
    - 2:    Rotate 180 degrees.
    - 3:    Rotate 270 degrees.

#### Remove rainbow pattern and boot messages

Open `/boot/cmdline.txt` and add the following text to de end of the line:

    - `quiet plymouth.ignore-serial-consoles logo.nologo vt.global_cursor_default=0`

Also, change `console=tty1` to `console=tty3`

#### Remove Raspberry logo

Open `/boot/config.txt` and add the line `disable_splash=1`