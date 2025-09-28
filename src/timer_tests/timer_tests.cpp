/*
	timer_tests.cpp - methods to test timer methods
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

/**
 * All timer paths
 * 
 * base: configuration when timer hasn't been touched since startup
 * init: initHardTimer function
 * deconstruct: deconstructHardTimer function
 * cancel: cancelHardTimer function
 * set: setHardTimer function
 * 
 * Valid paths:
 * 
 * base/deconstruct -> init
 * init/cancel -> deconstruct
 * init/cancel -> set
 * set -> cancel
 * set -> deconstruct
 * 
 * Invalid paths:
 * 
 * base/deconstruct -> deconstruct
 * base/deconstruct -> cancel
 * base/deconstruct -> set
 * 
 * init/cancel -> init
 * init/cancel -> cancel
 * 
 * set -> set
 * set -> init
 */

/**
 * All timer paths
 * 
 * (x, y): x(init), y(start)
 * 
 * (0,0): deconstruct, base
 * (1,0): init, set
 * (1,1): set
 * (0,1): NOT POSSIBLE
 * 
 * Valid paths:
 * (0,0) <-> (1,0) <-> (1,1)
 * (0,0) <- (1,1)
 * 		OR
 * (0,0) -> (1,0)
 * (1,0) -> (0,0)
 * (1,0) -> (1,1)
 * (1,1) -> (0,0)
 * (1,1) -> (1,0)
 * 
 * Invalid paths:
 * 
 * (0,0) -> (0,0)
 * (0,0) -> (1,0)
 * 
 * (1,0) -> (1,0)
 * 
 * (1,1) -> (1,1)
 */

#include "timer_tests.h"
#include "../test_cases.h"

#if NUM_TIMERS > 0

#include <timer.h>

/****************************
 * Slow Timer Defines
****************************/

#define HARD_TIMER_TEST HARD_TIMER(HARD_TIMER_TEST_INDEX) // hardware timer for testing
#define HARD_TIMER_TEST_FUNCTION() HARD_TIMER_FUNCTION(HARD_TIMER_TEST_INDEX) // starter function for testing
#define HARD_TIMER_TEST_REFERENCE HARD_TIMER_REFERENCE(HARD_TIMER_TEST_INDEX) // reference for testing function

#define HARD_TIMER_TEST_DELAY_MS 100 // delay for each iteration of timer
#define HARD_TIMER_TEST_DELAY_ELLAPSE_MS 1000 // time for timer to run for
#define HARD_TIMER_TEST_COUNT_TARGET HARD_TIMER_TEST_DELAY_ELLAPSE_MS / HARD_TIMER_TEST_DELAY_MS // target count for timer
#ifndef HARD_TIMER_TEST_COUNT_BUFFER
	#define HARD_TIMER_TEST_COUNT_BUFFER 0 // amount timer can be off of goal
#endif

/**
 * Initializes testing timer
 * 
 * @return bool: if initialization successful
 */
#define INIT_TIMER() initHardTimer(HARD_TIMER_TEST, &HARD_TIMER_TEST_REFERENCE, HARD_TIMER_TEST_SCALAR)

/**
 * Sets testing timer
 * 
 * @return bool: if set successful
 */
#define SET_TIMER() setHardTimer(HARD_TIMER_TEST, &HARD_TIMER_TEST_REFERENCE, HARD_TIMER_TEST_SCALAR, HARD_TIMER_TEST_MULTIPLIER * HARD_TIMER_TEST_DELAY_MS)

/**
 * Cancels testing timer
 * 
 * @return bool: if set successful
 */
#define CANCEL_TIMER() cancelHardTimer(HARD_TIMER_TEST)

/**
 * Decontructs testing timer
 * 
 * @return bool: if deconstruct successful
 */
#define DECONSTRUCT_TIMER() deconstructHardTimer(HARD_TIMER_TEST)

/****************************
 * Fast Timer Defines
****************************/

#define HARD_TIMER_FAST_TEST HARD_TIMER(HARD_TIMER_FAST_TEST_INDEX) // hardware timer for testing
#define HARD_TIMER_FAST_TEST_FUNCTION() HARD_TIMER_FUNCTION(HARD_TIMER_FAST_TEST_INDEX) // starter function for testing
#define HARD_TIMER_FAST_TEST_REFERENCE HARD_TIMER_REFERENCE(HARD_TIMER_FAST_TEST_INDEX) // reference for testing function

