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

#include <osc_common/common_timer.h>

#if NUM_TIMERS > 0

#include <osc_common/common_thread.h>

#define TEST_SLOW_FREQ 10 // frequency for testing slow timing
#ifndef TEST_FAST_FREQ
	#define TEST_FAST_FREQ 10000 // target frequency
#endif

/**
 * frequency for testing setting timer
 * 
 * @note included for microcontrollers where
 * @note all timers can't run TEST_SLOW_FREQ
 */
#define TEST_CASES_FREQ 100

#define SLOW_TIMER_PRIORITY 0 // slow timer priority
#define FAST_TIMER_PRIORITY 255 // fast timer priority

#define TEST_DELAY_ELLAPSE_S 1 // time for timer to run for

#ifndef SLOW_TEST_BUFFER
	#define SLOW_TEST_BUFFER 0 // amount slow timer can be off of goal
#endif
#ifndef FAST_TEST_BUFFER
	#define FAST_TEST_BUFFER 0 // amount fast timer can be off of goal
#endif

/**
 * Tests if timer is invalid timer or not
 */
#define TIMER_NOT_INVALID(timer) \
	if (timer != HARD_TIMER_INVALID) { \
		printFail(setTimerFail); \
	}

memCharString invalidStartFail[] PROG_FLASH = {"Start State"};
memCharString startFail[] PROG_FLASH = {"Start"};
memCharString noSetTimerFail[] PROG_FLASH = {"No Set Timer"};
memCharString restartFail[] PROG_FLASH = {"Restart"};
memCharString cancelFail[] PROG_FLASH = {"Stop"};
memCharString recancelFail[] PROG_FLASH = {"Restop"};
memCharString allNullFail[] PROG_FLASH = {"All Null"};
memCharString freqNullFail[] PROG_FLASH = {"Freq Null"};
memCharString funcNullFail[] PROG_FLASH = {"Func Null"};

memCharString unclaimInvalidFail[] PROG_FLASH = {"Unclaim Invalid"};
memCharString unclaimNotClaimedFail[] PROG_FLASH = {"Unclaim Unclaimed"};
memCharString claimLoopFail[] PROG_FLASH = {"Claim Loop"};
memCharString allClaimedFail[] PROG_FLASH = {"All Claimed"};
memCharString unclaimLoopFail[] PROG_FLASH = {"Unclaim Loop"};
memCharString notClaimedFail[] PROG_FLASH = {"Not Claimed"};
memCharString isClaimedFail[] PROG_FLASH = {"Is Claimed"};
memCharString didntUnclaimFail[] PROG_FLASH = {"Didn't Unclaim"};
memCharString noStartFail[] PROG_FLASH = {"No Start"};
memCharString claimedActiveFail[] PROG_FLASH = {"Claimed Active"};
memCharString sameTimerFail[] PROG_FLASH = {"Same Timer"};

memCharString freq0Fail[] PROG_FLASH = {"Freq 0"};
memCharString freqMaxFail[] PROG_FLASH = {"Freq Max"};
memCharString notSameTimerFail[] PROG_FLASH = {"Not Same Timer"};
memCharString isStartFail[] PROG_FLASH = {"Is Start"};
memCharString notStartFail[] PROG_FLASH = {"Not Start"};
memCharString setTimerFail[] PROG_FLASH = {"Set Timer"};
memCharString setLoopFail[] PROG_FLASH = {"Set Loop"};
memCharString maxTimerFail[] PROG_FLASH = {"Max Timer"};
memCharString startLoopFail[] PROG_FLASH = {"Start Loop"};
memCharString cancelLoopFail[] PROG_FLASH = {"Cancel Loop"};
memCharString cancelInvalidFail[] PROG_FLASH = {"Cancel Invalid"};
memCharString startedLoopFail[] PROG_FLASH = {"Started Loop"};
memCharString didntStopFail[] PROG_FLASH = {"Didn't Stop"};

/**
 * Priority claim statements
 * P: Priority
 * S: Started
 * N: Not Started
 * C: Claimed
 * U: Unclaimed
 */

memCharString pNotStartedClaimedFail[] PROG_FLASH = {"PNC"};
memCharString pNotStartedClaimedUnequalFail[] PROG_FLASH = {"PNC Unequal"};
memCharString pStartedClaimedFail[] PROG_FLASH = {"PSC"};
memCharString pStartedClaimedUnequalFail[] PROG_FLASH = {"PSC Unequal"};
memCharString pStartedUnclaimedFail[] PROG_FLASH = {"PSU"};
memCharString pStartedUnclaimedUnequalFail[] PROG_FLASH = {"PSU Unequal"};

volatile uint32_t hardTimerCount = 0U;

/**
 * Testing function
 */
hard_timer_return_t RUN_IN_RAM(testTimingFunction) testTimingFunction(hard_timer_param_t emptyParams) {
	hardTimerCount++;
	HARD_TIMER_END();
}

/**
 * Resets all timers to off and unclaimed
 */
