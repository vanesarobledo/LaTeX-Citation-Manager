#include <json-c/json.h>
#include "Citations.h"

void parseJSON(char* json, Citation* citation) {
	// Create root node of JSON object & exit if failure to create json_object
	json_object* root = json_tokener_parse(json);
	if (root == NULL) {
		printf("Error reading JSON data.\n");
		return;
	}

	// Initialize data
	int arraySize = 0; // Size of array to iterate through in @graph
	json_object* temp; // Store JSON object at each index in @graph

	// Extract year
	char* dateModified; // Store dateModified string
	char* extractedYear;
	std::regex rgx("^(\\d{4})(?:.+)"); // regex to match year
	std::cmatch match;
	int year = 0;

	// Iterate through each key-value pair in the JSON data & drill down to relevant data
	json_object_object_foreach(root, key1, val1) {
		// Extract title
		if (strcmp(key1, "headline") == 0) {
			citation->Title = _strdup(json_object_get_string(val1));
		}

		// Extract author
		if (strcmp(key1, "author") == 0) {
			json_object* author = json_object_object_get(root, "author");
			// If author is an array
			if (json_object_is_type(author, json_type_array) == 1) {
				arraySize = (int)json_object_array_length(author);
				for (int i = 0; i < arraySize; i++) {
					temp = json_object_array_get_idx(author, i);
					json_object_object_foreach(temp, key2, val2) {
						if (strcmp(key2, "name") == 0) {
							citation->Author = _strdup(json_object_get_string(val2));
							break;
						}
					}
				}
			}
			// If author is string
			else {
				json_object_object_foreach(author, key2, val2) {
					if (strcmp(key2, "name") == 0) {
						citation->Author = _strdup(json_object_get_string(val2));
						break;
					}
				}
			}
		}

		// Extract year
		if (strcmp(key1, "datePublished") == 0) {
			dateModified = _strdup(json_object_get_string(val1));
			if (std::regex_search(dateModified, match, rgx)) {
				extractedYear = _strdup(match[1].first);
				sscanf_s(extractedYear, "%d", &year);
				citation->Year = year;
			}
		}
		if (strcmp(key1, "dateModified") == 0) { // Overwrite with modified year if available
			dateModified = _strdup(json_object_get_string(val1));
			if (std::regex_search(dateModified, match, rgx)) {
				extractedYear = _strdup(match[1].first);
				sscanf_s(extractedYear, "%d", &year);
				citation->Year = year;
			}
		}


		// Drill down to @graph node -> turn this into recursion for simplicity's sake
		if (strcmp(key1, "@graph") == 0) {
			json_object* graph = json_object_object_get(root, "@graph");
			
			// @graph contains an array of JSON objects, so iterate through the array & check each key-value pair
			arraySize = (int)json_object_array_length(graph);
			for (int i = 0; i < arraySize; i++) {
				temp = json_object_array_get_idx(graph, i);
				json_object_object_foreach(temp, key2, val2) {
					// Drill down to @type: WebSite node
					// Extract name of website (if author is not found)
					if (strcmp(key2, "@type") == 0 && strcmp(json_object_get_string(val2), "WebSite") == 0) {
						json_object* title_website = json_object_object_get(temp, "name");
						citation->Author = _strdup(json_object_get_string(title_website));
					}

					// Drill down to @type : WebPage
					// Extract title
					if (strcmp(key2, "@type") == 0 && strcmp(json_object_get_string(val2), "WebPage") == 0) {
						json_object* title = json_object_object_get(temp, "name");
						citation->Title = _strdup(json_object_get_string(title));
					}

					// Extract author
					if (strcmp(key2, "author") == 0) {
						// Drill down to author node
						json_object* author = json_object_object_get(temp, "author");
						json_object_object_foreach(author, key3, val3) {
							// Extract author
							if (strcmp(key3, "name") == 0) {
								citation->Author = _strdup(json_object_get_string(val3));
							}
						}
					}
					// Extract year
					if (strcmp(key2, "datePublished") == 0) {
						dateModified = _strdup(json_object_get_string(val2));
						if (std::regex_search(dateModified, match, rgx)) {
							extractedYear = _strdup(match[1].first);
							sscanf_s(extractedYear, "%d", &year);
							citation->Year = year;
						}
					}
					if (strcmp(key2, "dateModified") == 0) { // Overwrite with modified year if available
						dateModified = _strdup(json_object_get_string(val2));
						if (std::regex_search(dateModified, match, rgx)) {
							extractedYear = _strdup(match[1].first);
							sscanf_s(extractedYear, "%d", &year);
							citation->Year = year;
						}
					}
				}
			}
		}
	}

	json_object_put(root);
}