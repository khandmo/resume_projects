QUERIER DESIGN SPEC
Lab 6 CS50
Zion Slaughter

Describes the abstract data structures and pseudo code for querier

STRUCTURES:

Two unique structures are used: the doubleCounters and the scoreCheck.

doubleCounters is an object that holds two counters for the sake of easy
modification through existing iterate functions for the hashtable and counter data
types encountered throughout this project.

scoreCheck is an object that holds a pointer to a docID, a pointer to a count,
and a pointer to a counter object for the sake of easy scoring of docID's


FUNCTIONS:

queryParse takes a raw query and an index and normalizes the query,
prints the normalized query back to stdout, and implements the precedence
algorithm for query and literal parsing. Returns a counter object loaded with
the query result.

    Psuedocode:
        Parsing:
        Take out first token and check if literal
        Set boolean for whether or not the previous token was a literal for dupes
        While the token is not null
              Scan token for no alpha characters
              if token is literal and the previous token was literal
                 literals cannot be adjacent, exit
              if token is literal and next token is null
                 literal at end of query, exit
              if token is literal
                 set boolean to true
                 normalize word and send it to clean char*
              else  (the token is not literal)
                 set boolean to false
                 noramlize word and send it to clean char*
              set token to next token for while loop to repeat

        Query result building:
        Set up master counter and scum counter (accumulator)
        Take out first token from new, clean query
        While next token not NULL
              if next token is OR
                 Union scum and master
                 Reload while loop
                 Reset scum
              else if next token is AND
                 Intersect scum and next next token (token after AND)
                 Reload while loop
              else (next token is not literal)
                 Intersect scum and next token
                 Reload while loop
        Union scum and master
        Return master



setUnion, setIntersect, scoreResult, and counterSizer are all itemfunc type functions,
are very small, and, respectively, update a double counter object with union of inputs,
update a double counter object with intersect of inputs, update a scoreCheck object
with the largest scored item, and update an integer to give the size of a counter.


printResult takes a counter with the query result loaded, and the pageDirectory of where
the data comes from.

    PSUEDOCODE:
    Find size of result counter
    Preload a scoreCheck item and a counter to put docs that have already printed
    For every object in the result counter
        Find the highest item that has not been printed yet
        Add item to counter of docs that have been printed
        Open the docID to retreive the URL
        Print the score, docID, and URL
        close the docID file
        Reset pageDirectory (used to open the docID)
