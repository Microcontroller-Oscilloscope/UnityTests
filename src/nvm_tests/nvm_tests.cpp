/*
	eeprom_tests.cpp - methods to test Arduino EEPROM
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

#include "../test_cases.h"

#ifdef __TEST_CASES__

#include "nvm_tests.h"

uint16_t tnvmSize = 0U;
bool tInitTested = false;

#if VALID_NVM

/**
 * Prints nvm not started message
 */
void nvmNotStarted() {
	if (tnvmSize == DEFAULT_NVM_SIZE) {
		TEST_FAIL_MESSAGE("NVM not started!!! Call 'setupNVMTests(size)' before other NVM functions");
	}
	if (!tInitTested) {
		TEST_FAIL_MESSAGE("Call testNVMInit() before getting/writing");
	}
}

/**
 * Writes value to nvm
 */
template <typename T> void writeValue(uint16_t address, T value) {
	bool success = nvmWriteValue(address, value);
	if (!success) {
		TEST_FAIL_MESSAGE("NVM failed write");
	}
}

/**
 * Gets value from nvm
 */
template <typename T> void getValue(uint16_t address, T *value) {
	bool success = nvmGetValue(address, value);
	if (!success) {
		TEST_FAIL_MESSAGE("NVM failed get");
	}
}

/**
 * Tests if int writes and reads properly
 * 
 * @param key key of value to test
 * @param value value to compare operations to
 */
template <typename T>
void testInt(uint16_t key, T value) {

	T result;

	writeValue(key, value);
	getValue(key, &result);
	if (value != result) {
		TEST_FAIL_MESSAGE("Mismatched Values");
	}
}

/**
 * Tests 0 and max value of unsigned int
 * 
 * @param key key of value to test
 * @param max maximum value of the int
 */
template <typename T>
void testIntType(uint16_t key, T max) {

	nvmNotStarted();
	T zero = 0;

	testInt(key, max);
	testInt(key, zero);
}

/**
 * Tests 0, min value, and max value of signed int
 * 
 * @param key key of value to test
 * @param min minimum value of the int
 * @param max maximum value of the int
 */
template <typename T>
void testIntType(uint16_t key, T min, T max) {

	testIntType(key, max);
	testInt(key, min);
}

void setupNVMTests(uint16_t nvmSizeNew) {
	if (tnvmSize != DEFAULT_NVM_SIZE) {
		return;
	}
	tnvmSize = nvmSizeNew;
}

void testNVMInit() {

	if (tInitTested) {
		TEST_IGNORE_MESSAGE("NVM init already tested");
	}

	uint8_t result;
	enum NVMStartCode code;
	bool valid;

	// test initialization with size 0
	code = nvmInit(0U);
	if (code != NVM_INVALID_SIZE) {
		TEST_FAIL_MESSAGE("NVM shouldn't accept size of 0");
	}

	// tests getting/writing value prematurely
	valid = nvmGetValue(U8_KEY, &result);
	if (valid) {
		TEST_FAIL_MESSAGE("NVM shouldn't get value when not initialized");
	}
	valid = nvmWriteValue(U8_KEY, (uint8_t)0);
	if (valid) {
		TEST_FAIL_MESSAGE("NVM shouldn't write value when not initialized");
	}

	// tests starting nvm
	code = nvmInit(tnvmSize);
	if (code == NVM_STARTED) {
		TEST_FAIL_MESSAGE("NVM already started");
	}
	else if (code == NVM_FAILED) {
		TEST_FAIL_MESSAGE("NVM failed to start");
	}
	else if (code == NVM_INVALID_SIZE) {
		TEST_FAIL_MESSAGE("NVM size given was incorrect");
	}
	else if (code != NVM_OK) {
		TEST_FAIL_MESSAGE("NVM failed to start for some unknown reason");
	}

	// tests trying to start again
	code = nvmInit(tnvmSize);
	if (code != NVM_STARTED) {
		TEST_FAIL_MESSAGE("NVM should notice its already started");
	}

	tInitTested = true;
}

void testNVMBool() {
	testIntType(BOOL_KEY, true);
}

void testNVMi8() {
	typedef int8_t nvmInt;
	testIntType(I8_KEY, (nvmInt)INT8_MIN, (nvmInt)INT8_MAX);
}

void testNVMu8() {
	typedef uint8_t nvmInt;
	testIntType(U8_KEY, (nvmInt)UINT8_MAX);
}

void testNVMi16() {
	typedef int16_t nvmInt;
	testIntType(I16_KEY, (nvmInt)INT16_MIN, (nvmInt)INT16_MAX);
}

void testNVMu16() {
	typedef uint16_t nvmInt;
	testIntType(U16_KEY, (nvmInt)UINT16_MAX);
}

void testNVMi32() {
	typedef int32_t nvmInt;
	testIntType(I32_KEY, (nvmInt)INT32_MIN, (nvmInt)INT32_MAX);
}

void testNVMu32() {
	typedef uint32_t nvmInt;
	testIntType(U32_KEY, (nvmInt)UINT32_MAX);
}

void testNVMi64() {
	typedef int64_t nvmInt;
	testIntType(I64_KEY, (nvmInt)INT64_MIN, (nvmInt)INT64_MAX);
}

void testNVMu64() {
	typedef uint64_t nvmInt;
	testIntType(U64_KEY, (nvmInt)UINT64_MAX);
}

void testNVMFloat() {
	typedef float nvmInt;
	testIntType(FLOAT_KEY, (nvmInt)__FLT_MIN__, (nvmInt)__FLT_MAX__);
}

void testNVMDouble() {
	typedef double nvmInt;
	testIntType(DOUBLE_KEY, (nvmInt)__DBL_MIN__, (nvmInt)__DBL_MAX__);
}

void testNVM() {
	RUN_TEST(&testNVMInit);
	RUN_TEST(&testNVMBool);
	RUN_TEST(&testNVMi8);
	RUN_TEST(&testNVMu8);
	RUN_TEST(&testNVMi16);
	RUN_TEST(&testNVMu16);
	RUN_TEST(&testNVMi32);
	RUN_TEST(&testNVMu32);
	RUN_TEST(&testNVMi64);
	RUN_TEST(&testNVMu64);
	RUN_TEST(&testNVMFloat);
	RUN_TEST(&testNVMDouble);
}

#endif
#endif