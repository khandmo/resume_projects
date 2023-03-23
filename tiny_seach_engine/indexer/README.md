# Zion Slaughter

## CS50 Tiny Search Engine

The indexer takes two arguments: the location in the data folder
at which the user wishes an index to be formed, and the location
and file name under which the user wishes the index to be written to,
called **indexItem.ndx** . The testing.sh file will show in more detail the manner in
which this is done. The indexer serves to organize words with more than three letters
in a hashtable with a integer representing the amount of times it is seen in a
given document. It is the bridge between the raw html files written by
the crawler and the querier which ranks pages based on the frequency
of queried words.

I took the liberty of indexing the following data folders for query,
those with asterisks are the largest and therefore will have more words
available for query:
letters-1<br>
letters-2<br>
toscrape-1<br>
toscrape-2 **<br>
toscrape-3 **<br>
wikipedia-1 <br>
wikipedia-2 **<br>
