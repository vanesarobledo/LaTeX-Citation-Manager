/*
* FILE          : Main.cpp
* PROJECT       : SENG1050 Final Project: LaTeX Citation Manager
* PROGRAMMER    : Vanesa Robledo
* FIRST VERSION : 2025-03-21
* DESCRIPTION   : LaTeX Citation Manager is a console-based menu-driven application that allows users to import or add
*				  website citations by URL. The application will prompt the user to enter information necessary to
*				  complete each citation and allows the user to export their entire bibliography for a .tex file for
*				  use in BibLaTeX.
*				  This file is the main entry point into the program and contains the main user menu.
*/
#include <stdio.h>

#include "Citations.h"
#include "WebScraping.h"
#include <json-c/json.h>

// User Menu Functions
enum menu {
	IMPORT,
	ADD,
	UPDATE,
	REMOVE,
	PROCESS,
	EXPORT,
	EXIT
} OPERATIONS;

int main(int argc, char* argv[])
{
	// Initialize Data Structures
	CitationManager* Citations = InitializeHashTable(); // Hash Table
	Queue* CitationsToProcess = InitializeQueue(); // Queue
	Stack* ProcessedCitations = InitializeStack(); // Stack
	Citation* Head = NULL; // Sorted linked list

	// Initialize file pointer
	FILE* ImportFile = NULL;
	FILE* ExportFile = NULL;

	// Command Line Arguments
	// Non-web scraping
	if (argc == 3) {
		if (strcmp(argv[1], "-i") == 0) {
			importCitationsFile(ImportFile, CitationsToProcess, argv[2]);
			exportCitationsFile(ExportFile, CitationsToProcess, "references.bib");
			printf("URLs from %s imported to references.bib\n", argv[2]);
			exit(EXIT_SUCCESS);
		}

		else if (strcmp(argv[1], "-w") == 0) {
			importCitationsFile(ImportFile, CitationsToProcess, argv[2]);
			webscrapeAllCitations(CitationsToProcess);
			exportCitationsFile(ExportFile, CitationsToProcess, "references.bib");
			printf("URLs from %s imported to references.bib\n", argv[2]);
			exit(EXIT_SUCCESS);
		}

		// If invalid arguments entered
		else {
			printf("Error: Parameters not recognized. Indicate the file to import with -i flag or -w flag for web scraping.\n");
		}
	}

	// User input data
	int menuNum = 0; // Store menu choice
	bool running = true; // Flag to loop menu

	// Introduction
	header();

	// Main menu
	while (running) {
		menu();
		getMenuNum(&menuNum);
		switch (menuNum)
		{
		case IMPORT: // Import citations from a text-based file
			importCitations(ImportFile, Citations, CitationsToProcess);
			break;

		case ADD: // Manually add a citation
			addCitation(Citations, CitationsToProcess);
			break;

		case UPDATE: // Update a citation by URL
			updateCitation(Citations);
			break;

		case REMOVE: // Delete a citation by URL
			removeCitation(Citations, CitationsToProcess, ProcessedCitations);
			break;

		case PROCESS: // Process citations
			processCitations(Head, CitationsToProcess, ProcessedCitations);
			break;

		case EXPORT: // Export processed citations
			exportCitations(ExportFile, Citations, ProcessedCitations);
			break;

		case EXIT: // Exit the program & free all allocated memory
			running = false;
			exitProgram(Citations, CitationsToProcess, ProcessedCitations, Head);
			break;

		default: // Invalid option
			printf("Error: Invalid option. Please select an option from the menu.\n");
			break;
		}
		pressEntertoContinue();
		header();
	}
	return 0;
}