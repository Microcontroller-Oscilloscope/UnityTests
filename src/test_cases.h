/*
	test_cases.h - methodology to test features
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

#ifndef TEST_CASES_H
#define TEST_CASES_H

//#include "compile_flags.h"

#ifndef __TEST_CASES__
/**
 * Undefine only when writing code
 */
//#define __TEST_CASES__
#endif

/**
 * Removes debug statements from displaying and consuming resources
 */
#ifdef __TEST_CASES__

#undef __ERROR_DEBUG__
#undef __NVM_DEBUG__
#undef __TAG_DEBUG__

// delay to allow serial connection to board
#define SERIAL_DELAY 2000

#endif
#endif