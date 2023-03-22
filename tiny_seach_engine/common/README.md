#Zion Slaughter
##CS50 Tiny Search Engine

Common assembles three core files: index, pagedir, and word.

Index is responsible for holding the webpages for the indexer, and is
essentially a targeted hashtable. The indexer itself is responsible for
going through crawled webpages saved in the data folder and indexing the words
so that the querier is able to parse input from the user and match it to webapges
appropriately.

Pagedir is responsible for initializing directories for crawls, saving pages to
the page directory, validates page directories that were made by the crawler (contains
a .crawler file), and parses paths to prepare for webpage addition.

Word is responsible only for normalizing words from querier input.