/**
 * Initializes testing timer
 * 
 * @return bool: if initialization successful
 */
#define INIT_FAST_TIMER() initHardTimer(HARD_TIMER_FAST_TEST, &HARD_TIMER_FAST_TEST_REFERENCE, HARD_TIMER_FAST_TEST_SCALAR)

/**
 * Sets testing timer
 * 
 * @return bool: if set successful
 */
#define SET_FAST_TIMER() setHardTimer(HARD_TIMER_FAST_TEST, &HARD_TIMER_FAST_TEST_REFERENCE, HARD_TIMER_FAST_TEST_SCALAR, HARD_TIMER_FAST_TEST_MULTIPLIER * HARD_TIMER_FAST_TEST_DELAY)

/**
 * Cancels testing timer
 * 
 * @return bool: if set successful
 */
#define CANCEL_FAST_TIMER() cancelHardTimer(HARD_TIMER_FAST_TEST)

/**
 * Decontructs testing timer
 * 
 * @return bool: if deconstruct successful
 */
#define DECONSTRUCT_FAST_TIMER() deconstructHardTimer(HARD_TIMER_FAST_TEST)

memCharString invalidInitFail[] = {"Init State"};
memCharString invalidStartFail[] = {"Start State"};
memCharString initFail[] = {"Init"};
memCharString reinitFail[] = {"Reinit"};
memCharString startFail[] = {"Start"};
memCharString restartFail[] = {"Restart"};
memCharString cancelFail[] = {"Stop"};
memCharString recancelFail[] = {"Restop"};
memCharString deconstructFail[] = {"Decon"};
memCharString redeconstructFail[] = {"Redecon"};


memCharString path00toIFail[] = {"00>I"};
memCharString paths00toIFail[] = {"S00>I"};
memCharString pathi00toIFail[] = {"I00>I"};

memCharString path00toDFail[] = {"00>D"};
memCharString paths00toDFail[] = {"S00>D"};
memCharString pathi00toDFail[] = {"I00>D"};

memCharString path00toCFail[] = {"00>C"};
memCharString paths00toCFail[] = {"S00>C"};
memCharString pathi00toCFail[] = {"I00>C"};

memCharString path00toSFail[] = {"00>S"};
memCharString paths00toSFail[] = {"S00>S"};
memCharString pathi00toSFail[] = {"I00>S"};



memCharString path01toIFail[] = {"01>I"};
memCharString paths01toIFail[] = {"S01>I"};
memCharString pathi01toIFail[] = {"I01>I"};

memCharString path01toDFail[] = {"01>D"};
memCharString paths01toDFail[] = {"S01>D"};
memCharString pathi01toDFail[] = {"I01>D"};

memCharString path01toCFail[] = {"01>C"};
memCharString paths01toCFail[] = {"S01>C"};
memCharString pathi01toCFail[] = {"I01>C"};

memCharString path01toSFail[] = {"01>S"};
memCharString paths01toSFail[] = {"S01>S"};
memCharString pathi01toSFail[] = {"I01>S"};



memCharString path11toIFail[] = {"11>I"};
memCharString paths11toIFail[] = {"S11>I"};
memCharString pathi11toIFail[] = {"I11>I"};

memCharString path11toDFail[] = {"11>D"};
memCharString paths11toDFail[] = {"S11>D"};
memCharString pathi11toDFail[] = {"I11>D"};

memCharString path11toCFail[] = {"11>C"};
memCharString paths11toCFail[] = {"S11>C"};
memCharString pathi11toCFail[] = {"I11>C"};

memCharString path11toSFail[] = {"11>S"};
memCharString paths11toSFail[] = {"S11>S"};
memCharString pathi11toSFail[] = {"I11>S"};

volatile uint32_t hardTimerCount = 0U;

/**
 * Testing function
 */
HARD_TIMER_TEST_FUNCTION() {
	hardTimerCount++;
	HARD_TIMER_END();
}

/**
 * Tests timer init state
 * 
 * @param timer timer to test
 * @param init whether timer should or shouldn't be initialized
 */
