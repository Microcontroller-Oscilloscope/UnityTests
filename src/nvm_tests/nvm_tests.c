/*
	nvm_tests.c - methods to test nvm methods
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
memCharString acceptEmptyFail[] PROG_FLASH = {ACCEPT_EMPTY_FAIL_STR};
memCharString callInitFail[] PROG_FLASH = {CALL_INIT_FAIL_STR};
memCharString defaultedFail[] PROG_FLASH = {DEFAULTED_FAIL_STR};
memCharString getFail[] PROG_FLASH = {GET_FAIL_STR};
memCharString noNullFail[] PROG_FLASH = {NO_NULL_FAIL_STR};
memCharString unequalFail[] PROG_FLASH = {UNEQUAL_FAIL_STR};
memCharString writeFail[] PROG_FLASH = {WRITE_FAIL_STR};

// nvm init error strings
memCharString badSizeFail[] PROG_FLASH = {BAD_SIZE_FAIL_STR};
memCharString nvmInitFail[] PROG_FLASH = {INIT_FAIL_STR};
memCharString initTestedIgnore[] PROG_FLASH = {"NVM tested"};
memCharString notInitFail[] PROG_FLASH = {NOT_INIT_FAIL_STR};
memCharString nvmStartedFail[] PROG_FLASH = {NVM_STARTED_FAIL_STR};
memCharString size0Fail[] PROG_FLASH = {SIZE_0_FAIL_STR};
memCharString initStopFail[] PROG_FLASH = {"stop"};
memCharString initInitFail[] PROG_FLASH = {"init"};

// nvm set default error strings
memCharString defaultClearFail[] PROG_FLASH = {DEFAULT_CLEAR_FAIL_STR};
memCharString defaultMaxSizeFail[] PROG_FLASH = {DEFAULT_MAX_SIZE_FAIL_STR};
memCharString defaultMaxSizeUnknownFail[] PROG_FLASH = {DEFAULT_MAX_SIZE_UNKNOWN_FAIL_STR};
memCharString defaultSizeTooBigFail[] PROG_FLASH = {DEFAULT_SIZE_TOO_BIG_FAIL_STR};
memCharString defaultWriteFail[] PROG_FLASH = {DEFAULT_WRITE_FAIL_STR};

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
void nvmNotStarted(void) {
	if (!nvmInitialized()) {
		printFail(callInitFail);
	}
}

/**
 * Tests if int writes and reads properly
 * 
 * @param writeptr pointer to write function
 * @param getptr pointer to get function
 * @param key key of value to test
 * @param value value to compare operations to
 * @param defaultValue default value from get
 * @param canDefault if get can return default value
 * @param T variable type literal
 */
#define TEST_INT(writeptr, getptr, key, value, defaultValue, canDefault, T) \
	if (!writeptr(key, value)) { \
		printFail(writeFail); \
	} \
	if (!getptr(key, &result, CAN_DEFAULT)) { \
		printFail(getFail); \
	} \
	if (value != result) { \
		printFail(unequalFail); \
	} \
	if (!canDefault && value == defaultValue) { \
		printFail(defaultedFail); \
	}

/**
 * Tests 0 and max value of unsigned int
 * 
 * @param writeptr pointer to write function
 * @param getptr pointer to get function
 * @param key key of value to test
 * @param max maximum value of the int
 * @param defaultValue default value from get
 * @param T variable type literal
 */
#define TEST_UINT_TYPE(writeptr, getptr, key, max, defaultValue, T) \
	nvmNotStarted(); \
	T result; \
	TEST_INT(writeptr, getptr, key, max, defaultValue, CAN_DEFAULT, T); \
	TEST_INT(writeptr, getptr, key, max, defaultValue, CAN_NOT_DEFAULT, T); \
	TEST_INT(writeptr, getptr, key, (T)0, defaultValue, CAN_DEFAULT, T);

/**
 * Tests 0, min value, and max value of signed int
 * 
 * @param writeptr pointer to write function
 * @param getptr pointer to get function
 * @param key key of value to test
 * @param min minimum value of the int
 * @param max maximum value of the int
 * @param defaultValue default value from get
 * @param T variable type literal
 */
#define TEST_INT_TYPE(writeptr, getptr, key, min, max, defaultValue, T) \
	nvmNotStarted(); \
	T result; \
	TEST_INT(writeptr, getptr, key, max, defaultValue, CAN_DEFAULT, T); \
	TEST_INT(writeptr, getptr, key, max, defaultValue, CAN_NOT_DEFAULT, T); \
	TEST_INT(writeptr, getptr, key, (T)0, defaultValue, CAN_DEFAULT, T); \
	TEST_INT(writeptr, getptr, key, min, defaultValue, CAN_DEFAULT, T); \
	TEST_INT(writeptr, getptr, key, min, defaultValue, CAN_NOT_DEFAULT, T);

