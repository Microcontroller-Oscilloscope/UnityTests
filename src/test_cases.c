/*
	test_cases.c - methodology to test features
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

#include "test_cases.h"

#if !defined(NO_PROGMEM_COPY_SUPPORT)// || defined(NO_PROGMEM_COPY_SUPPORT)

void printIgnore(memCharString *message) {
	TEST_IGNORE_MESSAGE(message);
}

void printFail(memCharString *message) {
	TEST_FAIL_MESSAGE(message);
}

#else

/**
 * Copies flash message to memory
 * 
 * @param message flash message to print
 * @param buffer address to copy to
 * @param messageSize size of message to print
 */
void copyMessage(memCharString *message, char *buffer, uint8_t messageSize) {
	char charByte;
	for (uint8_t i = 0; i < messageSize; i++) {
		charByte = pgm_read_byte_near(message + i);
		buffer[i] = charByte;
	}
}

void printIgnore(memCharString *message) {
	uint8_t messageSize = strlen_P(message) + 1;
	char buffer[messageSize];
	copyMessage(message, buffer, messageSize);
	TEST_IGNORE_MESSAGE(buffer);
}

void printFail(memCharString *message) {
	uint8_t messageSize = strlen_P(message) + 1;
	char buffer[messageSize];
	copyMessage(message, buffer, messageSize);
	TEST_FAIL_MESSAGE(buffer);
}

#endif