void testGetInitState(hardware_timer_t timer, bool init) {
	if (timerInitialized(timer) != init) {
		printFail(invalidInitFail);
	}
}

/**
 * Tests timer init state
 * 
 * @param timer timer to test
 * @param init whether timer should or shouldn't be initialized
 * @param initMessage message to print
 */
void testGetInitState(hardware_timer_t timer, bool init, memCharString *initMessage) {
	if (timerInitialized(timer) != init) {
		printFail(initMessage);
	}
}

/**
 * Tests timer start state
 * 
 * @param timer timer to test
 * @param start whether timer should or shouldn't be started
 */
void testGetStartState(hardware_timer_t timer, bool start) {
	if (timerStarted(timer) != start) {
		printFail(invalidStartFail);
	}
}

/**
 * Tests timer start state
 * 
 * @param timer timer to test
 * @param start whether timer should or shouldn't be started
 * @param startMessage message to print
 */
void testGetStartState(hardware_timer_t timer, bool start, memCharString *startMessage) {
	if (timerStarted(timer) != start) {
		printFail(startMessage);
	}
}

/**
 * Tests timer states
 * 
 * @param timer timer to test
 * @param init whether timer should or shouldn't be initialized
 * @param start whether timer should or shouldn't be started
 */
void testGetState(hardware_timer_t timer, bool init, bool start) {
	testGetInitState(timer, init);
	testGetStartState(timer, start);
}

/**
 * Tests timer states
 * 
 * @param timer timer to test
 * @param init whether timer should or shouldn't be initialized
 * @param start whether timer should or shouldn't be started
 * @param initMessage init message to print
 * @param startMessage start message to print
 */
void testGetState(hardware_timer_t timer, bool init, bool start, memCharString *initMessage, memCharString *startMessage) {
	testGetInitState(timer, init, initMessage);
	testGetStartState(timer, start, startMessage);
}

/**
 * Tests that no timers are set when starting
 * 
 * @warning Only call when timers aren't started nor initialized
 */
void testProgramStart() {
	for (int i = 0; i < NUM_TIMERS; i++) {
		testGetState((hardware_timer_t)i, false, false);
	}
}

/**
 * Tests correct status for repeat initializations
 */
void testRepeatInit() {

	if (!INIT_TIMER()) {
		printFail(initFail);
	}
	if (INIT_TIMER()) {
		printFail(reinitFail);
	}
}

/**
 * Tests correct status for repeat starts
 */
void testRepeatStart() {

	if (!SET_TIMER()) {
		printFail(startFail);
	}
	if (SET_TIMER()) {
		printFail(restartFail);
	}
}

/**
 * Tests correct status for repeat cancels
 */
void testRepeatCancel() {

	if (!CANCEL_TIMER()) {
		printFail(cancelFail);
	}
	if (CANCEL_TIMER()) {
		printFail(recancelFail);
	}
}

/**
 * Tests correct status for repeat deconstructs
 */
void testRepeatDeconstruct() {

	if (!DECONSTRUCT_TIMER()) {
		printFail(deconstructFail);
	}
	if (DECONSTRUCT_TIMER()) {
		printFail(redeconstructFail);
	}
}

/**
 * Tests for all possible flag paths
 */
