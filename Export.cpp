/*
* FILE          : Export.cpp
* PROJECT       : SENG1050 Final Project: LaTeX Citation Manager (working title)
* PROGRAMMER    : Vanesa Robledo
* FIRST VERSION : 2025-03-21
* DESCRIPTION   : This file contains the exporting to file functions
*/

#include "Citations.h"

//
// FUNCTION     : SaveFile
// DESCRIPTION  : Exports processed citations LaTeX formatted file
// PARAMETERS   : FILE* file				 : Pointer to file to save data
//				  CitationManager* Citations : Hash table containing citations
//				  Stack* ProcessedCitations  : Stack of citations that have been processed
// RETURNS      : void
//
void SaveFile(FILE* file, CitationManager* Citations, Stack* ProcessedCitations) {
	// If there are no processed citations, do not write to file
	if (ProcessedCitations == NULL || isStackEmpty(ProcessedCitations)) {
		printf("No processed citations are stored.\n");
		return;
	}

	char filename[LINE_SIZE] = ""; // Name of file
	char buffer[LINE_SIZE] = ""; // Buffer for user input
	char ext[] = ".bib"; // File extension
	bool validFileName = false; // Flag for valid input

	// Prompt user for file name
	while (!validFileName)
	{
		printf("Enter name of file (before .bib): ");
		fgets(buffer, LINE_SIZE, stdin);

		// Validate input
		if (sscanf(buffer, "%s", &filename) == 0) {
			printf("Invalid input. Please try again.\n");
		}
		else {
			// Concatenate file extension and file name
			strcat(filename, ext);
			validFileName = true;
		}
	}

	// Open file for writing safely or creates it if it does not exist
	errno_t err;
	err = fopen_s(&file, filename, "w+");

	// Exit if there is a failure to open file
	if (err != 0) {
		perror("Error opening file.");
		exit(EXIT_FAILURE);
	}

	// Iterate through stack and write to file
	Citation* current = ProcessedCitations->Top;
	Citation* next = NULL;
	int index = 0; // Create unique citekey by adding counter
	char bib[BIB_SIZE]; // Store string to write to file

	while (current != NULL) {
		if (current->Year != 0) {
			sprintf_s(bib, BIB_SIZE, "@online{WebsiteCiteKey%d,\n\tauthor = {%s},\n\ttitle = {%s},\n\tyear = {%d},\n\turl = {%s},\n\turldate = {%s}\n}\n", index, current->Author, current->Title, current->Year, current->URL, current->DateAccessed);
			fprintf(file, bib);
		}
		else {
			sprintf_s(bib, BIB_SIZE, "@online{WebsiteCiteKey%d,\n\tauthor = {%s},\n\ttitle = {%s},\n\tyear = {},\n\turl = {%s},\n\turldate = {%s}\n}\n", index, current->Author, current->Title, current->URL, current->DateAccessed);
			fprintf(file, bib);
		}

		index++; // Increase counter for citekey
		next = current->Next; // Store next pointer

		// Free memory
		DeleteHashTable(Citations, SearchKVPHashTable(Citations, current->URL)); // Delete citation from hash table
		current = Pop(ProcessedCitations); // Pop the citation from the stack after processing
		if (current != NULL) {
			free(current);
		}

		current = next; // Move pointer
	}

	// Close the file safely
	if (fclose(file) != 0) {
		printf("Error closing file.\n");
		return;
	}

	printf("Data saved to file successfully.\n");
}

//
// FUNCTION     : exportCitationsFile
// DESCRIPTION  : Saves data from queue to a given filename
// PARAMETERS   : FILE* file				: Pointer to export file for citations
//				  Queue* CitationsToProcess	: Queue to store citations that need to be processed
//				  const char* url			: URL of website to be stored
// RETURNS      : void
//
void exportCitationsFile(FILE* ExportFile, Queue* CitationsToProcess, const char* filename) {
	// Open file for writing safely or creates it if it does not exist
	errno_t err;
	err = fopen_s(&ExportFile, filename, "w+");

	// Exit if there is a failure to open file
	if (err != 0) {
		perror("Error opening file.");
		exit(EXIT_FAILURE);
	}

	// Iterate through queue and write to file
	Citation* current = CitationsToProcess->Front;
	Citation* next = NULL;
	int index = 0; // Create unique citekey by adding counter
	char bib[BIB_SIZE]; // Store string to write to file

	while (current != NULL) {
		if (current->Year != 0) {
			sprintf_s(bib, BIB_SIZE, "@online{WebsiteCiteKey%d,\n\tauthor = {%s},\n\ttitle = {%s},\n\tyear = {%d},\n\turl = {%s},\n\turldate = {%s}\n}\n", index, current->Author, current->Title, current->Year, current->URL, current->DateAccessed);
			fprintf(ExportFile, bib);
		}
		else {
			sprintf_s(bib, BIB_SIZE, "@online{WebsiteCiteKey%d,\n\tauthor = {%s},\n\ttitle = {%s},\n\tyear = {},\n\turl = {%s},\n\turldate = {%s}\n}\n", index, current->Author, current->Title, current->URL, current->DateAccessed);
			fprintf(ExportFile, bib);
		}

		index++; // Increase counter for citekey
		next = current->Next; // Store next pointer
		Dequeue(CitationsToProcess); // Dequeue the citation from the stack after processing
		current = next; // Move pointer
	}

	// Close the file safely
	if (fclose(ExportFile) != 0) {
		printf("Error closing file.\n");
		return;
	}
}