/*
* FILE          : Import.cpp
* PROJECT       : SENG1050 Final Project: LaTeX Citation Manager (working title)
* PROGRAMMER    : Vanesa Robledo
* FIRST VERSION : 2025-03-21
* DESCRIPTION   : This file contains the main file import and storing data
*/

#include "Citations.h"

//
// FUNCTION     : LoadFile
// DESCRIPTION  : Prompts a user to enter a file name and returns a pointer to the file
// PARAMETERS   : none
// RETURNS      : FILE*
//
FILE* LoadFile(void) {
	FILE* file = NULL; // Initialize file pointer
	char filename[LINE_SIZE] = ""; // Name of file
	char buffer[LINE_SIZE] = ""; // Buffer for user input
	bool validFileName = false; // Flag for valid input

	while (!validFileName)
	{
		// Prompt user for file name
		printf("Enter name of file: ");
		fgets(buffer, LINE_SIZE, stdin);

		// Validate input
		if (sscanf(buffer, "%s", &filename) == 0) {
			printf("Invalid input. Please try again.\n");
		}
		// Input is valid, break loop
		else {
			validFileName = true;
		}
	}

	// Open file for reading safely
	errno_t err;
	err = fopen_s(&file, filename, "r+");

	// Print message if there is failure to open file
	if (err != 0) {
		perror("Error opening file.");
	}

	return file;
}

//
// FUNCTION     : StoreFileData
// DESCRIPTION  : Reads URLs from a file and stores them as citation nodes in hash table & queue of citations to process
// PARAMETERS   : FILE* file				 :	Pointer to file containing website URLs
//				  CitationManager* Citations : Hash table containing citations
//				  Queue* CitationsToProcess	 : Queue to store citations that need to be processed
// RETURNS      : void
//
void StoreFileData(FILE* file, CitationManager* Citations, Queue* CitationsToProcess) {
	// Ensure pointer to file is valid
	if (file == NULL) {
		printf("Error reading file.\n");
		return;
	}

	char buffer[LINE_SIZE] = ""; // String to store line from file
	char* readLine; // String to store line to add to node

	while (fgets(buffer, LINE_SIZE, file) != NULL) {
		// Stop reading file if some file error occurs
		if (ferror(file)) {
			printf("Error reading file.\n");
			return;
		}
		// Read from file
		else
		{
			clearNewLineChar(buffer);
			readLine = trimWhitespace(buffer);
			// Validate input from file - read valid URLs
			if (std::regex_match(buffer, std::regex("^https?:\\/\\/[A-za-z\\.0-9\\/_\\-\\:\\#\\[\\]\\@\\!\\$\\&\\'\\(\\)\\*\\+\\,\\;\\%\\=]+")) == true) {
				strncpy(readLine, buffer, LINE_SIZE);
				// Add data to data structures
				InsertData(Citations, CitationsToProcess, readLine);
			}
		}
	}

	// Close the file safely
	if (fclose(file) != 0) {
		printf("Error closing file.\n");
		return;
	}

	// Print if data was loaded from file
	if (Citations->Table != NULL) {
		printf("Data successfully loaded from file.\n");
	}
	else
	{
		printf("No data loaded from file.\n\n");
	}
}

//
// FUNCTION     : InsertData
// DESCRIPTION  : Initializes a citation node for a URL and inserts it into hash table & enqueues it for processesing
// PARAMETERS   : CitationManager* Citations : Hash table containing citations
//				  Queue* CitationsToProcess: Queue to store citations that need to be processed
//				  const char* url : URL of website to be stored
// RETURNS      : void
//
void InsertData(CitationManager* Citations, Queue* CitationsToProcess, const char* url) {
	// Create citation node with URL
	Citation* newCitation = InitializeCitation(url);

	// Add citation to hash table and queue of citations to process
	if (InsertHashTable(Citations, newCitation)) {
		Enqueue(CitationsToProcess, newCitation);
	}
	else {
		free(newCitation);
	}
}

//
// FUNCTION     : importCitationsFile
// DESCRIPTION  : Opens a file given a filename & stores data to queue
// PARAMETERS   : FILE* file				: Pointer to file containing website URLs
//				  Queue* CitationsToProcess	: Queue to store citations that need to be processed
//				  const char* url			: URL of website to be stored
// RETURNS      : void
//
void importCitationsFile(FILE* ImportFile, Queue* CitationsToProcess, const char* filename) {
	// Open file for reading safely
	errno_t err;
	err = fopen_s(&ImportFile, filename, "r+");

	// Print message if there is failure to open file
	if (err != 0) {
		perror("Error opening file.");
		exit(EXIT_FAILURE);
	}

	char buffer[LINE_SIZE] = ""; // String to store line from file
	char* readLine; // String to store line to add to node

	Citation* newCitation;

	while (fgets(buffer, LINE_SIZE, ImportFile) != NULL) {
		// Stop reading file if some file error occurs
		if (ferror(ImportFile)) {
			printf("Error reading file.\n");
			return;
		}
		// Read from file
		else
		{
			clearNewLineChar(buffer);
			readLine = trimWhitespace(buffer);
			// Validate input from file - read valid URLs
			if (std::regex_match(buffer, std::regex("^https?:\\/\\/[A-za-z\\.0-9\\/_\\-\\:\\#\\[\\]\\@\\!\\$\\&\\'\\(\\)\\*\\+\\,\\;\\%\\=]+")) == true) {
				strncpy(readLine, buffer, LINE_SIZE);
				// Add data to queue
				newCitation = InitializeCitation(readLine);
				Enqueue(CitationsToProcess, newCitation);
			}
		}
	}

	// Close the file safely
	if (fclose(ImportFile) != 0) {
		printf("Error closing file.\n");
		return;
	}
}