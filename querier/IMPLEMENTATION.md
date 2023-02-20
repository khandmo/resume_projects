QUERIER IMPLEMENTATION SPEC
Lab 6 CS50
Zion Slaughter

Testing plan for querier and way we implemented

Testing unable to work due to complications routing input to
the program from testing.sh. A variety of solution were tried,
including echo-ing the desired inputs, and routing inputs
from a seperate file. None worked. A detailed testing campaign
would have included (and can be checked by booting querier and
typing in these queries, querier DOES work):

Testing for major error cases including:
no valid arguments
no arguments
one argument
invalid pageDirectory (querier cannot handle this)
invald index
beginning literal
end literal
consecutive literals
token with non alpha characters

Testing for output:

toscrape-3:
historical
scrape
fiction
historical and scrape and fiction
historical and scrape or fiction
historical or scrape and fiction
historical or scrape or fiction
historical and travel or fiction and academic or erotica

wikipedia:
Perhaps because of the size, personal testing yields often
segmentation faults with this set, though some cross references do work out
if the amount of times seen is deliberately kept to a minimum. I doubt
its a difficult problem to work out, but I don't have time to ensure these
larger sets work. Most importantly, the program works as a whole on functionality
and can handle most decently sized systems. I still implore the reader
to attempt.