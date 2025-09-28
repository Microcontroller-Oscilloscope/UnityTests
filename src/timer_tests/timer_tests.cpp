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

#include <timer.h>

#define HARD_TIMER_TEST_INDEX HARD_TIMER_LED_INDEX // hardware timer index for testing
#define HARD_TIMER_TEST HARD_TIMER(HARD_TIMER_TEST_INDEX) // hardware timer for testing
#define HARD_TIMER_TEST_FUNCTION() HARD_TIMER_FUNCTION(HARD_TIMER_TEST_INDEX) // starter function for testing
#define HARD_TIMER_TEST_REFERENCE HARD_TIMER_REFERENCE(HARD_TIMER_TEST_INDEX) // reference for testing function
#define HARD_TIMER_TEST_SCALAR HARD_TIMER_LED_SCALAR // pre scalar for testing
#define HARD_TIMER_TEST_MULTIPLIER HARD_TIMER_LED_TICK_MULTIPLIER // multiplier for testing

#define HARD_TIMER_TEST_DELAY_MS 100 // delay for each iteration of timer

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

/**
 * Testing function
 */
HARD_TIMER_TEST_FUNCTION() {
	HARD_TIMER_END();
}

/**
 * Tests timer states
 * 
 * @param timer timer to test
 * @param init whether timer should or shouldn't be initialized
 * @param start whether timer should or shouldn't be started
 */
void testGetState(hardware_timer_t timer, bool init, bool start) {

	if (timerInitialized(timer) != init) {
		printFail(invalidInitFail);
	}
	if (timerStarted(timer) != start) {
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

void testTimers() {
	RUN_TEST(&testProgramStart);
	RUN_TEST(&testRepeatInit);
	RUN_TEST(&testRepeatStart);
	RUN_TEST(&testRepeatCancel);
	RUN_TEST(&testRepeatDeconstruct);
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