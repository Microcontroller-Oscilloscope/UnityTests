/*
	nvm_tests.h - methods to test nvm methods
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

#ifndef NVM_TESTS_H
#define NVM_TESTS_H

#include <compile_flags/compile_flags.h>
#include "../test_cases.h"
#include <Arduino.h>
#include <unity.h>
#include <nvm/nvm.h>
#include <nvm/eeprom_addresses.h>

#ifndef CRIT_END_KEY
	#error CRIT_END_KEY not found for nvm_tests.h
#endif

#define BOOL_KEY		0 + CRIT_END_KEY
#define I8_KEY			(BOOL_KEY + BYTE1_SIZE)
#define U8_KEY			(I8_KEY + BYTE1_SIZE)
#define I16_KEY			(U8_KEY + BYTE1_SIZE)
#define U16_KEY			(I16_KEY + BYTE2_SIZE)
#define I32_KEY			(U16_KEY + BYTE2_SIZE)
#define U32_KEY			(I32_KEY + BYTE4_SIZE)
#define I64_KEY			(U32_KEY + BYTE4_SIZE)
#define U64_KEY			(I64_KEY + BYTE8_SIZE)
#define FLOAT_KEY		(U64_KEY + BYTE8_SIZE)
#define DOUBLE_KEY		(FLOAT_KEY + BYTE8_SIZE)
#define CHAR_ARRAY_KEY	(DOUBLE_KEY + BYTE8_SIZE)

#define TEST_STRING "test"

// maximum allowed size for testing strings in nvm
#define CHAR_ARRAY_MAX_SIZE 5

#define TEST_NVM_END_KEY CHAR_ARRAY_KEY + CHAR_ARRAY_MAX_SIZE

// nvm operations error strings
#define ACCEPT_EMPTY_FAIL_STR "Accept empty"
#define CALL_INIT_FAIL_STR "Call testNVMInit"
#define DEFAULTED_FAIL_STR "Default"
#define GET_FAIL_STR "Get"
#define NO_NULL_FAIL_STR "No NULL"
#define UNEQUAL_FAIL_STR "Unequal"
#define WRITE_FAIL_STR "Write"

// nvm init error strings
#define BAD_SIZE_FAIL_STR "Bad size"
#define INIT_FAIL_STR "Init"
#define INIT_TESTED_IGNORE_STR "NVM tested"
#define NOT_INIT_FAIL_STR "Not init"
#define NVM_STARTED_FAIL_STR "NVM started"
#define SIZE_0_FAIL_STR "Size 0"

// nvm set default error strings
#define DEFAULT_CLEAR_FAIL_STR "NVM clear"
#define DEFAULT_MAX_SIZE_FAIL_STR "nvmMaxSize"
#define DEFAULT_MAX_SIZE_UNKNOWN_FAIL_STR "nvmMaxSize?"
#define DEFAULT_SIZE_TOO_BIG_FAIL_STR "NVM_SIZE too big"
#define DEFAULT_WRITE_FAIL_STR "Default write"

/**
 * Tests if NVM can be initialized properly
 */
void testNVMInit();

/**
 * Tests if writing and getting Bool works
 */
void testNVMBool();

/**
 * Tests if writing and getting i8 works
 */
void testNVMi8();

/**
 * Tests if writing and getting u8 works
 */
void testNVMu8();

/**
 * Tests if writing and getting i16 works
 */
void testNVMi16();

/**
 * Tests if writing and getting u16 works
 */
void testNVMu16();

/**
 * Tests if writing and getting i32 works
 */
void testNVMi32();

/**
 * Tests if writing and getting u32 works
 */
void testNVMu32();

/**
 * Tests if writing and getting i64 works
 */
void testNVMi64();

/**
 * Tests if writing and getting u64 works
 */
void testNVMu64();

/**
 * Tests if writing and getting float works
 */
void testNVMFloat();

/**
 * Tests if writing and getting double works
 */
void testNVMDouble();

#ifndef NO_CHAR_ARRAY_SUPPORT
/**
 * Tests if writing and getting char array works
 */
void testNVMCharArray();
#endif

/**
 * Tests all nvm tests at once
 */
void testNVM();

#endif