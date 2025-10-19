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

#include "timer_tests.h"
#include "../test_cases.h"

#if NUM_TIMERS > 0

#include <compile_flags/compile_flags.h>
#include <hard_timer.h>
#include <board_common.h>

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

/****************************
 * Fast Timer Defines
****************************/

#define HARD_TIMER_FAST_TEST HARD_TIMER(HARD_TIMER_FAST_TEST_INDEX) // hardware timer for testing
#define HARD_TIMER_FAST_TEST_FUNCTION() HARD_TIMER_FUNCTION(HARD_TIMER_FAST_TEST_INDEX) // starter function for testing
#define HARD_TIMER_FAST_TEST_REFERENCE HARD_TIMER_REFERENCE(HARD_TIMER_FAST_TEST_INDEX) // reference for testing function

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

memCharString invalidStartFail[] PROG_FLASH = {"Start State"};
memCharString startFail[] PROG_FLASH = {"Start"};
memCharString restartFail[] PROG_FLASH = {"Restart"};
memCharString cancelFail[] PROG_FLASH = {"Stop"};
memCharString recancelFail[] PROG_FLASH = {"Restop"};

volatile uint32_t hardTimerCount = 0U;

/**
 * Testing function
 */
HARD_TIMER_TEST_FUNCTION() {
	hardTimerCount++;
	HARD_TIMER_END();
}

/**
 * Tests timer start state
 * 
 * @param timer timer to test
 * @param start whether timer should or shouldn't be started
 */
void testGetStartState(hardware_timer_t timer, bool start) {
	if (hardTimerStarted(timer) != start) {
		printFail(invalidStartFail);
	}
}

/**
 * Tests that no timers are set when starting
 * 
 * @warning Only call when timers aren't started nor initialized
 */
void testProgramStart() {
	for (int i = 0; i < NUM_TIMERS; i++) {
		testGetStartState((hardware_timer_t)i, false);
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

#if defined(PLATFORMIO) && defined(UNOR3)
	#include <Arduino.h>
	#define MILLIS_DELAY
#endif

/**
 * Tests slow timing accuracy
 */
void testTiming() {

	testGetStartState(HARD_TIMER_TEST, false);
	hardTimerCount = 0U;

	if (!SET_TIMER()) {
		printFail(startFail);
	}

	#ifdef MILLIS_DELAY
		delay(HARD_TIMER_TEST_DELAY_ELLAPSE_MS);
	#else
		hardDelayMS(HARD_TIMER_TEST_DELAY_ELLAPSE_MS);
	#endif

	if (!CANCEL_TIMER()) {
		printFail(cancelFail);
	}

	TEST_ASSERT_UINT32_WITHIN(HARD_TIMER_TEST_COUNT_BUFFER, HARD_TIMER_TEST_COUNT_TARGET, hardTimerCount);
}

/**
 * Tests fast timing accuracy
 */
void testFastTiming() {

	testGetStartState(HARD_TIMER_TEST, false);
	hardTimerCount = 0U;
	volatile uint32_t result = 0U;

	if (!SET_FAST_TIMER()) {
		printFail(startFail);
	}

	#ifdef MILLIS_DELAY
		delay(HARD_TIMER_TEST_DELAY_ELLAPSE_MS);
	#else
		hardDelayMS(HARD_TIMER_TEST_DELAY_ELLAPSE_MS);
	#endif

	result = hardTimerCount;
	if (!CANCEL_FAST_TIMER()) {
		printFail(cancelFail);
	}

	TEST_ASSERT_UINT32_WITHIN(HARD_TIMER_FAST_TEST_COUNT_BUFFER, HARD_TIMER_FAST_TEST_COUNT_TARGET, result);
}

void testTimers() {
	RUN_TEST(&testProgramStart);
	RUN_TEST(&testRepeatStart);
	RUN_TEST(&testRepeatCancel);
	RUN_TEST(&testTiming);
	RUN_TEST(&testFastTiming);
}

#else

memCharString passIgnore[] PROG_FLASH = {"Ignoring timer tests"};

void passTimers() {
	printIgnore(passIgnore);
}

void testTimers() {
	RUN_TEST(&passTimers);
}
#endif