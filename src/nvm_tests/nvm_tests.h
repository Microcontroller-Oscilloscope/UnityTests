/*
	eeprom_tests.h - methods to test Arduino EEPROM
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

#include <compile_flags.h>

#include "../test_cases.h"

#ifdef __TEST_CASES__

#include <Arduino.h>
#include <unity.h>

#ifndef VALID_NVM
#define VALID_NVM 0
#endif

#ifdef NVM_EEPROM
#undef VALID_NVM
#define VALID_NVM 1
#endif

#ifdef NVM_PREF
#undef VALID_NVM
#define VALID_NVM 1
#endif

#if VALID_NVM

#include "nvm/eeprom_addresses.h"

#ifdef NVM_EEPROM
#include "nvm/core_eeprom.h"
#endif

#ifdef NVM_PREF
#include "nvm/core_pref.h"
#endif

#define BOOL_KEY		0
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

/**
 * Sets up NVM testing suite
 * 
 * @param nvmSize size of NVM storage
 */
void setupNVMTests(uint16_t nvmSize);

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

#endif
#endif