# CS50 TSE
## Zion Slaughter (khandmo)

## CRAWLER:
The crawler crawls Dartmouth webpages at a specificied depth and
saves raw html files of the webpages to the data folder. I have taken
the liberty of crawling a few pages, which you may find in the data
folder already. The numbers at the end signify how deep each crawl was.

## INDEXER:
The indexer searches the raw html files for words (no numbers, more than
three letters) and saves them in a .ndx file with the page number it was
found in, followed by the amount of times it was seen in that page.
I've taken the liberty of indexing a few of data folders, those which have
files labeled "indexItem.ndx" in them.

## QUERIER:
The querier allows the user to search in a particular data folder for words
or groups of words. Implicitly the querier assumes the user means "and",
and will provide webapges which hold intersections of both queries.
Explicitly one may type "and" and/or "or" in any complex manner to provide
the webpages which include these queries. As prepared, one may download all
files in this repo and go straight to querier and use the search engine.
The README.md file for this folder includes command line examples
for the largest groups of webpages.

More information for all of these processes can be found in the README.md
files in each of the folders.