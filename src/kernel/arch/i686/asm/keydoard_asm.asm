; Initialize the keyboard hardware
; This code assumes an 8042 keyboard controller

global i386_init_keyboard
i386_init_keyboard:
    ; Send command to keyboard controller
    mov al, 0xAE ; Enable the keyboard
    out 0x64, al ; Send the command to the controller

    ret