void testNVMInit() {

	uint8_t result;
	enum NVMStartCode startCode;
	enum NVMDefaultCode defaultCode;
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
	valid = nvmGetUI8(U8_KEY, &result, CAN_DEFAULT);
	if (valid) {
		printFail(notInitFail);
	}
	valid = nvmWriteUI8(U8_KEY, (uint8_t)0);
	if (valid) {
		printFail(notInitFail);
	}

	// tests starting nvm
	startCode = nvmInit(NVM_SIZE);
	if (startCode == NVM_STARTED) {
		printFail(nvmStartedFail);
	}
	else if (startCode == NVM_FAILED) {
		printFail(nvmInitFail);
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
		printFail(initStopFail);

	}
	else if (defaultCode == NVM_DEFAULT_FAIL_INIT) {
		printFail(initInitFail);

	}
}

void testNVMBool() {
	TEST_UINT_TYPE(nvmWriteBool, nvmGetBool, BOOL_KEY, true, DEFAULT_BOOL, bool);
}

void testNVMi8() {
	TEST_INT_TYPE(nvmWriteI8, nvmGetI8, I8_KEY, (int8_t)INT8_MIN, (int8_t)INT8_MAX, (int8_t)DEFAULT_INT, int8_t);
}

void testNVMu8() {
	TEST_UINT_TYPE(nvmWriteUI8, nvmGetUI8, U8_KEY, (uint8_t)UINT8_MAX, (uint8_t)DEFAULT_INT, uint8_t);
}

void testNVMi16() {
	TEST_INT_TYPE(nvmWriteI16, nvmGetI16, I16_KEY, (int16_t)INT16_MIN, (int16_t)INT16_MAX, (int16_t)DEFAULT_INT, int16_t);
}

void testNVMu16() {
	TEST_UINT_TYPE(nvmWriteUI16, nvmGetUI16, U16_KEY, (uint16_t)UINT16_MAX, (uint16_t)DEFAULT_INT, uint16_t);
}

void testNVMi32() {
	TEST_INT_TYPE(nvmWriteI32, nvmGetI32, I32_KEY, (int32_t)INT32_MIN, (int32_t)INT32_MAX, (int32_t)DEFAULT_INT, int32_t);
}

void testNVMu32() {
	TEST_UINT_TYPE(nvmWriteUI32, nvmGetUI32, U32_KEY, (uint32_t)UINT32_MAX, (uint32_t)DEFAULT_INT, uint32_t);
}

void testNVMi64() {
	TEST_INT_TYPE(nvmWriteI64, nvmGetI64, I64_KEY, (int64_t)INT64_MIN, (int64_t)INT64_MAX, (int64_t)DEFAULT_INT, int64_t);
}

void testNVMu64() {
	TEST_UINT_TYPE(nvmWriteUI64, nvmGetUI64, U64_KEY, (uint64_t)UINT64_MAX, (uint64_t)DEFAULT_INT, uint64_t);
}

void testNVMFloat() {
	TEST_INT_TYPE(nvmWriteFloat, nvmGetFloat, FLOAT_KEY, (float)__FLT_MIN__, (float)__FLT_MAX__, (float)DEFAULT_FLOAT, float);
}

void testNVMDouble() {
	TEST_INT_TYPE(nvmWriteDouble, nvmGetDouble, DOUBLE_KEY, (double)__DBL_MIN__, (double)__DBL_MAX__, (double)DEFAULT_FLOAT, double);
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

	valid = nvmWriteCharArray(CHAR_ARRAY_KEY, nullPtr, 1);
	if (valid) {
		printFail(noNullFail);
	}

	valid = nvmWriteCharArray(CHAR_ARRAY_KEY, testVal, 0);
	if (valid) {
		printFail(size0Fail);
	}

	valid = nvmWriteCharArray(CHAR_ARRAY_KEY, emptyVal, 1);
	if (!valid) {
		printFail(acceptEmptyFail);
	}

	valid = nvmGetCharArray(CHAR_ARRAY_KEY, smallOutput, 1);
	if (!valid) {
		printFail(acceptEmptyFail);
	}

	if (!sameString(smallOutput, emptyVal)) {
		printFail(unequalFail);
	}

	valid = nvmWriteCharArray(CHAR_ARRAY_KEY, testVal, 2);
	if (valid) {
		printFail(badSizeFail);
	}

	valid = nvmWriteCharArray(CHAR_ARRAY_KEY, testVal, charSize);
	if (!valid) {
		TEST_FAIL();
	}

	valid = nvmGetCharArray(CHAR_ARRAY_KEY, nullPtr, 0);
	if (valid) {
		printFail(noNullFail);
	}

	valid = nvmGetCharArray(CHAR_ARRAY_KEY, smallOutput, 0);
	if (valid) {
		printFail(size0Fail);
	}

	valid = nvmGetCharArray(CHAR_ARRAY_KEY, output, 2);
	if (valid) {
		printFail(badSizeFail);
	}

	valid = nvmGetCharArray(CHAR_ARRAY_KEY, output, charSize);
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