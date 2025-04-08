/*
* FILE          : Stack.cpp
* PROJECT       : SENG1050 Final Project: LaTeX Citation Manager (working title)
* PROGRAMMER    : Vanesa Robledo
* FIRST VERSION : 2025-03-21
* DESCRIPTION   : This file contains the stack functions needed for the LaTeX Citation Manager (working title)
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "Citations.h"


//
// FUNCTION     : InitializeStack
// DESCRIPTION  : Dynamically allocates memory for stack to store ProcessedCitations
// PARAMETERS   : none
// RETURNS      : Stack*
//
struct Stack* InitializeStack() {
	Stack* stack = (Stack*)malloc(sizeof(Stack*));

	// Check if memory was allocated; if not, exit program
	if (stack == NULL) {
		printf("Insufficient memory to create stack. Exiting program...\n");
		exit(EXIT_FAILURE);
	}

	// Initialize head node value of stack
	stack->Top = NULL;
	stack->StackIndex = -1;

	return stack;
}

//
// FUNCTION     : isStackEmpty
// DESCRIPTION  : Returns boolean if stack is empty by checking the Top pointer
// PARAMETERS   : Stack* ProcessedCitations : Stack of citations that have been processed
// RETURNS      : bool
//
bool isStackEmpty(Stack* ProcessedCitations) {
	return ProcessedCitations->Top == NULL;
}

//
// FUNCTION     : Push
// DESCRIPTION  : Adds a Citation node to the Top of the stack
// PARAMETERS   : Stack* ProcessedCitations		: Stack of citations that have been processed
//				  Citation* completedCitation	: Citation that has been processed
// RETURNS      : none
//
void Push(Stack* ProcessedCitations, Citation* completedCitation) {
	if (ProcessedCitations == NULL) {
		printf("Error: Stack has not been initialized.\n");
		return;
	}

	if (completedCitation == NULL) {
		printf("Error: Invalid citation. Cannot add to queue.\n");
		return;
	}

	completedCitation->Next = ProcessedCitations->Top;
	ProcessedCitations->Top = completedCitation;
	ProcessedCitations->StackIndex++;
}

//
// FUNCTION     : Pop
// DESCRIPTION  : Removes the citation node from the top of the stack
// PARAMETERS   : Stack* ProcessedCitations	: Stack of citations that have been processed
// RETURNS      : Citation*
//
Citation* Pop(Stack* ProcessedCitations) {
	// Initialize Citation pointer to pop from stack
	Citation* toPop = NULL;

	if (isStackEmpty(ProcessedCitations)) {
		return toPop;
	}

	toPop = ProcessedCitations->Top;
	ProcessedCitations->Top = ProcessedCitations->Top->Next;
	ProcessedCitations->StackIndex--;

	toPop->Next = NULL;

	return toPop;
}

//
// FUNCTION     : FreeStack
// DESCRIPTION  : Frees memory of citations in the stack
// PARAMETERS   : Stack* ProcessedCitations	: Stack of citations that have been processed
// RETURNS      : void
//
void FreeStack(Stack* ProcessedCitations) {
	// Check if stack is empty
	if (isStackEmpty(ProcessedCitations)) {
		return;
	}
	// If stack is not empty, free memory
	Citation* toFree = NULL;
	while (ProcessedCitations->Top != NULL) {
		toFree = ProcessedCitations->Top;
		ProcessedCitations->Top = ProcessedCitations->Top->Next;
		ProcessedCitations->StackIndex--;
		free(toFree);
	}

	printf("Stack was completely freed.\n");
}