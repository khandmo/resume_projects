#Zion Slaughter
##CS50 Tiny Search Engine
##Crawler implementation

Crawler utilizes multiple dependcies as appropriated in the Makefile and top level README to crawl dartmouth webpages
and saves the raw html files to a data folder in the top level. The crawl takes as arguments the following:
1. The root page URL to crawl for subsequent pages
2. The route to the data folder in which to save the html of the pages, should start with ../data
3. The depth to which to crawl, a number hard coded between 0 and 10

Dependencies:
The pagedir.c module hardcodes the prefix for the directory where pages are saved. That prefix is ../data as indicated
in the preparing files.
Multiple parts of the code relies on not only that the folder which pages are saved to is named "data" but also that it exists
outside the crawler directory.