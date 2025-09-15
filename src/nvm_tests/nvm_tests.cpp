/*
	nvm.cpp - methods to test nvm methods
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

#include "nvm_tests.h"

// nvm operations error strings
memCharString acceptEmptyFail[] = {ACCEPT_EMPTY_FAIL_STR};
memCharString callInitFail[] = {CALL_INIT_FAIL_STR};
memCharString defaultedFail[] = {DEFAULTED_FAIL_STR};
memCharString getFail[] = {GET_FAIL_STR};
memCharString noNullFail[] = {NO_NULL_FAIL_STR};
memCharString unequalFail[] = {UNEQUAL_FAIL_STR};
memCharString writeFail[] = {WRITE_FAIL_STR};

// nvm init error strings
memCharString badSizeFail[] = {BAD_SIZE_FAIL_STR};
memCharString initFail[] = {INIT_FAIL_STR};
memCharString initTestedIgnore[] = {INIT_TESTED_IGNORE_STR};
memCharString notInitFail[] = {NOT_INIT_FAIL_STR};
memCharString nvmStartedFail[] = {NVM_STARTED_FAIL_STR};
memCharString size0Fail[] = {SIZE_0_FAIL_STR};

// nvm set default error strings
memCharString defaultClearFail[] = {DEFAULT_CLEAR_FAIL_STR};
memCharString defaultMaxSizeFail[] = {DEFAULT_MAX_SIZE_FAIL_STR};
memCharString defaultMaxSizeUnknownFail[] = {DEFAULT_MAX_SIZE_UNKNOWN_FAIL_STR};
memCharString defaultSizeTooBigFail[] = {DEFAULT_SIZE_TOO_BIG_FAIL_STR};
memCharString defaultWriteFail[] = {DEFAULT_WRITE_FAIL_STR};

/**
 * Tests if nvm is started
 */
bool nvmInitialized(void) {

	enum NVMStartCode code;

	// test if nvm started
	code = nvmInit(DEFAULT_NVM_SIZE);

	if (code == NVM_STARTED) {
		return true;
	}
	return false;
}

/**
 * Prints nvm not started message
 */
void nvmNotStarted() {
	if (!nvmInitialized()) {
		printFail(callInitFail);
	}
}

/**
 * Writes value to nvm
 */
template <typename T> void writeValue(uint16_t address, T value) {
	bool success = nvmWriteValue(address, value);
	if (!success) {
		printFail(writeFail);
	}
}

/**
 * Gets value from nvm
 */
