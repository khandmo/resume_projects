#Zion Slaughter
##CS50 Tiny Search Engine

THe indexer takes two arguments: the location in the data folder
at which the user wishes an index to be formed, and the location
and file name under which the user wishes the index to be written to,
as an .ndx file. The testing.sh file will show in more detail the manner in
which this is done. The indexer serves to organize words with more than three letters
in a hashtable with a integer representing the amount of times it is seen in a
given document. It is the bridge between the raw html files written by
the crawler and the querier which ranks pages based on the frequency
of queried words.

I took the liberty of indexing the following data folders for query,
those with asterisks are the largest and therefore will have more words
available for query:
letters-1
letters-2
toscrape-1
toscrape-2 **
toscrape-3 **
wikipedia-1 
wikipedia-2 **
