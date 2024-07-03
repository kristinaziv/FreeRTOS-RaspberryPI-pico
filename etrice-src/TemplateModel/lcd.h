/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#pragma once

// commands
#ifndef LCD_H
#define LCD_H


// By default these LCD display drivers are on bus address 0x27
static int addr = 0x27;

// Modes for lcd_send_byte
#define LCD_CHARACTER  1
#define LCD_COMMAND    0

#define MAX_LINES      2
#define MAX_CHARS      16


void i2c_write_byte(uint8_t val);
void lcd_init();
void lcd_print(int line, char *message);
void lcd_toggle_enable(uint8_t val) ;
void lcd_send_byte(uint8_t val, int mode);
void lcd_clear(void);
void lcd_string(const char *s);
void lcd_set_cursor(int line, int position);
static void inline lcd_char(char val);

#endif

