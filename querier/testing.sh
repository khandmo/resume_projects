#!/bin/bash

# Zion Slaughter
# Lab 6 CS50
# testing.sh
#
# runs through major error cases in the first phase,
# searches a variety of words in the largest document (wiki)
# for phase two

##****************** PHASE ONE ******************##

# querier with no valid arguments
./querier rotten apple

# querier with no arguments
./querier

# querier with one argument
./querier ../data/letters-2

# querier with invalid pageDirectory
#./querier ../data/notthere ../data/letters-2/indexItem.ndx
# Whenever I attempt to make the document look for .crawler files the indexFilename gets
# written over with the string '.crawler' and all functionality is lost

# querier with invalid index
./querier ../data/letters-2 ..data/letters-2/notthere.ndx

# querier with beginning literal, end literal, consecutive literals, and non alpha inputs
./querier ../data/letters-2 ../data/letters-2/indexItem.ndx < testqueryp1




##****************** PHASE TWO ******************##
