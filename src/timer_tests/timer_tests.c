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

#define TEST_SLOW_FREQ 10 // frequency for testing slow timing
#ifndef TEST_FAST_FREQ
	#define TEST_FAST_FREQ 10000
#endif

#define SET_SLOW_TIMER(timer, freq) setHardTimer(&timer, &freq, &testTimingFunction, 0)
#define SET_FAST_TIMER(timer, freq) setHardTimer(&timer, &freq, &testTimingFunction, 255)
#define CANCEL_TIMER(timer) cancelHardTimer(timer)

#define TEST_DELAY_ELLAPSE_MS 1000 // time for timer to run for

#ifndef SLOW_TEST_BUFFER
	#define SLOW_TEST_BUFFER 0 // amount slow timer can be off of goal
#endif
#ifndef FAST_TEST_BUFFER
	#define FAST_TEST_BUFFER 1 // amount fast timer can be off of goal
#endif

memCharString invalidStartFail[] PROG_FLASH = {"Start State"};
memCharString startFail[] PROG_FLASH = {"Start"};
memCharString restartFail[] PROG_FLASH = {"Restart"};
memCharString cancelFail[] PROG_FLASH = {"Stop"};
memCharString recancelFail[] PROG_FLASH = {"Restop"};

volatile uint32_t hardTimerCount = 0U;

/**
 * Testing function
 */
hard_timer_return_t RUN_IN_RAM(testTimingFunction) testTimingFunction(hard_timer_param_t emptyParams) {
	hardTimerCount++;
	HARD_TIMER_END();
}

/**
 * Tests timer start state
 * 
 * @param timer timer to test
 * @param start whether timer should or shouldn't be started
 */
void testGetStartState(hard_timer_t timer, bool start) {
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
		testGetStartState((hard_timer_t)i, false);
	}
}

/**
 * Tests correct status for repeat starts and cancels
 */
void testRepeat() {

	freq_t freq = TEST_SLOW_FREQ;
	hard_timer_t timer = HARD_TIMER_INVALID;

	if (!SET_SLOW_TIMER(timer, freq)) {
		printFail(startFail);
	}
	if (SET_SLOW_TIMER(timer, freq)) {
		printFail(restartFail);
	}
	if (!CANCEL_TIMER(timer)) {
		printFail(cancelFail);
	}
	if (CANCEL_TIMER(timer)) {
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

	hard_timer_t timer = HARD_TIMER_INVALID;
	freq_t freq = TEST_SLOW_FREQ;

	testGetStartState(timer, false);
	hardTimerCount = 0U;

	if (!SET_SLOW_TIMER(timer, freq)) {
		printFail(startFail);
	}

	#ifdef MILLIS_DELAY
		delay(TEST_DELAY_ELLAPSE_MS);
	#else
		hardDelayMS(TEST_DELAY_ELLAPSE_MS);
	#endif

	if (!CANCEL_TIMER(timer)) {
		printFail(cancelFail);
	}

	TEST_ASSERT_UINT32_WITHIN(SLOW_TEST_BUFFER, TEST_SLOW_FREQ, hardTimerCount);
}

/**
 * Tests fast timing accuracy
 */
void testFastTiming() {

	hard_timer_t timer = HARD_TIMER_INVALID;
	freq_t freq = TEST_FAST_FREQ;

	testGetStartState(timer, false);
	hardTimerCount = 0U;
	volatile uint32_t result = 0U;

	if (!SET_FAST_TIMER(timer, freq)) {
		printFail(startFail);
	}

	#ifdef MILLIS_DELAY
		delay(TEST_DELAY_ELLAPSE_MS);
	#else
		hardDelayMS(TEST_DELAY_ELLAPSE_MS);
	#endif

	result = hardTimerCount;
	if (!CANCEL_TIMER(timer)) {
		printFail(cancelFail);
	}

	TEST_ASSERT_UINT32_WITHIN(FAST_TEST_BUFFER, freq, result);
}

void testTimers() {
	RUN_TEST(&testProgramStart);
	RUN_TEST(&testRepeat);
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