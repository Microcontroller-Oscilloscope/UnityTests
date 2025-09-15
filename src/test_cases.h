/*
	test_cases.h - methodology to test features
	Copyright (C) 2025 Camren Chraplak

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef TEST_CASES_H
#define TEST_CASES_H

#include <compile_flags/compile_flags.h>

// version of microcontroller oscilloscope unity tests
#define OSC_TEST_MAJOR_VERSION 0
#define OSC_TEST_MINOR_VERSION 2
#define OSC_TEST_PATCH_VERSION 0

#include <Arduino.h>
#include <unity.h>

// delay to allow serial connection to board
#define SERIAL_DELAY 2000

// data type for progmem char arrays
typedef const PROGMEM char memCharString;

/**
 * Writes ignore message
 * 
 * @param message flash message to print
 */
void printIgnore(memCharString *message);

/**
 * Writes fail message
 * 
 * @param message flash message to print
 */
void printFail(memCharString *message);
#endif