void testPathFlags() {

	/**
	 * (0,0)
	 */
	testGetState(HARD_TIMER_TEST, false, false);

	// 00>D
	if (DECONSTRUCT_TIMER()) {
		printFail(path00toDFail);
	}
	testGetState(HARD_TIMER_TEST, false, false, paths00toDFail, pathi00toDFail);
	// 00>C
	if (CANCEL_TIMER()) {
		printFail(path00toCFail);
	}
	testGetState(HARD_TIMER_TEST, false, false, paths00toCFail, pathi00toCFail);
	// 00>S
	if (SET_TIMER()) {
		printFail(path00toSFail);
	}
	testGetState(HARD_TIMER_TEST, false, false, paths00toSFail, pathi00toSFail);
	// 00>I
	if (!INIT_TIMER()) {
		printFail(path00toIFail);
	}
	testGetState(HARD_TIMER_TEST, true, false, paths00toIFail, pathi00toIFail);

	/**
	 * (1,0)
	 */

	// 10>I
	if (INIT_TIMER()) {
		printFail(path01toIFail);
	}
	testGetState(HARD_TIMER_TEST, true, false, paths01toIFail, pathi01toIFail);
	// 10>C
	if (CANCEL_TIMER()) {
		printFail(path01toCFail);
	}
	testGetState(HARD_TIMER_TEST, true, false, paths01toCFail, pathi01toCFail);
	// 10>S
	if (!SET_TIMER()) {
		printFail(path01toSFail);
	}
	testGetState(HARD_TIMER_TEST, true, true, paths01toSFail, pathi01toSFail);

	/**
	 * (1,1)
	 */
	
	// 11>I
	if (INIT_TIMER()) {
		printFail(path11toIFail);
	}
	testGetState(HARD_TIMER_TEST, true, true, paths11toIFail, pathi11toIFail);
	// 11>S
	if (SET_TIMER()) {
		printFail(path11toSFail);
	}
	testGetState(HARD_TIMER_TEST, true, true, paths11toSFail, pathi11toSFail);
	// 11>C
	if (!CANCEL_TIMER()) {
		printFail(path11toCFail);
	}
	testGetState(HARD_TIMER_TEST, true, false, paths11toCFail, pathi11toCFail);

	/**
	 * (1,0)
	 */
	
	// 10>D
	if (!DECONSTRUCT_TIMER()) {
		printFail(path01toDFail);
	}
	testGetState(HARD_TIMER_TEST, false, false, paths01toDFail, pathi01toDFail);

	/**
	 * (0,0)
	 */

	// 00>I
	if (!INIT_TIMER()) {
		printFail(path00toIFail);
	}
	testGetState(HARD_TIMER_TEST, true, false, paths00toIFail, pathi00toIFail);

	/**
	 * (1,0)
	 */

	// 10>S
	if (!SET_TIMER()) {
		printFail(path01toSFail);
	}
	testGetState(HARD_TIMER_TEST, true, true, paths01toSFail, pathi01toSFail);

	/**
	 * (1,1)
	 */

	// 11>D
	if (!DECONSTRUCT_TIMER()) {
		printFail(path11toDFail);
	}
	testGetState(HARD_TIMER_TEST, false, false, paths11toDFail, pathi11toDFail);

	/**
	 * (0,0)
	 */
	testGetState(HARD_TIMER_TEST, false, false);
}

void testTiming() {

	testGetState(HARD_TIMER_TEST, false, false);
	hardTimerCount = 0U;

	if (!INIT_TIMER()) {
		printFail(initFail);
	}
	if (!SET_TIMER()) {
		printFail(startFail);
	}

	delay(HARD_TIMER_TEST_DELAY_ELLAPSE_MS);
	if (!DECONSTRUCT_TIMER()) {
		printFail(deconstructFail);
	}

	TEST_ASSERT_UINT32_WITHIN(HARD_TIMER_TEST_COUNT_BUFFER, HARD_TIMER_TEST_COUNT_TARGET, hardTimerCount);
}

void testFastTiming() {

	// ensure slow timing works first
	testTiming();

	testGetState(HARD_TIMER_FAST_TEST, false, false);
	hardTimerCount = 0U;

	if (!INIT_FAST_TIMER()) {
		printFail(initFail);
	}
	if (!SET_FAST_TIMER()) {
		printFail(startFail);
	}

	delay(HARD_TIMER_TEST_DELAY_ELLAPSE_MS);
	if (!DECONSTRUCT_FAST_TIMER()) {
		printFail(deconstructFail);
	}

	TEST_ASSERT_UINT32_WITHIN(HARD_TIMER_FAST_TEST_COUNT_BUFFER, HARD_TIMER_FAST_TEST_COUNT_TARGET, hardTimerCount);
}

void testTimers() {
	RUN_TEST(&testProgramStart);
	RUN_TEST(&testRepeatInit);
	RUN_TEST(&testRepeatStart);
	RUN_TEST(&testRepeatCancel);
	RUN_TEST(&testRepeatDeconstruct);
	RUN_TEST(&testPathFlags);
	RUN_TEST(&testTiming);
	RUN_TEST(&testFastTiming);
}

#else

memCharString passIgnore[] = {"Ignoring timer tests"};

void passTimers() {
	printIgnore(passIgnore);
}

void testTimers() {
	RUN_TEST(&passTimers);
}
#endif