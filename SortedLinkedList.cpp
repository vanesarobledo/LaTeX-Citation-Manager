/*
* FILE          : SortedLinkedList.cpp
* PROJECT       : SENG1050 Final Project: LaTeX Citation Manager (working title)
* PROGRAMMER    : Vanesa Robledo
* FIRST VERSION : 2025-03-21
* DESCRIPTION   : This file contains functions needed for inserting into a sorted linked list the LaTeX Citation Manager (working title)
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <regex>

#include "Citations.h"

//
// FUNCTION     :	InsertSorted
// DESCRIPTION  :	Inserts a citation node in a linked list reverse-alphabetically by author, title, or URL
// PARAMETERS   :	Citation** Head		: Double pointer to head of sorted linked list
//					Citation* newCitation	: Citation node to insert into list
// RETURNS      :	void
//
void InsertSorted(Citation** Head, Citation* newCitation) {
	// Error-checking - if newCitation is valid
	if (newCitation == NULL) {
		printf("Error: Invalid pointer to citation. Did not insert into sorted linked list.\n");
		return;
	}

	// If list is empty - insert directly as head
	if (*Head == NULL) {
		*Head = newCitation;
		return;
	}

	// Store strings to compare
	char* currentCitation;
	char* nextCitation;

	// Store whether or not new string occurs alphabetically after current string
	bool diff = false;

	// Store string for comparison for current citation
	if (strlen(newCitation->Author) > 0) {
		currentCitation = _strdup(newCitation->Author);
	}
	else if (strlen(newCitation->Title) > 0) {
		currentCitation = _strdup(newCitation->Title);
	}
	else {
		currentCitation = trimURL(_strdup(newCitation->URL));
	}

	// Check head
	if (strlen((*Head)->Author) > 0) {
		nextCitation = _strdup((*Head)->Author);
	}
	else if (strlen((*Head)->Title) > 0) {
		nextCitation = _strdup((*Head)->Title);
	}
	else {
		nextCitation = trimURL(_strdup((*Head)->URL));
	}

	diff = compareStrings(currentCitation, nextCitation);

	// Insert as new head if it comes alphabetically after head
	if (diff) {
		newCitation->Next = *Head;
		*Head = newCitation;
		return;
	}

	// Iterate through list
	Citation* current = *Head;

	while (current->Next != NULL) {
		if (strlen(current->Next->Author) > 0) {
			nextCitation = _strdup(current->Next->Author);
		}
		else if (strlen(current->Next->Title) > 0) {
			nextCitation = _strdup(current->Next->Title);
		}
		else {
			nextCitation = trimURL(_strdup(current->Next->URL));
		}

		// Compare both strings
		diff = compareStrings(currentCitation, nextCitation);

		// If newCitation is alphabetically after next citation, break
		if (diff) {
			break;
		}
		// Otherwise, keep iterating
		else {
			diff = false; // Reset flag
			current = current->Next;
		}
	}

	// Insert into sorted linked list
	newCitation->Next = current->Next;
	current->Next = newCitation;
}

//
// FUNCTION     :	SortQueue
// DESCRIPTION  :	Dequeues all citations, inserts them into a sorted linked list, and then adds them back to queue
// PARAMETERS   :	Citation* Head				: Pointer to head of sorted linked list
//					Queue* CitationsToProcess	: Queue to store citations that need to be processed
// RETURNS      :	void
//
void SortQueue(Citation* Head, Queue* CitationsToProcess) {
	// Check if queue is empty
	if (isQueueEmpty(CitationsToProcess)) {
		printf("Error: No sorted citations to process.\n");
		return;
	}

	// Move all citations from queue to sorted linked list
	Citation* current = CitationsToProcess->Front;
	Citation* sortedNode;
	Citation* next;

	while (current != NULL) {
		// Store next pointer in queue
		next = current->Next;

		// Insert dequeued node into sorted linked list
		sortedNode = Dequeue(CitationsToProcess);
		sortedNode->Next = NULL; // Reset Next pointer
		InsertSorted(&Head, sortedNode);

		// Move pointer to next in queue
		current = next;
	}

	// Add all citations back to queue
	Citation* current2 = Head;
	Citation* next2;

	while (current2 != NULL) {
		next2 = current2->Next;
		Enqueue(CitationsToProcess, current2);
		current2 = next2;
	}
	
	// Reset head pointer to sorted linked list
	Head = NULL;
}

//
// FUNCTION     :	FreeSortedLinkedList
// DESCRIPTION  :	Frees all memory in linked list
// PARAMETERS   :	Citation** Head	: Pointer to head of sorted linked list
// RETURNS      :	void
//
void FreeSortedLinkedList(Citation* Head) {
	// If list is already empty, return
	if (Head == NULL) {
		return;
	}

	// Iterate through linked list and free memory
	Citation* toFree = NULL;
	Citation* current = Head;

	while (current->Next != NULL) {
		toFree = current;
		current = current->Next;
		free(toFree);
	}

	free(current);

	printf("Linked list was completely freed.\n");
}