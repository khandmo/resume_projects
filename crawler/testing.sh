#!/bin/bash

# simple letters construction
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-3 10

# out of bounds seed case
./crawler http://www.google.com/ ../data/google 5

# out of bounds too high maxDepth
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-3 14

# out of bounds too low maxDepth (below zero)
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-3 -4

# valgrind moderate case
valgrind ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/valgrind-mod 1

# letters at 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-0 0

#letters at 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-1 1

#letters at 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-2 2

#letters at 10
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-10 10

#toscrape at 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/toscrape-0 0

#toscrape at 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/toscrape-1 1

#toscrape at 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/toscrape-2 2

#toscrape at 3
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/toscrape-3 3

#wikipedia at 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia-0 0

#wikipedia at 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia-1 1

#wikipedia at 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia-2 2
