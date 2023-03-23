#Zion Slaughter

##CS50 Tiny Search Engine


Querier allows for functionality such that a user may search the crawled Dartmouth webpages
for words (no numbers, more than three letters). One may also utilize "and" (or implicitly
a space between two queries) and/or "or" to group queries and search for intersections/unions
of sets of webpages respectively.

To begin the querier, the first argument should be the data folder of the crawl you wish to
analyze, and the second argument should be the .ndx file created by the indexer for that respective folder.<br>
Example of these for the largest sets are below.

./querier ../data/toscrape-2 ../data/toscrape-2/indexItem.ndx<br>
./querier ../data/toscrape-3 ../data/toscrape-3/indexItem.ndx<br>
./querier ../data/wikipedia-2 ../data/wikipedia-2/indexItem.ndx<br>

Querier output will display the 'score' (amount of hits on a particular URL of the query),
followed by the page number (how to find the page in the appropriate data folder), followed
by the URL of the page itself.

To end the querier, Ctrl+C.