template <typename T> void getValue(uint16_t address, T *value) {
	bool success = nvmGetValue(address, value);
	if (!success) {
		printFail(getFail);
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
		printFail(unequalFail);
	}
	if (!canDefault && value == defaultValue) {
		printFail(defaultedFail);
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

	testInt(key, max, defaultValue, CAN_DEFAULT);
	testInt(key, max, defaultValue, CAN_NOT_DEFAULT);
	testInt(key, (T)0, defaultValue, CAN_DEFAULT);
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

void testNVMInit() {

	uint8_t result;
	enum NVMStartCode startCode;
	NVMDefaultCode defaultCode;
	bool valid;

	/**
	 * Tests before nvm start
	 */

	// test if nvm started successfully
	if (nvmInitialized()) {
		printIgnore(initTestedIgnore);
	}

	// test setting default when nvm not started
	defaultCode = nvmSetDefaults();
	if (defaultCode != NVM_DEFAULT_FAIL_MAX_SIZE) {
		printFail(defaultMaxSizeFail);
	}

	// test initialization with size 0
	startCode = nvmInit(DEFAULT_NVM_SIZE);
	if (startCode != NVM_INVALID_SIZE) {
		printFail(size0Fail);
	}

	// tests getting/writing value prematurely
	valid = nvmGetValue(U8_KEY, &result);
	if (valid) {
		printFail(notInitFail);
	}
	valid = nvmWriteValue(U8_KEY, (uint8_t)0);
	if (valid) {
		printFail(notInitFail);
	}

	// tests starting nvm
	startCode = nvmInit(NVM_SIZE);
	if (startCode == NVM_STARTED) {
		printFail(nvmStartedFail);
	}
	else if (startCode == NVM_FAILED) {
		printFail(initFail);
	}
	else if (startCode == NVM_INVALID_SIZE) {
		printFail(badSizeFail);
	}
	else if (startCode != NVM_OK) {
		TEST_FAIL();
	}

	/**
	 * Tests after nvm start
	 */

	// tests trying to start again
	startCode = nvmInit(NVM_SIZE);
	if (startCode != NVM_STARTED) {
		printFail(nvmStartedFail);
	}

	// tests setting defaults
	defaultCode = nvmSetDefaults();
	if (defaultCode == NVM_DEFAULT_SIZE_TOO_BIG) {
		printFail(defaultSizeTooBigFail);
	}
	else if (defaultCode == NVM_DEFAULT_FAIL_MAX_SIZE) {
		printFail(defaultMaxSizeUnknownFail);
	}
	else if (defaultCode == NVM_DEFAULT_FAIL_CLEAR) {
		printFail(defaultClearFail);
	}
	else if (defaultCode == NVM_DEFAULT_FAIL_WRITE) {
		printFail(defaultWriteFail);
	}
	else if (defaultCode == NVM_DEFAULT_FAIL_STOP) {
		printFail("stop");
	}
	else if (defaultCode == NVM_DEFAULT_FAIL_INIT) {
		printFail("init");
	}
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

#ifndef NO_CHAR_ARRAY_SUPPORT

// test to verify testing strings are within bounds
static_assert(CHAR_ARRAY_MAX_SIZE > 0, "CHAR_ARRAY_MAX_SIZE needs to be greater than 0");
static_assert(sizeof(TEST_STRING) <= CHAR_ARRAY_MAX_SIZE, "Test string too large or \
		CHAR_ARRAY_MAX_SIZE too small");

void testNVMCharArray() {

	nvmNotStarted();

	char *testVal = (char*)TEST_STRING;
	uint8_t charSize = charArraySize(testVal);

	char *nullPtr = nullptr;
	char smallOutput[1];
	char output[charSize];
	char *emptyVal = (char*)"";
	bool valid;

	valid = nvmWriteValue(CHAR_ARRAY_KEY, nullPtr, 1);
	if (valid) {
		printFail(noNullFail);
	}

	valid = nvmWriteValue(CHAR_ARRAY_KEY, testVal, 0);
	if (valid) {
		printFail(size0Fail);
	}

	valid = nvmWriteValue(CHAR_ARRAY_KEY, emptyVal, 1);
	if (!valid) {
		printFail(acceptEmptyFail);
	}

	valid = nvmGetValue(CHAR_ARRAY_KEY, smallOutput, 1);
	if (!valid) {
		printFail(acceptEmptyFail);
	}

	if (!sameString(smallOutput, emptyVal)) {
		printFail(unequalFail);
	}

	valid = nvmWriteValue(CHAR_ARRAY_KEY, testVal, 2);
	if (valid) {
		printFail(badSizeFail);
	}

	valid = nvmWriteValue(CHAR_ARRAY_KEY, testVal, charSize);
	if (!valid) {
		TEST_FAIL();
	}

	valid = nvmGetValue(CHAR_ARRAY_KEY, nullPtr, 0);
	if (valid) {
		printFail(noNullFail);
	}

	valid = nvmGetValue(CHAR_ARRAY_KEY, smallOutput, 0);
	if (valid) {
		printFail(size0Fail);
	}

	valid = nvmGetValue(CHAR_ARRAY_KEY, output, 2);
	if (valid) {
		printFail(badSizeFail);
	}

	valid = nvmGetValue(CHAR_ARRAY_KEY, output, charSize);
	if (!valid) {
		TEST_FAIL();
	}

	if (!sameString(output, testVal)) {
		printFail(unequalFail);
	}
}

#endif

void testNVM() {
	RUN_TEST(&testNVMInit);
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
	#ifndef NO_CHAR_ARRAY_SUPPORT
		RUN_TEST(&testNVMCharArray);
	#endif
}