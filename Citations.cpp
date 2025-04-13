/*
* FILE          : Citations.cpp
* PROJECT       : SENG1050 Final Project: LaTeX Citation Manager
* PROGRAMMER    : Vanesa Robledo
* FIRST VERSION : 2025-03-21
* DESCRIPTION   : LaTeX Citation Manager is a console-based menu-driven application that allows users to import or add
*				  website citations by URL. The application will prompt the user to enter information necessary to
*				  complete each citation and allows the user to export their entire bibliography for a .tex file for
*				  use in BibLaTeX.
*				  This file contains the main input and validation functions for storing citations
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "Citations.h"
#include "WebScraping.h"

// Citation Functions

//
// FUNCTION     : InitializeCitation
// DESCRIPTION  : Dynamically allocates memory for a Citation struct
// PARAMETERS   : const char* url : URL of website to be stored
// RETURNS      : Citation*
//
Citation* InitializeCitation(const char* url) {
    Citation* newCitation = (Citation*)malloc(sizeof(Citation));

    if (newCitation == NULL) {
        printf("Insufficient memory to create citation. Exiting program...\n");
        exit(EXIT_FAILURE);
    }

    // Initialize timestamp
    time_t now = time(NULL);
    char dateAccessed[TIMESTAMP];
    strftime(dateAccessed, sizeof(dateAccessed), "%Y-%m-%d", localtime(&now));

    // Initialize values for citation
    newCitation->Author = _strdup("");
    newCitation->Title = _strdup("");
    newCitation->Year = 0;
    newCitation->Next = NULL;

    // Store values in citation
    newCitation->URL = _strdup(url);
    strncpy(newCitation->DateAccessed, dateAccessed, TIMESTAMP);

    return newCitation;
}

//
// FUNCTION     : printCitation
// DESCRIPTION  : Prints a citation struct data to the screen
// PARAMETERS   : Citation*	citation : Pointer to citation struct
// RETURNS      : void
//
void printCitation(Citation* citation) {
	// If invalid, return
	if (citation == NULL) {
		printf("Error: Invalid citation memory.\n");
		return;
	}

	// Print citation to screen
	printf("URL: %s\n", citation->URL);
	if (strlen(citation->Title) > 0) {
		printf("Author: %s\n", citation->Author);
	}
	else {
		printf("Author: [blank]\n");
	}
	if (strlen(citation->Title) > 0) {
		printf("Title: %s\n", citation->Title);
	}
	else {
		printf("Title: [blank]\n");
	}
	if (citation->Year > 0) {
		printf("Year: %d\n", citation->Year);
	}
	else {
		printf("Year: [blank]\n");
	}
	printf("Date Accessed: %s\n", citation->DateAccessed);
}

//
// FUNCTION     : printAllCitations
// DESCRIPTION  : Prints all citations given a citation node
// PARAMETERS   : Citation*	citation : Pointer to citation struct
// RETURNS      : void
//
void printAllCitations(Citation* citation) {
	// Iterate through all citations
	Citation* current = citation;
	Citation* next;

	while (current != NULL) {
		// Store next pointer
		next = current->Next;
		// Print citation
		printCitation(current);
		printf("---------------------------------------------\n");
		// Move next pointer
		current = next;
	}
}

// User Menu Functions

//
// FUNCTION		:	importCitations
// DESCRIPTION	:	Imports a file of URLs using ImportFile and stores them in the hash table and queue of citations to process
// PARAMETERS	:	FILE* ImportFile			:	File pointer to a file to read from
//					CitationManager* Citations	:	Hash table containing citations
//					Queue* CitationsToProcess	:	Queue to store citations that need to be processed
// RETURNS		:	void
//
void importCitations(FILE* ImportFile, CitationManager* Citations, Queue* CitationsToProcess) {
	printf("\nImporting Citations:\n");
	printf("--------------------\n");
	ImportFile = LoadFile();
	if (ImportFile != NULL) {
		StoreFileData(ImportFile, Citations, CitationsToProcess);
	}
}

//
// FUNCTION		:	addCitation
// DESCRIPTION	:	Allows a user to add a citation
// PARAMETERS	:	CitationManager* Citations	:	Hash table containing citations
//					Queue* CitationsToProcess	:	Queue to store citations that need to be processed
// RETURNS		:	void
//
void addCitation(CitationManager* Citations, Queue* CitationsToProcess) {
	// Initialize data to add to citation
	char* URL;
	char* author;
	char* title;
	int year = 0;

	printf("\nAdd a New Citation:\n");
	printf("--------------------\n");

	// Ask user to input URL
	URL = inputURL();

	// If user cancels, stop adding citation and return
	if (strlen(URL) == 0) {
		printf("Cancelled adding citation.\n");
		return;
	}

	// Create citation with data
	Citation* newCitation = InitializeCitation(URL);
	bool insertedSuccessfully = InsertHashTable(Citations, newCitation);

	if (insertedSuccessfully) {
		// Ask user to input other data
		author = inputAuthor();
		title = inputTitle();
		year = inputYear();

		if (newCitation == NULL) {
			printf("Failed to allocate memory for citation. Citation not added.\n");
			return;
		}

		// Assign data to citation
		if (strlen(author) > 0) {
			newCitation->Author = _strdup(author);
		}
		if (strlen(title) > 0) {
			newCitation->Title = _strdup(title);
		}
		if (year != 0) {
			newCitation->Year = year;
		}

		// Add citation to hash table and queue
		Enqueue(CitationsToProcess, newCitation);

		printf("Citation added.\n");
	}
}

//
// FUNCTION		:	updateCitation
// DESCRIPTION	:	Allows a user to look up a citation by URL to update its data
// PARAMETERS	:	CitationManager* Citations	:	Hash table containing citations
// RETURNS		:	void
//
void updateCitation(CitationManager* Citations) {
	// Initialize data to update citation
	char* URL;
	char* author;
	char* title;
	int year = 0;

	printf("\nUpdating a Citation:\n");
	printf("--------------------\n");

	// Ask user to input URL
	URL = inputURL();

	// If user cancels, stop adding citation and return
	if (strlen(URL) == 0) {
		printf("Cancelled updating citation.\n");
		return;
	}

	// Look up Citation using hash table
	Citation* citationToUpdate = SearchHashTable(Citations, URL);

	if (citationToUpdate == NULL) {
		printf("Citation for that URL not found.\n");
		return;
	}

	// Update data
	author = inputAuthor();
	title = inputTitle();
	year = inputYear();

	// Assign data to citation
	if (strlen(author) > 0) {
		citationToUpdate->Author = _strdup(author);
	}
	if (strlen(title) > 0) {
		citationToUpdate->Title = _strdup(title);
	}
	if (year != 0) {
		citationToUpdate->Year = year;
	}

	printf("\nCitation updated:\n");
	printCitation(citationToUpdate);
}

//
// FUNCTION		:	removeCitation
// DESCRIPTION	:	Removes a citation from the program memory
// PARAMETERS	:	CitationManager* Citations	:	Hash table containing citations
//					Queue* CitationsToProcess	:	Queue to store citations that need to be processed
//					Stack* ProcessedCitations	:	Stack of citations that have been processed
// RETURNS		:	void
//
void removeCitation(CitationManager* Citations, Queue* CitationsToProcess, Stack* ProcessedCitations) {
	if (isQueueEmpty(CitationsToProcess) && isStackEmpty(ProcessedCitations)) {
		printf("No citations stored.\n");
		return;
	}

	printf("\nRemove a Citation:\n");
	printf("--------------------\n");

	// Ask user to input URL
	char* URL = inputURL();

	// If user cancels, stop adding citation and return
	if (strlen(URL) == 0) {
		printf("Cancelled removing citation.\n");
		return;
	}

	// Delete Citation KVP from hash table
	CitationKVP* citationToDelete = SearchKVPHashTable(Citations, URL);
	if (citationToDelete == NULL) {
		printf("Citation for that URL not found.\n");
		return;
	}

	// Remove Citation from queue
	Citation* toFree = NULL;
	if (!isQueueEmpty(CitationsToProcess)) {
		Citation* current = CitationsToProcess->Front;
		// Check front of queue
		if (strcmp(current->URL, URL) == 0) {
			toFree = Dequeue(CitationsToProcess);
		}
		else {
			while (current->Next != NULL) {
				// If next node matches URL
				if (strcmp(current->Next->URL, URL) == 0) {
					// Change pointer to the back of queue if removing from the back
					if (current->Next == CitationsToProcess->Back) {
						CitationsToProcess->Back = current;
					}
					// Move current node's pointer to next node's next
					current->Next = current->Next->Next;
					break;
				}
				current = current->Next;
			}
			toFree = citationToDelete->Citation;
		}
	}

	// Remove Citation from stack
	Citation* toFree2 = NULL;
	if (!isStackEmpty(ProcessedCitations)) {
		Citation* current2 = ProcessedCitations->Top;
		// Check top of stack
		if (strcmp(current2->URL, URL) == 0) {
			toFree2 = Pop(ProcessedCitations);
		}
		else {
			while (current2->Next != NULL) {
				// If next node matches URL
				if (strcmp(current2->Next->URL, URL) == 0) {
					// Move current node's pointer to next node's next
					current2->Next = current2->Next->Next;
					ProcessedCitations->StackIndex--; // Decrement stack index
					break;
				}
				current2 = current2->Next;
			}
			toFree2 = citationToDelete->Citation;
		}
	}

	// Free citation node
	DeleteHashTable(Citations, citationToDelete);
	if (toFree != NULL) {
		free(toFree);
	}
	if (toFree2 != NULL) {
		free(toFree2);
	}

	printf("Citation successfully deleted.\n");
}

//
// FUNCTION		:	processCitations
// DESCRIPTION	:	Moves citations from queue to sorted linked list and/or stack
// PARAMETERS	:	Citation* Head				: Pointer to head of sorted linked list
//					Queue* CitationsToProcess	: Queue to store citations that need to be processed
//					Stack* ProcessedCitations	: Stack of citations that have been processed
// RETURNS		:	void
//
void processCitations(Citation* Head, Queue* CitationsToProcess, Stack* ProcessedCitations) {
	// If there are no citations to process, return
	if (isQueueEmpty(CitationsToProcess)) {
		printf("No citations stored to process.\n");
		return;
	}

	printf("\nProcessing Citations:\n");
	printf("---------------------");

	// Ask user for process oepration
	bool running = true;
	int processChoice= 0;
	while (running) {
		printf("\n");
		processMenu();
		getMenuNum(&processChoice);
		switch (processChoice - 1) {
			// User selects to update all citations
			case UPDATE_ALL:
				updateAllCitations(CitationsToProcess);
				break;

			// User selects to sort all of the current citations and sort them
			case SORT_ALL:
				sortAllCitations(Head, CitationsToProcess);
				break;

			// User selects to webscrape of the current citations and sort them
			case WEB_SCRAPE_ALL:
				webscrapeAllCitations(CitationsToProcess);
				break;

			// User selects to process all citations - break loop
			case PROCESS_ALL:
				running = false;
				break;

			// User chooses to cancel - return to main menu
			case CANCEL:
				printf("Cancelled processing citations.\n");
				return;
			
			// User selects invalid choice
			default:
				printf("Error: Invalid option. Please select an option from the menu.\n");
				break;
		}
	}

	processAllCitations(CitationsToProcess, ProcessedCitations);
}

//
// FUNCTION		:	updateAllCitations
// DESCRIPTION	:	Iterates through entire queue and prompts user to update each citation
// PARAMETERS	:	Queue* CitationsToProcess	: Queue to store citations that need to be processed
// RETURNS		:	void
//
void updateAllCitations(Queue* CitationsToProcess) {
	// Check if queue of citations is empty
	if (isQueueEmpty(CitationsToProcess)) {
		return;
	}

	// Iterate through queue
	Citation* current = CitationsToProcess->Front;
	Citation* next;

	int year = 0;

	// Add/update citation data
	while (current != NULL) {
		printf("\nAdd Citation Data:\n");
		printf("--------------------\n");
		printf("URL: %s\n", current->URL);
		
		if (strlen(current->Author) == 0) {
			char* author = inputAuthor();
			if (strlen(author) > 0) {
				current->Author = _strdup(author);
				printf("Author: %s\n", current->Author);
			}
		}
		else {
			printf("Author: %s\n", current->Author);
		}

		if (strlen(current->Title) == 0) {
			char* title = inputTitle();
			current->Title = _strdup(title);
			printf("Title: %s\n", current->Title);
		}
		else {
			printf("Title: %s\n", current->Title);
		}

		if (year == 0) {
			year = inputYear();
			if (year > 0) {
				printf("Year: %d\n", current->Year);
				current->Year = year;
			}
		}
		else {
			printf("Year: %d\n", current->Year);
		}

		printf("Date Accessed: %s", current->DateAccessed);

		printf("\n\nAll citation data added.\n\n");

		// Move pointer
		current = current->Next;
	}

}

//
// FUNCTION		:	sortAllCitations
// DESCRIPTION	:	Calls SortQueue to sort the citations in a queue
// PARAMETERS	:	Citation* Head				: Pointer to head of sorted linked list
//					Queue* CitationsToProcess	: Queue to store citations that need to be processed
// RETURNS		:	void
//
void sortAllCitations(Citation* Head, Queue* CitationsToProcess) {
	// Check if queue of citations is empty
	if (isQueueEmpty(CitationsToProcess)) {
		return;
	}

	printf("\nSorted Citations\n");
	printf("--------------------\n");

	// Call SortQueue to sort the queue
	SortQueue(Head, CitationsToProcess);

	printAllCitations(CitationsToProcess->Front);
}

//
// FUNCTION		:	webscrapeAllCitations
// DESCRIPTION	:	Attempts web scraping to add citation data
// PARAMETERS	:	Queue* CitationsToProcess	: Queue to store citations that need to be processed
// RETURNS		:	void
//
void webscrapeAllCitations(Queue* CitationsToProcess) {
	// Check if queue of citations is empty
	if (isQueueEmpty(CitationsToProcess)) {
		return;
	}

	printf("\nScraping the web for data...\n\n");
	Citation* current = CitationsToProcess->Front;
	while (current != NULL) {
		// Call WebScraping
		WebScraping(current);

		// Print each citation
		printCitation(current);
		printf("\n");
		current = current->Next;
	}

	printf("\nWeb scraping complete.\n");
}

//
// FUNCTION		:	processAllCitations
// DESCRIPTION	:	Moves citations from queue to stack
// PARAMETERS	:	Queue* CitationsToProcess	: Queue to store citations that need to be processed
//					Stack* ProcessedCitations	: Stack of citations that have been processed
// RETURNS		:	void
//
void processAllCitations(Queue* CitationsToProcess, Stack* ProcessedCitations) {
	// Check if queue of citations is empty
	if (isQueueEmpty(CitationsToProcess)) {
		return;
	}

	// Move all items in queue to stack
	Citation* current = CitationsToProcess->Front;
	Citation* next;

	printf("\nProcessed Citations:\n\n");
	while (current != NULL) {
		// Store next pointer in queue
		next = current->Next;

		// Push current citation to the stack by dequeuing
		Push(ProcessedCitations, Dequeue(CitationsToProcess));
		current = next;
	}

	// Print all processed citations
	printAllCitations(ProcessedCitations->Top);

}

//
// FUNCTION		:	exportCitations
// DESCRIPTION	:	Exports all processed citations in stack to a file
// PARAMETERS	:	FILE* ExportFile			: File pointer to save citations
//					CitationManager* Citations : Hash table containing citations
//					Stack* ProcessedCitations	: Stack of citations that have been processed
// RETURNS		:	void
//
void exportCitations(FILE* ExportFile, CitationManager* Citations, Stack* ProcessedCitations) {
	printf("\nExporting Citations\n");
	printf("--------------------\n");
	
	// Check if stack is empty
	if (isStackEmpty(ProcessedCitations)) {
		printf("No citations processed to export.\n");
		return;
	}

	SaveFile(ExportFile, Citations, ProcessedCitations);
}