void resetTimers() {
	for (int i = 0; i < NUM_TIMERS; i++) {
		unclaimTimer((hard_timer_t)i);
		cancelHardTimer((hard_timer_t)i);
	}
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
	resetTimers();
	for (int i = 0; i < NUM_TIMERS; i++) {
		testGetStartState((hard_timer_t)i, false);
	}
}

/**
 * Tests correct status for repeat starts and cancels
 */
void testRepeat() {
	resetTimers();
	freq_t freq = TEST_CASES_FREQ;
	hard_timer_t timer = HARD_TIMER_INVALID;

	if (!setHardTimer(&timer, &freq, &testTimingFunction, DEFAULT_HARD_TIMER_PRIORITY)) {
		printFail(startFail);
	}
	if (timer == HARD_TIMER_INVALID) {
		printFail(noSetTimerFail);
	}

	hard_timer_t secondTimer = timer;
	freq = TEST_CASES_FREQ;
	if (!setHardTimer(&secondTimer, &freq, &testTimingFunction, DEFAULT_HARD_TIMER_PRIORITY)) {
		printFail(startFail);
	}
	if (timer == secondTimer) {
		printFail(restartFail);
	}

	if (!cancelHardTimer(timer)) {
		printFail(cancelFail);
	}
	if (cancelHardTimer(timer)) {
		printFail(recancelFail);
	}
}

/**
 * Tests claim functionality
 */
void testClaims(void) {

	resetTimers();

	hard_timer_t timer = HARD_TIMER_INVALID;
	freq_t freq = TEST_CASES_FREQ;

	// unclaim
	if (unclaimTimer(timer)) {
		printFail(unclaimInvalidFail);
	}
	if (unclaimTimer(HARD_TIMER0)) {
		printFail(unclaimNotClaimedFail);
	}

	// none claimed
	for (uint8_t i = 0; i < NUM_TIMERS; i++) {
		if (hardTimerClaimed(i)) {
			printFail(notClaimedFail);
		}
	}

	// claim
	for (uint8_t i = 0; i < NUM_TIMERS; i++) {
		if (claimTimer(NULL) == HARD_TIMER_INVALID) {
			printFail(claimLoopFail);
		}
	}
	if (claimTimer(NULL) != HARD_TIMER_INVALID) {
		printFail(allClaimedFail);
	}

	// all claimed
	for (uint8_t i = 0; i < NUM_TIMERS; i++) {
		if (!hardTimerClaimed(i)) {
			printFail(isClaimedFail);
		}
	}

	// remove all claims and test unclaimed
	for (uint8_t i = 0; i < NUM_TIMERS; i++) {
		if (!unclaimTimer(i)) {
			printFail(unclaimLoopFail);
		}
		if (hardTimerClaimed(i)) {
			printFail(didntUnclaimFail);
		}
	}

	// claim active timer
	if (!setHardTimer(&timer, &freq, testTimingFunction, DEFAULT_HARD_TIMER_PRIORITY)) {
		printFail(noStartFail);
	}
	for (uint8_t i = 0; i < NUM_TIMERS - 1; i++) {
		if (claimTimer(NULL) == HARD_TIMER_INVALID) {
			printFail(claimLoopFail);
		}
	}
	if (claimTimer(NULL) != HARD_TIMER_INVALID) {
		printFail(claimedActiveFail);
	}

	// remove all claims
	bool claimed = false;
	for (uint8_t i = 0; i < NUM_TIMERS; i++) {
		if (!unclaimTimer(i)) {
			if (!claimed) {
				claimed = true;
			}
			else {
				printFail(unclaimLoopFail);
			}
		}
	}

	if (!cancelHardTimer(timer)) {
		printFail(cancelFail);
	}
	timer = HARD_TIMER_INVALID;
}

/**
 * Tests start parameters
 */
