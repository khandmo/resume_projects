#!/bin/bash

# Zion Slaughter
# Lab 5 CS50
# testing.sh
#
# runs through major error cases in first phase,
# creates multiple files using the indexer and
# copies of files from the indextest module from
# crawler data

##****************** PHASE ONE ******************##

# indexer with no valid arguments
./indexer rotten apple

# indexer with no arguments
./indexer

# indexer with one argument
./indexer ../data/letters-2

# indexer with three arguments
./indexer ../data/letters-0 ../data/letters-1 ../data/letters-2

# indexer with invalid pageDirectory
./indexer ../data/notthere ../data/letters-2/indexItem.ndx

# indexer with NOT crawler directory
./indexer ../data/notCrawler ../data/letters-2/indexItem.ndx

# indexer with invalid index item  (fake path)
./indexer ../data/letters-2 ../data/letters-infinite/indexItem.ndx





##****************** PHASE TWO ******************/

# indexer with letters-2 and VALGRIND

valgrind --leak-check=full ./indexer ../data/letters-2 ../data/letters-2/indexItem.ndx

# indextest with letters-2 product and VALGRIND

valgrind --leak-check=full ./indextest ../data/letters-2/indexItem.ndx ../data/letters-2/indexRepro.ndx

# comparing outputs

~/cs50-dev/shared/tse/indexcmp ../data/letters-2/indexItem.ndx ../data/letters-2/indexRepro.ndx




# indexer with letters-3

./indexer ../data/letters-3 ../data/letters-3/indexItem.ndx

#indextest with letters-3 product

./indextest ../data/letters-3/indexItem.ndx ../data/letters-3/indexRepro.ndx


# comparing outputs

~/cs50-dev/shared/tse/indexcmp ../data/letters-3/indexItem.ndx ../data/letters-3/indexRepro.ndx





# indexer with toscrape-3

./indexer ../data/toscrape-3 ../data/toscrape-3/indexItem.ndx

# indextest with toscrape-3 product

./indextest ../data/toscrape-3/indexItem.ndx ../data/toscrape-3/indexRepro.ndx

# comparing outputs

~/cs50-dev/shared/tse/indexcmp ../data/toscrape-3/indexItem.ndx ../data/toscrape-3/indexRepro.ndx





# indexer with wikipedia-2

./indexer ../data/wikipedia-2 ../data/wikipedia-2/indexItem.ndx

# indextest with wikipedia-2 product

./indextest ../data/wikipedia-2/indexItem.ndx ../data/wikipedia-2/indexRepro.ndx

# comparing outputs

~/cs50-dev/shared/tse/indexcmp ../data/wikipedia-2/indexItem.ndx ../data/wikipedia-2/indexRepro.ndx
