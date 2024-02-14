#pragma once

#include <stddef.h>
#include <stdint.h>

#include "stdio.h"
#include "x86.h"
#include "vga_text.h"
#include "vesa_text.h"

#include "../../libs/include/memory.h"
#include "../../libs/include/stdlib.h"
#include "../../libs/include/string.h"

#define HIDDEN_KEYBOARD         0
#define VISIBLE_KEYBOARD        1

#define F4_BUTTON               '\254'
#define F3_BUTTON               '\255'
#define F2_BUTTON               '\7'
#define F1_BUTTON               '\6'

#define UP_ARROW_BUTTON         '\4'
#define DOWN_ARROW_BUTTON       '\3'
#define LEFT_ARROW_BUTTON       '\1'
#define RIGHT_ARROW_BUTTON      '\2'

#define ENTER_BUTTON            '\n'
#define BACKSPACE_BUTTON        '\b'

int key_press();
char get_character(char character);

char* keyboard_read(int mode, int color);
char keyboard_navigation();
void keyboard_wait(char symbol);

void __attribute__((cdecl)) i386_init_keyboard();