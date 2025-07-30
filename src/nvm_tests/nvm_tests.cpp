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
 * @param defaultValue default value from get
 * @param canDefault if get can return default value
 */
template <typename T>
void testInt(uint16_t key, T value, T defaultValue, bool canDefault) {

	T result;

	writeValue(key, value);
	getValue(key, &result);
	if (value != result) {
		TEST_FAIL_MESSAGE("Mismatched Values");
	}
	if (!canDefault && value == defaultValue) {
		TEST_FAIL_MESSAGE("Value can't default");
	}
}

/**
 * Tests 0 and max value of unsigned int
 * 
 * @param key key of value to test
 * @param max maximum value of the int
 * @param defaultValue default value from get
 */
template <typename T>
void testIntType(uint16_t key, T max, T defaultValue) {

	nvmNotStarted();
	T zero = 0;

	testInt(key, max, defaultValue, CAN_DEFAULT);
	testInt(key, max, defaultValue, CAN_NOT_DEFAULT);
	testInt(key, zero, defaultValue, CAN_DEFAULT);
}

/**
 * Tests 0, min value, and max value of signed int
 * 
 * @param key key of value to test
 * @param min minimum value of the int
 * @param max maximum value of the int
 * @param defaultValue default value from get
 */
template <typename T>
void testIntType(uint16_t key, T min, T max, T defaultValue) {

	testIntType(key, max, defaultValue);
	testInt(key, min, defaultValue, CAN_DEFAULT);
	testInt(key, min, defaultValue, CAN_NOT_DEFAULT);
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
	testIntType(BOOL_KEY, true, DEFAULT_BOOL);
}

void testNVMi8() {
	testIntType(I8_KEY, (int8_t)INT8_MIN, (int8_t)INT8_MAX, (int8_t)DEFAULT_INT);
}

void testNVMu8() {
	testIntType(U8_KEY, (uint8_t)UINT8_MAX, (uint8_t)DEFAULT_INT);
}

void testNVMi16() {
	testIntType(I16_KEY, (int16_t)INT16_MIN, (int16_t)INT16_MAX, (int16_t)DEFAULT_INT);
}

void testNVMu16() {
	testIntType(U16_KEY, (uint16_t)UINT16_MAX, (uint16_t)DEFAULT_INT);
}

void testNVMi32() {
	testIntType(I32_KEY, (int32_t)INT32_MIN, (int32_t)INT32_MAX, (int32_t)DEFAULT_INT);
}

void testNVMu32() {
	testIntType(U32_KEY, (uint32_t)UINT32_MAX, (uint32_t)DEFAULT_INT);
}

void testNVMi64() {
	testIntType(I64_KEY, (int64_t)INT64_MIN, (int64_t)INT64_MAX, (int64_t)DEFAULT_INT);
}

void testNVMu64() {
	testIntType(U64_KEY, (uint64_t)UINT64_MAX, (uint64_t)DEFAULT_INT);
}

void testNVMFloat() {
	testIntType(FLOAT_KEY, (float)__FLT_MIN__, (float)__FLT_MAX__, (float)DEFAULT_FLOAT);
}

void testNVMDouble() {
	testIntType(DOUBLE_KEY, (double)__DBL_MIN__, (double)__DBL_MAX__, (double)DEFAULT_FLOAT);
}

void testNVMCharArray() {

	nvmNotStarted();

	char *testVal = (char*)"test";
	uint8_t charSize = charArraySize(testVal);

	char *nullPtr = nullptr;
	char smallOutput[1];
	char output[charSize];
	char *emptyVal = (char*)"";
	bool valid;

	valid = nvmWriteValue(CHAR_ARRAY_KEY, nullPtr, 1);
	if (valid) {
		TEST_FAIL_MESSAGE("Write shouldn't accept null input");
	}

	valid = nvmWriteValue(CHAR_ARRAY_KEY, testVal, 0);
	if (valid) {
		TEST_FAIL_MESSAGE("Write shouldn't accept max length of 0");
	}

	valid = nvmWriteValue(CHAR_ARRAY_KEY, emptyVal, 1);
	if (!valid) {
		TEST_FAIL_MESSAGE("Write should accept empty char array");
	}

	valid = nvmGetValue(CHAR_ARRAY_KEY, smallOutput, 1);
	if (!valid) {
		TEST_FAIL_MESSAGE("Get should accept empty char array");
	}

	if (!sameString(smallOutput, emptyVal)) {
		TEST_FAIL_MESSAGE("Failed to get empty string");
	}

	valid = nvmWriteValue(CHAR_ARRAY_KEY, testVal, 2);
	if (valid) {
		TEST_FAIL_MESSAGE("Write should catch max length not long enough");
	}

	valid = nvmWriteValue(CHAR_ARRAY_KEY, testVal, charSize);
	if (!valid) {
		TEST_FAIL_MESSAGE("Write should be successful");
	}

	valid = nvmGetValue(CHAR_ARRAY_KEY, nullPtr, 0);
	if (valid) {
		TEST_FAIL_MESSAGE("Get shouldn't accept null input");
	}

	valid = nvmGetValue(CHAR_ARRAY_KEY, smallOutput, 0);
	if (valid) {
		TEST_FAIL_MESSAGE("Get shouldn't accept max length of 0");
	}

	valid = nvmGetValue(CHAR_ARRAY_KEY, output, 2);
	if (valid) {
		TEST_FAIL_MESSAGE("Get should catch max length not long enough");
	}

	valid = nvmGetValue(CHAR_ARRAY_KEY, output, charSize);
	if (!valid) {
		TEST_FAIL_MESSAGE("Get should be successful");
	}

	if (!sameString(output, testVal)) {
		TEST_FAIL_MESSAGE("Failed to get test string");
	}
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
	RUN_TEST(&testNVMCharArray);
}

#endif
#endif