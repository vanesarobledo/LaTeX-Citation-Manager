/*
* FILE          : Utilities.cpp
* PROJECT       : SENG1050 Final Project: LaTeX Citation Manager (working title)
* PROGRAMMER    : Vanesa Robledo
* FIRST VERSION : 2025-03-21
* DESCRIPTION   : This file contains user input and console-output related functions
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "Citations.h"

// Define constants
#define INPUT_OPERATION_LENGTH	4

// Main Program Utilities

//
// FUNCTION     : header
// DESCRIPTION  : Prints header to the screen
// PARAMETERS   : none
// RETURNS      : void
//
void header(void) {
	printf("==================================================================\n");
	printf("LaTeX Citation Manager (working title)\n");
	printf("by: Vanesa Robledo\n");
	printf("==================================================================\n");
}

//
// FUNCTION     : menu
// DESCRIPTION  : Prints menu of operations to screen
// PARAMETERS   : none
// RETURNS      : void
//
void menu(void) {
	printf("[0] Import URLs from file\n");
	printf("[1] Add citation\n");
	printf("[2] Update citation\n");
	printf("[3] Remove citation \n");
	printf("[4] Process citations\n");
	printf("[5] Export processed citations\n");
	printf("[6] Exit\n");
}

//
// FUNCTION     : processMenu
// DESCRIPTION  : Prints menu of process citation operations to the screen
// PARAMETERS   : none
// RETURNS      : void
//
void processMenu(void) {
	printf("[1] Add data for all citations\n");
	printf("[2] Sort citations alphabetically\n");
	printf("[3] Attempt web scraping for all citations\n");
	printf("--------------------------------------------\n");
	printf("[4] Finish processing all citations\n");
	printf("[5] Cancel processing citations\n");
}

//
// FUNCTION		:	getMenuNum
// DESCRIPTION	:	Asks the user to input a number for menu operation
// PARAMETERS	:	int* menuNum	:	Integer of menu operation
// RETURNS		:	void
//
void getMenuNum(int* menuNum) {
	char input[INPUT_OPERATION_LENGTH] = ""; // Buffer to store input

	// Valid size of input
	const int kAllowedInputLength = 2;

	// Flag to loop getting a valid menu number
	bool loopFlag = true;

	// Ask user for menu number
	while (loopFlag) {
		printf("Enter your choice: ");
		fgets(input, sizeof(input), stdin);

		// If input is too large or not an integer
		if (strlen(input) > kAllowedInputLength || sscanf_s(input, "%d", menuNum) == 0) {
			printf("Error: Please enter a valid menu number.\n");
		}
		// Input is valid, exit loop
		else {
			loopFlag = false;
		}
	}
}

//
// FUNCTION		:	pressEntertoContinue
// DESCRIPTION	:	Waits for user input, and then clears the screen.
// PARAMETERS	:	none
// RETURNS		:	void
//
void pressEntertoContinue(void) {
	printf("\nPress enter to continue...");
	char input = getchar();
	system("cls");
}

//
// FUNCTION		:	exitProgram
// DESCRIPTION	:	Calls freeMemory() to free all dynamically-allocated memory
// PARAMETERS   :	CitationManager* Citations	: Hash table containing citations
//					Queue* CitationsToProcess	: Queue to store citations that need to be processed
//					Stack* ProcessedCitations	: Stack of citations that have been processed
//					Citation* Head				: Pointer to head of sorted linked list
// RETURNS		:	
//
void exitProgram(CitationManager* Citations, Queue* CitationsToProcess, Stack* ProcessedCitations, Citation* Head) {
	freeMemory(Citations, CitationsToProcess, ProcessedCitations, Head);
	printf("Exiting program...\n");
	exit(EXIT_SUCCESS);
}

// Input Functions

//
// FUNCTION		:	inputURL
// DESCRIPTION	:	Prompts the user to enter a URL and validates it
// PARAMETERS	:	void
// RETURNS		:	char*
//
char* inputURL(void) {
	bool validData = false; // Flag for whether input was valid
	char tempBuffer[LINE_SIZE] = ""; // Buffer for user input
	char* buffer = NULL;
	char* url;

	while (!validData) {
		printf("Enter URL (or press enter to cancel): ");
		fgets(tempBuffer, LINE_SIZE, stdin);
		clearNewLineChar(tempBuffer);
		buffer = trimWhitespace(tempBuffer);

		// Validate user input
		if (strlen(buffer) == 0) { // Break if only enter was pressed
			url = _strdup("");
			return url;
		}
		else if (std::regex_match(buffer, std::regex("^https?:\\/\\/[A-za-z\\.0-9\\/_\\-\\:\\#\\[\\]\\@\\!\\$\\&\\'\\(\\)\\*\\+\\,\\;\\%\\=]+")) == false) {
			printf("Invalid URL entered.\n");
		}
		else {
			url = _strdup(buffer);
			return url;
			validData = true;
		}
	}
}


//
// FUNCTION		:	inputTitle
// DESCRIPTION	:	Prompts the user to enter a title for the website and validates it
// PARAMETERS	:	char* title	:	Pointer to string containing website title
// RETURNS		:	void
//
char* inputTitle() {
	bool validData = false; // Flag for whether input was valid
	char tempBuffer[LINE_SIZE] = ""; // Buffer for user input
	char* buffer = NULL;
	char* title;

	while (!validData) {
		printf("Enter title of website: ");
		fgets(tempBuffer, LINE_SIZE, stdin);
		clearNewLineChar(tempBuffer);
		buffer = trimWhitespace(tempBuffer);

		// Validate user input
		if (strlen(buffer) == 0) {
			printf("Title cannot be empty.\n");
		}
		else {
			title = _strdup(buffer);
			return title;
			validData = true;
		}
	}
	return buffer;
}

//
// FUNCTION		:	inputAuthor
// DESCRIPTION	:	Prompts the user to enter an author or organization for the website and validates it
//					(Author or organization can be left blank)
// PARAMETERS	:	char* author	:	Pointer to string containing website author or organization
// RETURNS		:	void
//
char* inputAuthor() {
	bool validData = false; // Flag for whether input was valid
	char tempBuffer[LINE_SIZE] = ""; // Buffer for user input
	char* buffer = NULL;
	char* author;

	while (!validData) {
		printf("Enter author or organization of website (optional): ");
		fgets(tempBuffer, LINE_SIZE, stdin);
		clearNewLineChar(tempBuffer);
		buffer = trimWhitespace(tempBuffer);

		// Validate user input
		if (strlen(buffer) == 0) {
			printf("Author or organization left blank.\n");
			author = _strdup(buffer);
			return author;
		}
		else {
			author = _strdup(buffer);
			return author;
			validData = true;
		}
	}
	return buffer;
}

//
// FUNCTION		:	inputYear
// DESCRIPTION	:	Prompts user to input a year and validates it
// PARAMETERS	:	int* year	:	Pointer to int containing year
// RETURNS		:	void
//
int inputYear() {
	bool validData = true; // Flag for whether input was valid
	char inputBuffer[LINE_SIZE] = ""; // Buffer to store input
	char* buffer;
	int year = 0;

	// Ask user for menu number
	while (validData) {
		printf("Enter year (YYYY): ");
		fgets(inputBuffer, sizeof(inputBuffer), stdin);
		buffer = trimWhitespace(inputBuffer);

		if (strlen(buffer) == 0) {
			printf("Year left blank.\n");
			return year;
		}

		// If input is not a valid integer
		if (sscanf_s(buffer, "%d", &year) == 0 || year <= 0) {
			printf("Invalid year. Please enter in YYYY format.\n");
		}

		// If year is invalid format
		else if (std::regex_match(buffer, std::regex("[0-9]{4}")) == false) {
			printf("Invalid year. Please enter in YYYY format.\n");
		}

		else {
			validData = false;
		}
	}

	return year;
}

//
// FUNCTION     : compareStrings
// DESCRIPTION  : Compares two strings and returns true if the first string comes alphabetically before
//				  the second string and false if not
// PARAMETERS   : char* str1 : First string to compare
//				  char* str2 : Second string to compare
// RETURNS      : bool
//
bool compareStrings(char* str1, char* str2) {
	bool alphabetical = false; // Return if str1 < str2

	// Convert strings into char arrays
	char string1[LINE_SIZE];
	strncpy(string1, str1, LINE_SIZE);
	nullTerminate(string1);

	char string2[LINE_SIZE];
	strncpy(string2, str2, LINE_SIZE);
	nullTerminate(string2);

	// Store lengths of strings
	int stringLength1 = (int)strlen(string1);
	int stringLength2 = (int)strlen(string2);
	int length = 0;

	// Set length to iterate through to shorter length of string
	if (stringLength1 >= stringLength2) {
		length = stringLength1;
	}
	else {
		length = stringLength2;
	}

	// Compare both strings
	for (int i = 0; i < length; i++) {
		// Find first different letter and break
		if (string1[i] != string2[i]) {
			// If the current URL letter is 'greater' (alphabetically later) than next string, change diff flag to true
			if (string1[i] > string2[i]) {
				alphabetical = true;
			}
			break;
		}
	}

	return alphabetical;
}

// Input Functions

//
// FUNCTION		: nullTerminate
// DESCRIPTION	: Ensures string is null-terminated
// PARAMETERS	: char* str		:	String to be modified
// RETURNS		: None
//
void nullTerminate(char* str) {
	str[strlen(str)] = '\0';
}

//
// FUNCTION		: clearNewLineChar
// DESCRIPTION	: Removes the trailing newline character \n from fgets input
// PARAMETERS	: char* str		:	String to be modified
// RETURNS		: None
//
void clearNewLineChar(char* str) {
	if (str[strlen(str) - 1] == '\n') {
		str[strlen(str) - 1] = '\0';
	}
}

//
// FUNCTION		: trimURL
// DESCRIPTION	: Trims the protocol (http://|https://)(www?) from URLs
// PARAMETERS	: char* str		:	String to be modified
// RETURNS		: char*			:	Trimmed string
//
char* trimURL(char* str) {
	// Use <regex> to capture second group
	std::regex rgx("(?:https:\\/\\/|http:\\/\\/)(?:www.)?([A-za-z0-9\\.\\/_\\-\\:\\#\\[\\]\\@\\!\\$\\&\\'\\(\\)\\*\\+\\,\\;\\%\\=]+)");
	std::cmatch match;
	char* trimmedURL;

	if (std::regex_search(str, match, rgx)) {
		trimmedURL = _strdup(match[1].first);
		return trimmedURL;
	}
	else {
		return str;
	}
}

//
// FUNCTION		: trimWhitespace
// DESCRIPTION	: Trims any leading and/or trailing whitespace in a string
// PARAMETERS	: char* str		:	String to be modified
// RETURNS		: char*			:	Trimmed string
//
char* trimWhitespace(char* str) {
	int strLength = (int)strlen(str); // Store length of string
	int trimmedStrLength = 0; // Store new length of string

	// Check if string is empty
	if (strLength == 0) {
		return str;
	}

	// Remove leading whitespace
	while (isspace(*str)) {
		str++; // Increment pointer of string to skip leading spaces
	}
	trimmedStrLength = (int)strlen(str); // Calculate new length of string

	// Remove trailing whitespace
	while (isspace(str[trimmedStrLength - 1]) && trimmedStrLength > 0) {
		trimmedStrLength--; // While last character is a space, decrement string length
	}

	// Null-terminate string at first non-space character from end of string
	str[trimmedStrLength] = '\0';

	return str;
}

//
// FUNCTION     : freeMemory
// DESCRIPTION  : Frees all dynamically allocated memory in the program
// PARAMETERS   : CitationManager* Citations	: Hash table containing citations
//				  Queue* CitationsToProcess		: Queue to store citations that need to be processed
//				  Stack* ProcessedCitations		: Stack of citations that have been processed
//				  Citation* Head				: Pointer to head of sorted linked list
// RETURNS      : void
//
void freeMemory(CitationManager* Citations, Queue* CitationsToProcess, Stack* ProcessedCitations, Citation* Head) {
	printf("Freeing dynamically allocated memory...\n");
	FreeQueue(CitationsToProcess);
	FreeStack(ProcessedCitations);
	FreeSortedLinkedList(Head);
	FreeHashTable(Citations);
	printf("Memory cleanup complete.\n");
}