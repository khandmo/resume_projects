Zion Slaughter
CS50 Lab 5
IMPLEMENTATION.md - short description of implementation of indexer

Indexer uses a new data structure - the Index - made of a hashtable holding counters sets as items.
Each hashtable entry is based on hash_jenkins-ing the words found in the document, and the counters
show which number document file  the word is found in as well as how many times the word is found.

The Indexer is made of three main functions:

MAIN()
The main function parses the two arguements of the indexer, namely, the pageDirectory
and fileOutputName. It is responsible for opening the file fileOutputname and calls the
second function indexBuild.
Also responsible for deleting the index and closing the file.

indexBUILD()
indexBuild creates a new index objects, loops for docID numbers and tries
to load a webpage from pageDirectory/id and if successful, passes webpage
to the third function: indexPage.
Returns full index_t* object when done.

indexPAGE()
indexpage loops through each word of the page input and skips words of length less than
three, normalizes each word with the external normalizeWord() function found in /common/index.c,
looks up each word in the index, and adds to the index if not already present, or
increments the counter if the word is already there.

PROTOTYPES:
static index_t* indexBuild(char* pageDirectory);
static void indexPage(webpage_t* page, index_t* theIndex, int docID
int main(int argc, char* argv[]);


The Index Data Structure has multiple functions to make a new index, delete an index,
save and index into an open file, and a function to print each word in the index and cycle
through its counters.

PROTOTYPES:
index_t* index_new();
void index_delete(index_t* indexObj);
index_t* index_save(FILE* loadedFile);
void indexPrint(void* file, const char* word, void* counter);

The Index Data Structure utilizes a few static functions which are not among those stated above
and cannot be accessed from outside index.c.



The Word file only includes the function to normalize a word.

PROTOTYPES:
char* normalizeWord(char* word);