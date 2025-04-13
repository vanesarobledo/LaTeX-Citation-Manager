/*
* FILE          : HashTable.cpp
* PROJECT       : SENG1050 Final Project: LaTeX Citation Manager
* PROGRAMMER    : Vanesa Robledo
* FIRST VERSION : 2025-03-21
* DESCRIPTION   : This file contains the hash table functions
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "Citations.h"

//
// FUNCTION     : Hash
// DESCRIPTION  : Hashing function for use of storing citations in the hash table (CitationManager)
//                This hash function is a modification of a hash function that was developed by Daniel J. Bernstein
//                in 1991 and is colloquially known as the "djb2 Hash"
// PARAMETERS   : const char* str   :   The string to hash (URL)
// RETURNS      : int               :   The hashed string (hashed URL) to store in hash table
//
/*
* TITLE         : djb2 Hash
* AUTHOR        : Daniel J. Bernstein
* DATE          : 1991
* VERSION       : 1.0
* AVAILABIILTY  : https://theartincode.stanis.me/008-djb2/
*/
unsigned int Hash(const char* str)
{
    unsigned int hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c;
    return hash % HASH_TABLE_SIZE;
}

//
// FUNCTION     : InitializeHashTable
// DESCRIPTION  : Dynamically allocates memory for hash table and initializes all values as NULL
// PARAMETERS   : none
// RETURNS      : CitationManager*
//
CitationManager* InitializeHashTable(void) {
    CitationManager* hashTable = (CitationManager*)malloc(sizeof(CitationManager));

    if (hashTable == NULL) {
        printf("Insufficient memory to create hash table. Exiting program...\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        hashTable->Table[i] = NULL;
    }

    return hashTable;
}

//
// FUNCTION     : InitializeKeyValuePair
// DESCRIPTION  : Dynamically allocates memory for KVP & calls new struct
// PARAMETERS   : Citation* newCitation : Citation node to be stored as value
// RETURNS      : CitationKVP*
//
CitationKVP* InitializeKeyValuePair(Citation* newCitation) {
    CitationKVP* kvp = (CitationKVP*)malloc(sizeof(CitationKVP));

    if (kvp == NULL) {
        printf("Insufficient memory to create key-value pair. Exiting program...\n");
        exit(EXIT_FAILURE);
    }

    // Store values in key-value pair
    kvp->URL = _strdup(newCitation->URL); // Key: citation URL
    kvp->Citation = newCitation; // Value
    kvp->NextKeyValuePair = NULL;

    return kvp;
}

//
// FUNCTION     : InsertHashTable
// DESCRIPTION  : Hashes a given URL and creates a KVP pair and store in the hash table - returns true if successful
// PARAMETERS   : CitationManager* Citations : Hash table containing citations
//                Citation* newCitation      : Citation node to be stored in hash table
// RETURNS      : bool
//
bool InsertHashTable(CitationManager* Citations, Citation* newCitation) {
    // Error-Checking - make sure null pointers are not dereferenced
    if (Citations == NULL) {
        printf("Error: Uninitialized memory for hash table.\n");
        return false;
    }
    if (newCitation == NULL) {
        printf("Error: Uninitialized memory for citation.\n");
        return false;
    }

    // Check for non-empty URL data
    if (strlen(newCitation->URL) == 0) {
        printf("Error: URL in citation is empty.\n");
        return false;
    }

    // Hash the key (website URL)
    int hash = Hash(newCitation->URL);

    // Create key-value pair
    CitationKVP* newKVP = InitializeKeyValuePair(newCitation);

    // Insert directly if bucket is empty
    if (Citations->Table[hash] == NULL) {
        Citations->Table[hash] = newKVP;
        return true;
    }

    // Do not insert if URL already exists in hash table
    if (strcmp(Citations->Table[hash]->URL, newCitation->URL) == 0) {
        printf("Error: URL already stored in data.\n");
        free(newKVP);
        return false;
    }

    // Insert with separate chaining if there is collision
    CitationKVP* current = (Citations)->Table[hash];
    while (current->NextKeyValuePair != NULL) {
        current = current->NextKeyValuePair;
    }

    // Do not insert if URL already exists in hash table
    if (strcmp(current->URL, newCitation->URL) == 0) {
        printf("Error: URL already stored in data.\n");
        free(newKVP);
        return false;
    }

    // Insert into hash table
    current->NextKeyValuePair = newKVP;
    return true;
}

//
// FUNCTION     : SearchHashTable
// DESCRIPTION  : Searches hash table for URL and returns the citation struct
// PARAMETERS   : CitationManager* Citations : Hash table containing citations
//                const char* url            : The website URL to search for citation
// RETURNS      : Citation*
//
Citation* SearchHashTable(CitationManager* Citations, const char* url) {
    int hash = Hash(url);
    Citation* nullCitation = NULL; // Return null if not found

    // Search if bucket is not empty
    if (Citations->Table[hash] != NULL) {
        CitationKVP* current = Citations->Table[hash];

        // If there is only one citation stored in the bucket, check if the URL matches, and return
        if (current->NextKeyValuePair == NULL && strncmp(current->Citation->URL, url, LINE_SIZE) == 0) {
            return current->Citation;
        }

        // If there is more than one citation stored in the bucket, iterate until there is a match
        while (current != NULL) {
            if (strncmp(current->Citation->URL, url, LINE_SIZE) == 0) {
                return current->Citation;
            }
            current = current->NextKeyValuePair;
        }
    }

    // If bucket is empty, return NULL
    return nullCitation;
}

//
// FUNCTION     : SearchKVPHashTable
// DESCRIPTION  : Searches hash table wih URL and returns the KVP
// PARAMETERS   : CitationManager* Citations : Hash table containing citations
//                const char* url            : The website URL to search for citation
// RETURNS      : CitationKVP*
//
CitationKVP* SearchKVPHashTable(CitationManager* Citations, const char* url) {
    int hash = Hash(url);
    CitationKVP* nullKVP = NULL; // Return null if not found

    // Search if bucket is not empty
    if (Citations->Table[hash] != NULL) {
        CitationKVP* current = Citations->Table[hash];

        // If there is only one citation stored in the bucket, check if the URL matches, and return
        if (current->NextKeyValuePair == NULL && strncmp(current->Citation->URL, url, LINE_SIZE) == 0) {
            return current;
        }

        // If there is more than one citation stored in the bucket, iterate until there is a match
        while (current != NULL) {
            if (strncmp(current->Citation->URL, url, LINE_SIZE) == 0) {
                return current;
            }
            current = current->NextKeyValuePair;
        }
    }

    // If bucket is empty, return NULL
    return nullKVP;
}

//
// FUNCTION     : DeleteHashTable
// DESCRIPTION  : Deletes a key-value pair from the table. If there is more than one key-value pair stored in a
//                bucket, 
// PARAMETERS   : CitationManager* Citations    : Hash table containing citations
//                CitationKVP* toDelete         : Citation node to be deleted in hash table
// RETURNS      : bool
//
bool DeleteHashTable(CitationManager* Citations, CitationKVP* toDelete) {
    // Error-Checking - make sure null pointers are not dereferenced
    if (Citations == NULL) {
        printf("Error: Uninitialized memory for hash table.\n");
        return false;
    }
    if (toDelete == NULL) {
        printf("Error: Citation key-value pair not found.\n");
        return false;
    }

    // Hash the key (website URL)
    int hash = Hash(toDelete->URL);

    // Store KVP to free
    CitationKVP* toFree = NULL;
    CitationKVP* current = Citations->Table[hash];

    // Check if there are other KVPs are stored in the same bucket
    if (Citations->Table[hash]->NextKeyValuePair != NULL) {
        // Check first item
        if (strcmp(current->Citation->URL, toDelete->URL) == 0) {
            toFree = current;
            Citations->Table[hash] = current->NextKeyValuePair;
        }
        // Iterate through linked list
        else
        {
            while (current->NextKeyValuePair != NULL) {
                // If next node matches, move current node to next node's next
                if (strcmp(current->NextKeyValuePair->Citation->URL, toDelete->URL) == 0) {
                    toFree = current->NextKeyValuePair;
                    current->NextKeyValuePair = current->NextKeyValuePair->NextKeyValuePair;
                    break;
                }
                current = current->NextKeyValuePair;
            }
        }
    }
    else {
        toFree = current;
        Citations->Table[hash] = NULL;
    }

    // Free memory
    if (toFree != NULL) {
        free(toFree);
        return true;
    }

    return false;
}

//
// FUNCTION     : FreeHashTable
// DESCRIPTION  : Frees dynamically allocated memory in hash table
// PARAMETERS   : CitationManager* Citations : Hash table containing citations
// RETURNS      : void
//
void FreeHashTable(CitationManager* Citations) {
    // Iterate through every bucket and free memory of KVP
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        if (Citations->Table[i] != NULL) {
            // If bucket has only one item, free memory
            if (Citations->Table[i]->NextKeyValuePair == NULL) {
                free(Citations->Table[i]);
            }
            // Otherwise iterate through linked list to free all memory
            else {
                CitationKVP* current = Citations->Table[i];
                CitationKVP* next = NULL;
                while (current != NULL) {
                    next = current->NextKeyValuePair;
                    free(current);
                    current = next;
                }
            }
        }
    }
    printf("Hash table was completely freed.\n");
}