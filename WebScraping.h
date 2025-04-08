#pragma once
#include <curl/curl.h>
#include "libxml/HTMLparser.h"
#include "libxml/xpath.h"

// Web Scraping
struct CURLResponse {
	char* html;
	size_t size;
};

typedef struct WebsiteInfo {
	char* title;
	char* author;
	char* year;
};

void WebScraping(Citation* citation);
struct CURLResponse GetRequest(CURL* curl_handle, const char* url);
void parseJSON(char* json, Citation* citation);