/*
* FILE          : WebScraping.cpp
* PROJECT       : SENG1050 Final Project: LaTeX Citation Manager (working title)
* PROGRAMMER    : Vanesa Robledo
* FIRST VERSION : 2025-04-07
* DESCRIPTION   : This file contains web scraping functions
*                 Code taken & modified from: https://www.zenrows.com/blog/web-scraping-c and
                  https://scrape.do/blog/web-scraping-with-c/
*/

#include <stdio.h>
#include <stdlib.h>

#include "Citations.h"
#include "WebScraping.h"

/*
* TITLE         : Web Scraping with C
* AUTHOR        : Unknown
* DATE          : June 1, 2024
* VERSION       : 1.0
* AVAILABIILTY  : https://www.zenrows.com/blog/web-scraping-c
*/
void WebScraping(Citation* citation) {
    // initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);

    // initialize a CURL instance
    CURL* curl_handle = curl_easy_init();

    // retrieve the HTML document of the target page
    struct CURLResponse response = GetRequest(curl_handle, citation->URL);

    // parse the HTML document returned by the server
    htmlDocPtr doc = htmlReadMemory(response.html, (unsigned long)response.size, NULL, NULL, HTML_PARSE_NOERROR);
    xmlXPathContextPtr context = xmlXPathNewContext(doc);

    // get the HTML elements

    /*
    * TITLE         : Web Scraping with C
    * AUTHOR        : Fimber (Kasarachi) Elemuwa
    * DATE          : October 14, 2024 
    * VERSION       : 1.0
    * AVAILABIILTY  : https://scrape.do/blog/web-scraping-with-c/
    */

    // Read Application JSON data
    xmlChar* xpath = (xmlChar*)"//script[contains(@type, 'application/ld+json')]";
    if (xpath != NULL) {
        xmlXPathContextPtr xpathCtxtPtr = xmlXPathNewContext(doc);
        xmlXPathObjectPtr xpathObjPtr = xmlXPathEvalExpression(xpath, xpathCtxtPtr);
        if (xpathObjPtr->nodesetval->nodeTab == NULL) {
            return;
        }
        xmlNodePtr node = xpathObjPtr->nodesetval->nodeTab[0];

        // Store content in string
        char* json = (char*)xmlNodeGetContent(node);

        if (node != NULL) {
            // Call reading JSON to assign values
            parseJSON(json, citation);

            // Free memory
            xmlXPathFreeObject(xpathObjPtr);
            xmlXPathFreeContext(xpathCtxtPtr);
            xmlFreeDoc(doc);
            xmlCleanupParser();
        }

        // If there is no application JSON, read other data
        else {
            // Get title
            xmlChar* xpathTitle = (xmlChar*)"//title";
            if (xpath != NULL) {
                xmlXPathContextPtr xpathCtxtPtr = xmlXPathNewContext(doc);
                xmlXPathObjectPtr xpathObjPtr = xmlXPathEvalExpression(xpathTitle, xpathCtxtPtr);
                xmlNodePtr node = xpathObjPtr->nodesetval->nodeTab[0];

                // Store content in string
                char* title = (char*)xmlNodeGetContent(node);
                if (strcmp(title, "Just a moment...") != 0) {
                    citation->Title = (char*)xmlNodeGetContent(node);
                }

                // Free memory
                xmlXPathFreeObject(xpathObjPtr);
                xmlXPathFreeContext(xpathCtxtPtr);
                xmlFreeDoc(doc);
                xmlCleanupParser();
            }
        }

    }

    // cleanup the curl instance
    curl_easy_cleanup(curl_handle);
    // cleanup the curl resources
    curl_global_cleanup();
}


static size_t WriteHTMLCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    struct CURLResponse* mem = (struct CURLResponse*)userp;
    char* ptr = (char*)realloc(mem->html, mem->size + realsize + 1);

    if (!ptr)
    {
        printf("Not enough memory available (realloc returned NULL)\n");
        return 0;
    }

    mem->html = ptr;
    memcpy(&(mem->html[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->html[mem->size] = 0;

    return realsize;
}

struct CURLResponse GetRequest(CURL* curl_handle, const char* url)
{
    CURLcode res;
    struct CURLResponse response;

    // initialize the response
    response.html = (char*)malloc(1);
    response.size = 0;

    // specify URL to GET
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    // send all data returned by the server to WriteHTMLCallback
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteHTMLCallback);
    // pass "response" to the callback function
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&response);
    // set a User-Agent header
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/117.0.0.0 Safari/537.36");
    // perform the GET request
    res = curl_easy_perform(curl_handle);

    // check for HTTP errors
    if (res != CURLE_OK)
    {
        fprintf(stderr, "GET request failed: %s\n", curl_easy_strerror(res));
    }

    return response;
}