void testStart(void) {

	resetTimers();

	hard_timer_t timer = HARD_TIMER_INVALID;
	freq_t freq = TEST_CASES_FREQ;

	// null parameters
	if (setHardTimer(NULL, NULL, NULL, DEFAULT_HARD_TIMER_PRIORITY)) {
		printFail(allNullFail);
	}
	TIMER_NOT_INVALID(timer);
	if (setHardTimer(&timer, NULL, testTimingFunction, DEFAULT_HARD_TIMER_PRIORITY)) {
		printFail(freqNullFail);
	}
	TIMER_NOT_INVALID(timer);
	if (setHardTimer(&timer, &freq, NULL, DEFAULT_HARD_TIMER_PRIORITY)) {
		printFail(funcNullFail);
	}
	TIMER_NOT_INVALID(timer);
	if (cancelHardTimer(HARD_TIMER_INVALID)) {
		printFail(cancelInvalidFail);
	}

	// cancel timer
	if (cancelHardTimer(timer)) {
		printFail(cancelFail);
	}
	if (cancelHardTimer(HARD_TIMER0)) {
		printFail(cancelFail);
	}

	// set all timers
	for (uint8_t i = 0; i < NUM_TIMERS; i++) {
		hard_timer_t loopTimer = HARD_TIMER_INVALID;
		freq = TEST_CASES_FREQ;
		if (!setHardTimer(&loopTimer, &freq, testTimingFunction, DEFAULT_HARD_TIMER_PRIORITY)) {
			printFail(setLoopFail);
		}
		if (!hardTimerStarted(loopTimer)) {
			printFail(startedLoopFail);
		}
	}
	if (setHardTimer(NULL, &freq, testTimingFunction, DEFAULT_HARD_TIMER_PRIORITY)) {
		printFail(maxTimerFail);
	}
	for (uint8_t i = 0; i < NUM_TIMERS; i++) {
		if (!cancelHardTimer((hard_timer_t)i)) {
			printFail(cancelLoopFail);
		}
		if (hardTimerStarted((hard_timer_t)i)) {
			printFail(didntStopFail);
		}
	}

	// 0 frequency
	freq = 0;
	if (setHardTimer(&timer, &freq, testTimingFunction, DEFAULT_HARD_TIMER_PRIORITY)) {
		printFail(freq0Fail);
	}
	TIMER_NOT_INVALID(timer);
	if (hardTimerStarted(timer)) {
		printFail(isStartFail);
	}
	if (cancelHardTimer(timer)) {
		printFail(cancelFail);
	}
	if (hardTimerStarted(timer)) {
		printFail(notStartFail);
	}
	timer = HARD_TIMER_INVALID;

	// over max frequency
	freq = FREQ_MAX + 1;
	if (setHardTimer(&timer, &freq, testTimingFunction, DEFAULT_HARD_TIMER_PRIORITY)) {
		printFail(freqMaxFail);
	}
	TIMER_NOT_INVALID(timer);
	if (hardTimerStarted(timer)) {
		printFail(isStartFail);
	}
	if (cancelHardTimer(timer)) {
		printFail(cancelFail);
	}
	if (hardTimerStarted(timer)) {
		printFail(notStartFail);
	}
	timer = HARD_TIMER_INVALID;
}

/**
 * Tests timer priorities are set according
 * to 'hard_timer.h'. Best Timer ignored
 */
void testTimerPriority() {
	
	resetTimers();

	hard_timer_t timer = HARD_TIMER_INVALID;
	hard_timer_t secondTimer = HARD_TIMER_INVALID;
	freq_t freq = TEST_CASES_FREQ;

	// test timer claimed and unstarted
	timer = claimTimer(NULL);
	secondTimer = timer;
	if (!setHardTimer(&secondTimer, &freq, testTimingFunction, DEFAULT_HARD_TIMER_PRIORITY)) {
		printFail(pNotStartedClaimedFail);
	}

	if (timer != secondTimer) {
		printFail(pNotStartedClaimedUnequalFail);
	}

	// test timer claimed and started
	freq = TEST_CASES_FREQ;
	if (setHardTimer(&secondTimer, &freq, testTimingFunction, DEFAULT_HARD_TIMER_PRIORITY)) {
		printFail(pStartedClaimedFail);
	}

	if (timer != secondTimer) {
		printFail(pStartedClaimedUnequalFail);
	}

	// test timer unclaimed and started
	unclaimTimer(secondTimer);

	if (!setHardTimer(&secondTimer, &freq, testTimingFunction, DEFAULT_HARD_TIMER_PRIORITY)) {
		printFail(pStartedUnclaimedFail);
	}

	if (timer == secondTimer) {
		printFail(pStartedUnclaimedUnequalFail);
	}
}

/**
 * Tests timing for hardware timer
 * 
 * @param freq target frequency to run at
 * @param buffer amount actual freq can be off
 * @param priority priority to run timer function at
 */
void testTiming(freq_t freq, uint8_t buffer, timer_priority_t priority) {
	resetTimers();
	hard_timer_t functionTimer = HARD_TIMER_INVALID;

	testGetStartState(functionTimer, false);
	hardTimerCount = 0U;

	if (!setHardTimer(&functionTimer, &freq, &testTimingFunction, priority)) {
		printFail(startFail);
	}

	hardDelayMS(TEST_DELAY_ELLAPSE_S * 1000);

	if (!cancelHardTimer(functionTimer)) {
		printFail(cancelFail);
	}

	TEST_ASSERT_UINT32_WITHIN(buffer, freq, hardTimerCount);
}

/**
 * Tests slow timing accuracy
 */
void testSlowTiming() {
	testTiming(TEST_SLOW_FREQ, SLOW_TEST_BUFFER, SLOW_TIMER_PRIORITY);
}

/**
 * Tests fast timing accuracy
 */
void testFastTiming() {
	testTiming(TEST_FAST_FREQ, FAST_TEST_BUFFER, FAST_TIMER_PRIORITY);
}

void testTimers() {
	RUN_TEST(&testProgramStart);
	RUN_TEST(&testRepeat);
	RUN_TEST(&testClaims);
	RUN_TEST(&testStart);
	RUN_TEST(&testTimerPriority);
	RUN_TEST(&testSlowTiming);
	RUN_TEST(&testFastTiming);
}

#else

memCharString passTimerIgnore[] PROG_FLASH = {"Ignoring timer tests"};

void passTimers() {
	printIgnore(passTimerIgnore);
}

void testTimers() {
	RUN_TEST(&passTimers);
}
#endif