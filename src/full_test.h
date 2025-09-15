/*
	full_test.h - tests all functionality
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

#ifndef FULL_TEST_H
#define FULL_TEST_H

#include "test_cases.h"
#include <Arduino.h>
#include <unity.h>

void fullTest(void);

/****************************
 * NVM Size Config
****************************/

#include "nvm_tests/nvm_tests.h"

// tests if final key is found
#ifndef TEST_NVM_END_KEY
	#error TEST_NVM_END_KEY not found
#endif

#define NVM_TEST_SIZE TEST_NVM_END_KEY

#if NVM_TEST_SIZE > NVM_SIZE
	#error NVM_SIZE not large enough for testing
#endif

#endif