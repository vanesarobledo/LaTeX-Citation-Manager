![Visual Studio](https://img.shields.io/badge/Visual%20Studio-5C2D91.svg?style=for-the-badge&logo=visual-studio&logoColor=white) ![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white) ![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white) 

# About LaTeX Citation Manager
LaTeX Citation Manager is a command-line based tool designed to organize bibliographies and generate BibLaTeX files for use in [LaTeX](https://www.latex-project.org/) documents.

The current iteration supports website citations. A text file of line-separated URLs can be imported, or manually entered in the command line. The program will prompt for data that can be added for a website citation. Once the citations have been processed, they can be exported into a `.bib` file that can be directly added to a LaTeX document for use in citations.

This is a version my final project for [SENG1050: Data Structures at Conestoga College](https://github.com.mcas.ms/vanesarobledo/SENG1050-Final-Project). This version includes web scraping in C.

## What is LaTeX?
**LaTeX** is a document typesetting method analogous to other markup languages such as HTML and Markdown. It is the standard for scientific publishing. LaTeX allows researchers to focus on the content of the papers and adjusting the formatting later, including citation style.

LaTeX offers many packages that manage bibliographies. One such a package is [BibLaTeX](https://github.com/plk/biblatex), which allows the storage of citations using name-value pairs depending on the type of source. For example, when citing a website, the `.tex` markup would be stored as:

```bibtex
@online{WebsiteCiteKey,
	author = {Author or editor of website},
	title = {Title of webpage},
	year = {Year of publication},
	url = {URL to website},
	urldate = {Date website was accessed}
}
```

Each citation has a unique key (`WebsiteCiteKey`) that can be called upon anywhere in the document to do inline citations.

**Suggested Resources**
- [Learn LaTeX in 30 Minutes](https://www.overleaf.com/learn/latex/Learn_LaTeX_in_30_minutes)
- [Getting started with BibLaTeX](https://www.overleaf.com/learn/latex/Articles/Getting_started_with_BibLaTeX)

**Suggested Research Flow**
- As you are doing research, bookmark or save websites that might be useful.
- Take notes on your research. Use each website URL as a heading and summarize pertinent information or save quotes.
- Once you have a finalized list of the research you are citing in your document, save a text file of all of the URLs you are using, separated by a new line, and import it into LaTeX Citation Manager.
- Depending on the source, you may not want to use the `@online` modifier. Articles may be better suited with `@article` - multiple types of citations support the `url` field, so choose the citation wisely.
# Getting Started

## Prerequisites
LaTeX Citation Manager is built in C/C++ using Visual Studio 2022 Community Edition.
- **OS**: This program is only compatible with Windows. Windows 10/11 is recommended
- **Compiler**: Visual Studio 2022 is recommended, since there are calls to Microsoft's safe functions in this code.

## Dependencies
- **vcpkg** with the following libraries:
	- [libcurl](https://everything.curl.dev/install/windows/win-vcpkg.html)
	- [libxml2](https://vcpkg.io/en/package/libxml2.html)
	- [json-c](https://github.com/json-c/json-c?tab=readme-ov-file#buildvcpkg)

## Installation
1. Ensure vcpkg is installed. See [Installing vcpkg on Windows](https://www.studyplan.dev/pro-cpp/vcpkg-windows) for an easy guide.
2. Install `libcurl`, `libxml2`, and `json-c` packages.
3. Open the solution file using Visual Studio.
4. Click on "Local Windows Debugger" to run the program in debug mode.
5. Or: Create a `.exe` by right-clicking on the solution and click on "Build Solution" to compile the code.
   
# Usage

## Quick Import & Export
For quick usage, data can be added quickly from the command line using command line arguments.

Run the following command in the folder containing the `.exe` (contained in the `Debug` folder), replacing "<import.txt>" with the path to the file containing line-separated website URLs:

```bash
./SENG1050-Final-Project -i <import.txt>
```

This will automatically create `references.bib` in the same directory as the `.exe`.

To try the experimental web scraping feature, change the flag to "-w" instead. Note that not all data will be retrieved.

## Importing Citations
1. To import website citations, create a text-based file with all of the website URLs, separated by line, and place them in the same directory as the `.exe`, or copy its path.
2. Select '0' in the main console interface, and then type the name of the file or its path.
3. Once successful, the citations will be loaded into the program.
	- The "Date Accessed" field will automatically be configured to the date the program is running.
## Adding a Citation
1. Citations can also be manually added by selecting '1' in the main console interface, and entering the website URL.
2. You will be prompted to enter additional information:
	- **Author** (optional): The author of the website. You may also fill in the name of the website if there is no author.
		- Multiple Authors are separated by "and" or a semicolon
		- Depending on the citation style you will end up using, the author may be changed to First Initial + Last Name format or Last Name, First Initial format. For best results, stick to the citation style you know you will be using.
	- **Title**: The title of the webpage.
	- **Year** (optional): The year the website or article was published.
## Updating a Citation
1. To update the information of the citation, select '2' in the main console interface and enter the website URL.
2. You will be prompted to enter the information needed for citation.
3. If you press enter without inputting anything, the stored information will not update.
## Removing a Citation
1. To delete a citation, select '3' in the main console interface and enter the website URL to be removed.
2. This will remove the citation, even if it has already been processed.
## Processing Citations
1. Citations must be processed before they are exported in the main program. Select '4' in the main console interface once you have imported or added citations.
2. You will be prompted for 3 options:
	1. **Add data for all citations**: You will be prompted to enter information for every citation that you have added to the program.
	2. **Sort citations alphabetically**: This option will sort all of the citations alphabetically in order of priority:
		- Author
		- Title
		- Website URL
	3. **Attempt web scraping for all citations**: This option will attempt to scrape the web for citation data. Not every field will be filled, but some data may be found automatically.
	4. **Finish processing all citations**: Once you have finished adding data to your citations and/or have sorted them, you can select this option to make it ready for exporting to a file.
 	5. **Cancel processing citations**: This will exit the menu for processing citations, and they will not be ready to be exported to a file. 
3. All the processed citations will print to the screen once all data has been added or sorted. Note that the alphabetical sort will display the list of citations in reverse-alphabetical order - once it exports to a file, it will be in alphabetical order.
## Exporting Citations
1. To export processed citations, select '5' in the main console interface.
2. Type the name of the bibliography file - it will automatically append the `.bib`.
3. A file of all processed citations will be created in the same directory as the program.

## What to do with your `.bib` file
Once you have a bibliography file, import it into your LaTeX project.

While there are many ways to compile LaTeX documents, my favourite has been using the online tool [Overleaf](https://www.overleaf.com/), because installing LaTeX packages and using a compiler on Windows is an unnecessarily tedious task.

Although it isn't strictly necessary, you can change each `WebsiteCiteKey#` to a more informative key. To create an inline citation, use the following syntax:

```tex
\autocite{WebsiteCiteKey#}
```

There are many ways to display bibliographies using LaTeX, so I suggest reading through [bibtex](https://ctan.org/topic/bibtex-doc?lang=en) or [BibLaTeX](https://ctan.org/pkg/biblatex?lang=en) documentation to find how to customize citations for your particular needs.

# Contact
Vanesa Robledo - vanesa@robledo.ca

**Project Link**: https://github.com/vanesarobledo/LaTeX-Citation-Manager/

# Acknowledgements
- Madhan Chandrasekaran - the instructor for SENG1050 Data Structures
- [djb2 Function](https://theartincode.stanis.me/008-djb2/), with ideas for modification taken from [\[PSET5\] djb2 Hash Function](https://www.reddit.com/r/cs50/comments/ggvgvm/pset5_djb2_hash_function/) thread on Reddit for the hashing function for the hash table
- [Web Scraping with C in 2025 on Zenrows][https://www.zenrows.com/blog/web-scraping-c], where the majority of the web scraping code comes from
- [Web Scraping with C on scrape.do](https://scrape.do/blog/web-scraping-with-c/), where some of the web scraping code comes from
- [Json-c Tutorial Index](https://github.com/rbtylee/tutorial-jsonc/blob/master/tutorial/index.md) 
