/*
* FILE          : Queue.cpp
* PROJECT       : SENG1050 Final Project: LaTeX Citation Manager (working title)
* PROGRAMMER    : Vanesa Robledo
* FIRST VERSION : 2025-03-21
* DESCRIPTION   : This file contains the queue functions needed for the LaTeX Citation Manager (working title)
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <regex>

#include "Citations.h"

//
// FUNCTION     : InitializeQueue
// DESCRIPTION  : Dynamically allocates queue & initializes front & back nodes
// PARAMETERS   : none 
// RETURNS      : Queue*
//
struct Queue* InitializeQueue() {
	Queue* CitationsToProcess = (Queue*)malloc(sizeof(Queue));

	if (CitationsToProcess == NULL) {
		printf("Insufficient memory to create queue. Exiting program...\n");
		exit(EXIT_FAILURE);
	}

	// Initialize front & back nodes of queue
	CitationsToProcess->Front = NULL;
	CitationsToProcess->Back = NULL;

	return CitationsToProcess;
}

//
// FUNCTION     : isQueueEmpty
// DESCRIPTION  : Returns boolean if queue is empty by checking the Front pointer
// PARAMETERS   : Queue* CitationsToProcess	: Queue to store citations that need to be processed
// RETURNS      : bool
//
bool isQueueEmpty(Queue* CitationsToProcess) {
	return CitationsToProcess->Front == NULL;
}

//
// FUNCTION     : Enqueue
// DESCRIPTION  : Adds a Citation node to the back of the queue
// PARAMETERS   : Queue* CitationsToProcess	: Queue to store citations that need to be processed
//				  Citation& newCitation		: Citation node to be enqueued
// RETURNS      : none
//
void Enqueue(Queue* CitationsToProcess, Citation* newCitation) {
	if (CitationsToProcess == NULL) {
		printf("Error: Queue has not been initialized.\n");
		return;
	}

	if (newCitation == NULL) {
		printf("Error: Invalid citation. Cannot add to queue.\n");
		return;
	}

	// If queue is empty, add newCitation as front and back
	if (isQueueEmpty(CitationsToProcess)) {
		CitationsToProcess->Front = newCitation;
		CitationsToProcess->Back = newCitation;
	}
	// If queue is not empty, add newCitation as the back
	else {
		CitationsToProcess->Back->Next = newCitation;
		CitationsToProcess->Back = newCitation;
	}
}

//
// FUNCTION     : Dequeue
// DESCRIPTION  : Removes a node from the queue (FIFO) and citation that was removed
// PARAMETERS   : Queue* CitationsToProcess : Queue to store citations that need to be processed
// RETURNS      : Citation*
//
Citation* Dequeue(Queue* CitationsToProcess) {
	// Initialize NULL citation to return if queue is empty
	Citation* nullCitation = NULL;

	// Check if queue is empty
	if (isQueueEmpty(CitationsToProcess)) {
		printf("Error: There are no citations in the queue.\n");
		return nullCitation;
	}

	// Store citation to dequeue
	Citation* nodeToDequeue = CitationsToProcess->Front;

	// Check if there is only one citation in the queue
	if (CitationsToProcess->Front == CitationsToProcess->Back) {
		CitationsToProcess->Front = NULL;
		CitationsToProcess->Back = NULL;
	}
	// More than one citation in the queue
	else {
		// Change queue's front to next node
		CitationsToProcess->Front = CitationsToProcess->Front->Next;
	}

	return nodeToDequeue;
}

//
// FUNCTION     : FreeQueue
// DESCRIPTION  : Releases memory allocated in queue
// PARAMETERS   : Queue* CitationsToProcess : Queue to store citations that need to be processed
// RETURNS      : None
//
void FreeQueue(Queue* CitationsToProcess) {
	// Check if queue is empty
	if (isQueueEmpty(CitationsToProcess)) {
		return;
	}

	// Free memory allocated in queue
	Citation* toFree;
	while (CitationsToProcess->Front != NULL) {
		toFree = Dequeue(CitationsToProcess); // Obtain node to be freed
		CitationsToProcess->Front = toFree->Next; // Change front to the next node
		free(toFree); // Free node
	}

	// Change back node pointer to NULL
	CitationsToProcess->Back = NULL;

	printf("Queue was completely freed.\n");
}