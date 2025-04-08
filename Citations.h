#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <regex>

// Define constants
#define	LINE_SIZE	256
#define HASH_TABLE_SIZE	100
#define TIMESTAMP	11
#define BIB_SIZE	865

// Define Citation Node
typedef struct Citation {
	char* Author;
	char* Title;
	int Year;
	char* URL;
	char DateAccessed[TIMESTAMP];
	struct Citation* Next;
} Citation;

// Define Key-Value Pair to store citations
typedef struct CitationKVP {
	char* URL;
	Citation* Citation;
	struct CitationKVP* NextKeyValuePair;

} CitationKVP;

// Define Hash Table
typedef struct CitationManager {
	CitationKVP* Table[HASH_TABLE_SIZE];
} CitationManager;

// Define Queue
typedef struct Queue {
	Citation* Front;
	Citation* Back;
} Queue;

// Define Stack
typedef struct Stack {
	Citation* Top;
	int StackIndex;
} Stack;

// Process operations
enum processing {
	UPDATE_ALL,
	SORT_ALL,
	WEB_SCRAPE_ALL,
	PROCESS_ALL,
	CANCEL
};

// Function Prototypes

// File Functions
FILE* LoadFile(void);
void StoreFileData(FILE* file, CitationManager* Citations, Queue* CitationsToProcess);
void InsertData(CitationManager* Citations, Queue* CitationsToProcess, const char* url);
void SaveFile(FILE* file, CitationManager* Citations, Stack* ProcessedCitations);

// Citation Struct
Citation* InitializeCitation(const char* url);
void printCitation(Citation* citation);
void printAllCitations(Citation* citation);

// Hash Table Functions
unsigned int Hash(const char* str);
CitationManager* InitializeHashTable(void);
CitationKVP* InitializeKeyValuePair(Citation* newCitation);
bool InsertHashTable(CitationManager* Citations, Citation* newCitation);
Citation* SearchHashTable(CitationManager* Citations, const char* url);
CitationKVP* SearchKVPHashTable(CitationManager* Citations, const char* url);
bool DeleteHashTable(CitationManager* Citations, CitationKVP* toDelete);
void FreeHashTable(CitationManager* Citations); 

// Queue Functions
struct Queue* InitializeQueue();
bool isQueueEmpty(Queue* CitationsToProcess);
void Enqueue(Queue* CitationsToProcess, Citation* newCitation);
Citation* Dequeue(Queue* CitationsToProcess);
void FreeQueue(Queue* CitationsToProcess);

// Sorted Linked List
void InsertSorted(Citation** Head, Citation* newCitation);
void SortQueue(Citation* Head, Queue* CitationsToProcess);
void FreeSortedLinkedList(Citation* Head);

// Stack Function Prototypes
Stack* InitializeStack();
bool isStackEmpty(Stack* ProcessedCitations);
void Push(Stack* stack, Citation* completedCitation);
Citation* Pop(Stack* stack);
void FreeStack(Stack* stack);

// Main Program Functions
void importCitations(FILE* ImportFile, CitationManager* Citations, Queue* CitationsToProcess);
void addCitation(CitationManager* Citations, Queue* CitationsToProcess);
void updateCitation(CitationManager* Citations);
void removeCitation(CitationManager* Citations, Queue* CitationsToProcess, Stack* ProcessedCitations);

void processCitations(Citation* Head, Queue* CitationsToProcess, Stack* ProcessedCitations);
void processAllCitations(Queue* CitationsToProcess, Stack* ProcessedCitations);
void updateAllCitations(Queue* CitationsToProcess);
void sortAllCitations(Citation* Head, Queue* CitationsToProcess);
void webscrapeAllCitations(Queue* CitationsToProcess);
void exportCitations(FILE* ExportFile, CitationManager* Citations, Stack* ProcessedCitations);

void freeMemory(CitationManager* Citations, Queue* CitationsToProcess, Stack* ProcessedCitations, Citation* Head);
void exitProgram(CitationManager* Citations, Queue* CitationsToProcess, Stack* ProcessedCitations, Citation* Head);

// Command Line Functions
void importCitationsFile(FILE* ImportFile, Queue* CitationsToProcess, const char* filename);
void exportCitationsFile(FILE* ExportFile, Queue* CitationsToProcess, const char* filename);

// Menu Functions
void header(void);
void menu(void);
void processMenu(void);
void getMenuNum(int* menuNum);
void pressEntertoContinue(void);

// Input Functions
char* inputURL(void);
char* inputTitle(void);
char* inputAuthor(void);
int inputYear(void);

// String Validation
bool compareStrings(char* str1, char* str2);
void nullTerminate(char* str);
void clearNewLineChar(char* str);
char* trimURL(char* str);
char* trimWhitespace(char* str);