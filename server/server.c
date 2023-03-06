/**
 * @file server.c
 * @authors Nuggets; Charles Angles
 * @brief this program handles all the server logic to implement the Nuggets game
 *
 * @date 2023-02-28
 * CS 50, Winter 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "../libcs50/set.h"
#include "../libcs50/counters.h"
#include "../support/log.h"
#include "parseArgs.h"
#include "../support/message.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"
#include "../random/random.h"
#include "../grid/grid.h"
#include "../visibility/visibility.h"

/**************** global types ****************/
// Global Constants
static const int MaxNameLength = 50; // max number of chars in playerName
static const int MaxPlayers = 26;    // maximum number of players
static const char floor = '.';
static const char side_wall = '-';
static const char vertical_wall = '|';
static const char tunnel = '#';
static const char corner = '+';
static const char nugget = '*';

/**
* game_t - this struct holds important information about the running of the game
*/
typedef struct game
{
    int GoldTotal;       // amount of gold in the game
    int GoldMinNumPiles; // minimum number of gold piles
    int GoldMaxNumPiles; // maximum number of gold piles
    char *map;           // current complete map
    int currPlayers;     // amount of current players in the game
    addr_t spectatorAddress; // address of the spectator if there is one
    counters_t *goldMap; // a counters with gold locations and their gold

} game_t;

// global declaration
game_t *game;
/**
* player_t - this struct holds informations unique to each player object
*/
typedef struct player
{
    bool inGame;              // if true, the player is in the game, once they quit this becomes false
    bool isInitalized;        // has the player been initialized yet
    char name[150];           // name of player
    char letter;              // the letter representation on the map
    addr_t address;           // address specific to the client
    point_t *currentLocation; // point representing the current x and y location of the player
    char previousPoint;       // point that was replaced by the player letter
    int playerGold;           // total gold in the players purse
    int recentGold;           // most recent gold pickup
    counters_t *pointsSeen;   // counters containing all the locations that have been seen by a player
} player_t;
/**
* point - this struct holds an x and y value to represent a specific point on the map
*/
typedef struct point
{
    int x;
    int y;
} point_t;

// Function decclarations
static bool handleMessage(void *arg, const addr_t from, const char *message);
static void initializeGameData(char *filename, int seed);
static void deletegameStruct();
static void addPlayer(char *name, addr_t address, void *playerSet);
static player_t *getPlayer(addr_t address, void *playerSet);
static int processMove(player_t *player, void *playerSet, int x, int y);
void gold(player_t *player);
static bool handleKey(char *key, void *playerSet, addr_t address);
set_t *initializePlayerSet(int maxPlayers);
void playerSwap(int oldX, int oldY, player_t *player, void *playerSet);
static void deletePlayerSet(set_t *playerSet);
static void playerDelete(void *item);
void initializeSpectator();
void updateDisplay(void *playerSet);
static void Table(void *playerSet);
static void playerQuit(player_t *player);
void endGame(void *playerSet);
bool goldHelper(player_t *player, void *playerSet);
/********************************************************************************/
/*EXTRA CREDIT PROTOTYPES FOR BOTH PLAYER DROPPING AND STEALING GOLD*/
//static void dropGold(player_t* player, void* playerSet)

/**
 * @brief The main function initializes the game data based on whether there was a seed passed in or not 
 * exits different values based on faulty inputs
 * then calls message loop
 * 
 * @param argc argument count
 * @param argv array containing passed args
 * @return int exits non-zero if there is an error in the arguments, 0 if it runs correctly
 */
int main(const int argc, char *argv[])
{
    int parseArgsValue;
    parseArgsValue = parseArgs(argc, argv);
    char *filename = argv[1];

    if (parseArgsValue == 0)
    { // only valid map path provided
        initializeGameData(filename, -1);
    }
    else if (parseArgsValue == 1)
    { // valid map path and seed provided
        int seed = atoi(argv[2]);
        initializeGameData(filename, seed);
    }
    else if (parseArgsValue == 2)
    { // invalid inputs
        exit(2);
    }
    else if (parseArgsValue == 3)
    { // error with reading mapPath
        exit(3);
    }
    else if (parseArgsValue == 4)
    { // error with invalid seed (either negative or non integer)
        exit(4);
    }

    int port = message_init(NULL);
    printf("message_init: ready at port %d\n", port);

    set_t *playerSet = initializePlayerSet(MaxPlayers);

    message_loop(playerSet, 0, NULL, NULL, handleMessage);
    message_done();
    return 0;
}

/**
 * @brief The message handler that is within message loop interpreting messages coming from client
 * Arg is the playerSet of all players
 * from is the address the message was received from
 *  message is the char* message received
 * 
 * @param arg playerSet is passed into the arg
 * @param from the address from which the message came from
 * @param message the message from the client
 * @return bool returns true to end the loop, false otherwise
 */
static bool
handleMessage(void *arg, addr_t from, const char *message)
{
    // splits the message into an array 
    char *word = malloc(strlen(message) + 1);
    strcpy(word, message);
    char **wordArray = calloc(sizeof(char *), strlen(word) + 1);
    int size = strlen(word);
    int count = 0;
    int i = 0;
    char *termchar;

    for (i = 0; i < size; i++)
    {
        if (isgraph(message[i]) && message[i] != '\"')
        {
            // put the first letter in the word to the queryarray as a char*
            wordArray[count++] = &word[i];

            // accumulate a word while the following characters are alphaticals
            int v = word[i];
            while (isgraph(v) && v != '\"')
            {
                v = word[i];
                i++;
            }
            // add a terminating character
            termchar = &word[i - 1];
            *termchar = '\0';
            i = i - 1;
        }
    }
    // if the message is PLAY 
    if (message[0] == 'P')
    {
        
        char name[1000] = "";
        for (int i = 1; i < count; i++)
        {
            strcat(name, wordArray[i]);
            strcat(name, " ");
        }
        if (strlen(name) < 1)
        {
            message_send(from, "QUIT Sorry - you must provide players name");
        }
        else
        {
            addPlayer(name, from, arg);
        }
    }
    // if the message is SPECTATE 
    else if (message[0] == 'S')
    {
        if (!message_isAddr(game->spectatorAddress))
        {
            game->spectatorAddress = from;
            initializeSpectator();
        }
        else
        {
            
            message_send(game->spectatorAddress, "QUIT You have been replaced by a new spectator.");
            game->spectatorAddress = from;
            initializeSpectator();
        }
    }
    // if the message is a KEY
    else if (message[0] == 'K')
    {
        if (count > 1)
        {
            // this if statement checks if the gold remaining goes to 0
            if (handleKey(wordArray[1], arg, from))
            {
                free(word);
                free(wordArray);
                return true;
            }
        }
        else
        {
            //in case client makes a mistake
            message_send(from, "ERROR unknown keystroke");  
        }
    }
    else
    {
        //in case client makes a mistake
        message_send(from, "ERROR unknown keystroke");
    }
    // free to make space for next message
    free(word);
    free(wordArray);
    fflush(stdout);
    return false;
}

/**
 * @brief initializes the spectator client and send messages to the client with grid, gold, display
 *
 *
 *
 */
void initializeSpectator()
{
    // concatenate messages before sending to spectator
    char gridMessage[100] = "";
    // add GRID to message
    strcat(gridMessage, "GRID "); 
    char rowsNCols[10];
    // add rows and columns
    sprintf(rowsNCols, "%d %d", calculateRows(game->map), calculateColumns(game->map)); 
    // concatenate with GRID message
    strcat(gridMessage, rowsNCols);                                                     
    char goldMessage[100] = "";
    strcat(goldMessage, "GOLD ");
    char gold[20];
    // concatenate and add the most recent gold pickup with GOLD message
    sprintf(gold, "0 0 %d", game->GoldTotal); 
    strcat(goldMessage, gold);
    char displayMessage[10000] = "";
    strcat(displayMessage, "DISPLAY\n");
    strcat(displayMessage, game->map); // concatenate map text with DISPLAY message
    message_send((game->spectatorAddress), gridMessage);
    message_send((game->spectatorAddress), goldMessage);
    message_send((game->spectatorAddress), displayMessage);
}

/**
 * @brief initializes game structs values
 *
 * @return game_t*  game struct with filled parameters
 */
static void initializeGameData(char *filename, int seed)
{
    // initialize neccessary constants
    game = malloc(sizeof(game_t));
    game->GoldTotal = 250;
    game->GoldMinNumPiles = 10;
    game->GoldMaxNumPiles = 30;
    game->spectatorAddress = message_noAddr();
    game->currPlayers = 0;

    // open and read the map and assign it in the game struct
    FILE *fp;
    fp = fopen(filename, "r");
    char *map = file_readFile(fp);
    fclose(fp);
    game->map = map;

    // create counters for the gold and pass it to create random behaviour
    counters_t *goldmap = counters_new();
    random(game, goldmap, seed);
    // assign it to the gamestruct
    game->goldMap = goldmap;
}

/**
 * @brief initializes set of player objects
 *
 * @param maxPlayers takes in amount of player objects to create
 * @return set_t* returns the set of players
 */
set_t *initializePlayerSet(int maxPlayers)
{
    // create set
    set_t *playerSet = set_new();
    char key[3];
    // loop through and make the key a value 1-26
    for (int i = 1; i <= maxPlayers; i++)
    {
        sprintf(key, "%d", i);
        // create a player object and insert it at that key
        player_t *player = malloc(sizeof(player_t));
        player->isInitalized = false;
        set_insert(playerSet, key, player);
    }
    return playerSet;
}

/**
 * @brief deletes malloc'd object in the game struct
 *
 */
static void deletegameStruct()
{
    counters_delete(game->goldMap);
    free(game->map);
    free(game);
}

/**
 * @brief adds a player to the game, such that it can be used by the client
 *
 * @param name name of player
 * @param address address of player
 * @param playerSet set of players
 */
static void addPlayer(char *name, addr_t address, void *playerSet)
{
    // max players check
    if ((game->currPlayers + 1) >= MaxPlayers)
    {
        message_send(address, "QUIT Game is full: no more players can join.");
        return;
    }
    set_t *pSet = playerSet;
    // get the empty player object at the empty point at the set
    int i = game->currPlayers + 1;
    char key[3];
    sprintf(key, "%d", i);
    player_t *player = set_find(pSet, key);

    // assign address
    player->address = address;
    // check string length and truncate if neccessary
    int sizeOfName = strlen(name);
    if (sizeOfName > MaxNameLength + 1)
    {
        name[MaxNameLength] = '\0';
    }

    // replace character with an underscore if isgraph and isspace are false
    for (int i = 0; i < strlen(name); i++)
    {
        if (!isgraph(name[i]) && !isspace(name[i]))
        {
            name[i] = '_';
        }
    }
    // assign name
    strcpy(player->name, name);
    // create an empty counter for points seen
    player->pointsSeen = counters_new();
    // intitialize gold values to 0
    player->playerGold = 0;
    player->recentGold = 0;
    // picking a random spawn location for the player
    int spawn = spawnLocation(game);
    point_t *point = locationToPoint(spawn, game->map);
    player->currentLocation = point;
    // setting previous point to a floor because the spawn can only be on a floor
    player->previousPoint = floor;
    // add one to account for the fact current players starts at 0
    int playerNumber = game->currPlayers;
    // increment the letter's ascii by the player number
    char letter = 'A' + playerNumber;
    // assign it to player
    player->letter = letter;
    // update map to add player
    setCharAtPoint(game->map, player->letter, player->currentLocation);
    // set inGame to true to signl they are in the game
    player->inGame = true;
    // set boolean in player struct to show its been initialzed
    player->isInitalized = true;
    // increment game's current players by one
    game->currPlayers += 1;
    // send OK L, where L is the player letter
    char c[10] = "OK ";
    c[3] = player->letter;
    c[4] = '\0';

    message_send(player->address, c);

    // send them game info
    char gridMessage[100] = "";
    strcat(gridMessage, "GRID "); // add GRID to message
    char rowsNCols[10];
    // add rows and columns
    sprintf(rowsNCols, "%d %d", calculateRows(game->map), calculateColumns(game->map)); 
    // concatenate with GRID message
    strcat(gridMessage, rowsNCols);                                                     
    message_send(player->address, gridMessage);
    updateDisplay(playerSet);
}
/**
 * @brief Get the Player object based on the address
 *
 * @param address address of player we want to get
 * @param playerSet set of player objects
 * @return player_t* player with the address we were looking for
 */
static player_t *getPlayer(addr_t address, void *playerSet)
{
    // check the address isnt the spectator address
    if (message_eqAddr(address, game->spectatorAddress))
    {
        return NULL;
    }
    // loop through player set and look for the address
    playerSet = (set_t *)playerSet;
    int totalPlayers = 26;
    char c[10];
    for (int keyCount = 1; keyCount <= totalPlayers; keyCount++)
    {
        sprintf(c, "%d", keyCount);
        player_t *player = set_find(playerSet, c);
        if (player->isInitalized != false)
        {
            if (message_eqAddr(player->address, address))
            {
                return player;
            }
        }
    }
    // if for some reason we have not found the address
    return NULL; 
}

/**
 * processMove 
 * @brief given an x and y as well as a player and the player set, finds the point if the 
 *      x and y are added to the players location to see where the key would place them. Carries out
 *      various functions depending on what the character is at that spot
 *
 * 0 - invalid move
 * 1 - valid open square
 * 2 - gold
 * 3 - hits a player
 * int x is -1,0,1 -1 left, 0 no move, 1 right
 * int y is -1,o,1 -1 up, 0 no move, 1 down NOTE THIS IS REVERSED BECAUSE OF HOW THE MAP IS PRINTED
 */

static int
processMove(player_t *player, void *playerSet, int x, int y)
{
    // get the location the player is currently at
    int oldX = getX(player->currentLocation);
    int oldY = getY(player->currentLocation);
    x += getX(player->currentLocation);
    y += getY(player->currentLocation);
    // if the possible move is a wall
    if (getCharFromPair(x, y, game->map) == side_wall || getCharFromPair(x, y, game->map) == corner || getCharFromPair(x, y, game->map) == vertical_wall || isspace(getCharFromPair(x, y, game->map)))
    {
        return 0;
    }
    // if the possible move is a valid 
    else if (getCharFromPair(x, y, game->map) == floor || getCharFromPair(x, y, game->map) == tunnel)
    {
        // update players current location
        setY(y, player->currentLocation);
        setX(x, player->currentLocation);
        // create a point object to hold the old point
        point_t *oldPoint = malloc(sizeof(point_t));
        setX(oldX, oldPoint);
        setY(oldY, oldPoint);
        // store the new coming previous point
        char recentPrevPoint = getCharAtPoint(player->currentLocation, game->map);
        // set the letter and the previous point
        setCharAtPoint(game->map, player->letter, player->currentLocation);
        setCharAtPoint(game->map, player->previousPoint, oldPoint);
        free(oldPoint);
        // update previous point
        player->previousPoint = recentPrevPoint;
        return 1;
    }
    // if the possible move is a gold
    else if (getCharFromPair(x, y, game->map) == nugget)
    {
        setY(y, player->currentLocation);
        setX(x, player->currentLocation);
        point_t *oldPoint = malloc(sizeof(point_t));
        oldPoint->x = oldX;
        oldPoint->y = oldY;
        setCharAtPoint(game->map, player->letter, player->currentLocation);
        setCharAtPoint(game->map, player->previousPoint, oldPoint);
        free(oldPoint);
        player->previousPoint = floor;
        if (goldHelper(player, playerSet))
        {
            return 2;
        }
        return 1;
    }
    // if the possible move is another player
    else if (isalpha(getCharFromPair(x, y, game->map)))
    {
        setY(y, player->currentLocation);
        setX(x, player->currentLocation);
        playerSwap(oldX, oldY, player, playerSet);
        return 1;
    }
    return 0; // if some sort of error to prevent nothing from being returned
}

/*
h move left, if possible
l move right, if possible
j move down, if possible
k move up , if possible
y move diagonally up and left, if possible
u move diagonally up and right, if possible
b move diagonally down and left, if possible
n move diagonally down and right, if possible*/
/**
 * @brief handles the individual keystrokes for movement and quitting
 *        for the keystrokes we call process move which takes different
 *        values of x and y to move the player in each direction
 *
 *        for example: keying 'n' moves down and right, so we would pass x = + 1 y = + 1
 *        to check the spot down and right
 *
 * @param key the key passed by the client
 * @param playerSet set of players
 * @param address address the key came from
 */
static bool handleKey(char *key, void *playerSet, addr_t address)
{
    // get the player at the address
    player_t *player = getPlayer(address, playerSet);
    int moveResult = 0;
    // quit
    if (strcmp(key, "Q") == 0)
    {
        if (message_eqAddr(address, game->spectatorAddress))
        {
            message_send(game->spectatorAddress, "QUIT Thanks for watching!");
            game->spectatorAddress = message_noAddr();
        }
        else
        {
            playerQuit(player);
            message_send(address, "QUIT Thanks for playing!");
        }
    }

    // check to see if the key came from the spectator
    if (message_isAddr(game->spectatorAddress) && message_eqAddr(address, game->spectatorAddress))
    {
        updateDisplay(playerSet);
        return false;
    }
    // If spectator exists and address is not spectator address
    if (player != NULL)
    {
        // for capital letters call the move until it cannot be called again
        if (strcmp(key, "H") == 0)
        {
            // checking if it returns 0 means the move was valid, and 2 checks if all gold was gotten
            while ((moveResult = processMove(player, playerSet, -1, 0)) != 0 && moveResult != 2)
            {
                // update display for all players after each move
                updateDisplay(playerSet);
            }
        }
        // call the function process move for a lowercase key once
        else if (strcmp(key, "h") == 0)
        {
            // sets return value of process move to moveresult to check it later
            moveResult = processMove(player, playerSet, -1, 0);
        }
        else if (strcmp(key, "L") == 0)
        {
            while ((moveResult = processMove(player, playerSet, 1, 0)) != 0 && moveResult != 2)
            {
                updateDisplay(playerSet);
            }
        }
        else if (strcmp(key, "l") == 0)
        {
            moveResult = processMove(player, playerSet, 1, 0);
        }
        else if (strcmp(key, "J") == 0)
        {
            while ((moveResult = processMove(player, playerSet, 0, 1)) != 0 && moveResult != 2)
            {
                updateDisplay(playerSet);
            }
        }
        else if (strcmp(key, "j") == 0)
        {
            moveResult = processMove(player, playerSet, 0, 1);
        }
        else if (strcmp(key, "K") == 0)
        {
            while ((moveResult = processMove(player, playerSet, 0, -1)) != 0 && moveResult != 2)
            {
                updateDisplay(playerSet);
            }
        }
        else if (strcmp(key, "k") == 0)
        {
            moveResult = processMove(player, playerSet, 0, -1);
        }
        else if (strcmp(key, "Y") == 0)
        {
            while ((moveResult = processMove(player, playerSet, -1, -1)) != 0 && moveResult != 2)
            {
                updateDisplay(playerSet);
            }
        }
        else if (strcmp(key, "y") == 0)
        {
            moveResult = processMove(player, playerSet, -1, -1);
        }
        else if (strcmp(key, "U") == 0)
        {
            while ((moveResult = processMove(player, playerSet, 1, -1)) != 0 && moveResult != 2)
            {
                updateDisplay(playerSet);
            }
        }
        else if (strcmp(key, "u") == 0)
        {
            moveResult = processMove(player, playerSet, 1, -1);
        }
        else if (strcmp(key, "b") == 0)
        {
            moveResult = processMove(player, playerSet, -1, 1);
        }
        else if (strcmp(key, "B") == 0)
        {
            while ((moveResult = processMove(player, playerSet, -1, 1)) != 0 && moveResult != 2)
            {
                updateDisplay(playerSet);
            }
        }
        else if (strcmp(key, "n") == 0)
        {
            moveResult = processMove(player, playerSet, 1, 1);
        }
        else if (strcmp(key, "N") == 0)
        {
            while ((moveResult = processMove(player, playerSet, 1, 1)) != 0 && moveResult != 2)
            {
                updateDisplay(playerSet);
            }
        }
        // if there was an invalid keystroke
        else
        {
            message_send(address, "ERROR Unknown Keystroke");
            return false;
        }
        // if the game ends return true to break out of the message loop
        if (moveResult == 2)
        {
            return true;
        }
    }

    // update display and loop through again
    updateDisplay(playerSet);
    return false;
}

/**
 * @brief calls gold and checks if no gold leftover
 *
 * @param player player who found gold
 * @param playerSet set of players
 */
bool goldHelper(player_t *player, void *playerSet)
{
    // picks up gold
    gold(player);
    // check if gold is gone and game should end
    if (game->GoldTotal <= 0)
    {
        updateDisplay(playerSet);
        endGame(playerSet);
        return true;
    }
    return false;
}

/**
 * @brief handles game process if a player collects gold
 *
 * @param player player who found gold
 */
void gold(player_t *player)
{
    // get the amount of gold in the pile that was hit
    int ncols = calculateColumns(game->map);
    int location = pointToLocation(player->currentLocation, ncols);

    //update neccessary values// 
    int g = counters_get(game->goldMap, location);
    counters_set(game->goldMap, location, 0);
    player->playerGold += g;
    player->recentGold = g;
    game->GoldTotal -= g;
}
/**
 * @brief handles process if a player walks into a space where another player is
 *
 x value wa * @param oldX
 * @param oldY
 * @param player
 * @param playerSet
 */
void playerSwap(int oldX, int oldY, player_t *player, void *playerSet)
{
    char c[10];
    for (int keyCount = 1; keyCount <= MaxPlayers; keyCount++)
    {
        // loops through and finds the player that is at the same location as the player that moved onto its point
        sprintf(c, "%d", keyCount);
        player_t *otherPlayer = set_find(playerSet, c);
        // checks only initialized
        if (otherPlayer->isInitalized != false && otherPlayer->inGame && otherPlayer->letter != player->letter)
        {
            if (getX(player->currentLocation) == getX(otherPlayer->currentLocation) && getY(player->currentLocation) == getY(otherPlayer->currentLocation))
            {
                setX(oldX, otherPlayer->currentLocation); // swaps other players X and y to the old location of the other player that moved into them
                setY(oldY, otherPlayer->currentLocation);
                setCharAtPoint(game->map, player->letter, player->currentLocation);
                setCharAtPoint(game->map, otherPlayer->letter, otherPlayer->currentLocation); // updates the map to visually swap the two players letters
                char tempPrevChar = player->previousPoint;
                player->previousPoint = otherPlayer->previousPoint; // swaps the previous points that they players were standing on so map can be redrawn when they move
                otherPlayer->previousPoint = tempPrevChar;
            }
        }
    }
}
/**
 * @brief loops through all clients and send an updated display specific
 *        to each players visibility as well as updated information on gold
 *
 * @param playerSet
 */
void updateDisplay(void *playerSet)
{

    char c[10] = "";
    for (int keyCount = 1; keyCount <= MaxPlayers; keyCount++)
    {
        sprintf(c, "%d", keyCount);
        player_t *player = set_find(playerSet, c);
        if (player->isInitalized != false)
        {
            if (player->inGame)
            {
                char goldMessage[100] = "";
                strcat(goldMessage, "GOLD ");
                char gold[30];
                sprintf(gold, "%d %d %d", player->recentGold, player->playerGold, game->GoldTotal); // concatenate and add the most recent gold pickup with GOLD message
                strcat(goldMessage, gold);
                message_send(player->address, goldMessage);
                if (player->recentGold != 0){
                    player->recentGold = 0;
                }
                char displayMessage[10000] = "";
                strcat(displayMessage, "DISPLAY\n");
                char *tempMap = findVisibility(player, game->map);
                strcat(displayMessage, tempMap); // concatenate map text with the visible map
                message_send(player->address, displayMessage);
                free(tempMap);
            }
        }
    }

    if (message_isAddr(game->spectatorAddress))
    {
        char goldMessage[100] = "";
        strcat(goldMessage, "GOLD ");
        char gold[30];
        sprintf(gold, "0 0 %d", game->GoldTotal); // concatenate and add the most recent gold pickup with GOLD message
        strcat(goldMessage, gold);
        message_send(game->spectatorAddress, goldMessage);
        char displayMessage[10000] = "";
        strcat(displayMessage, "DISPLAY\n");
        strcat(displayMessage, game->map); // concatenate map text with the visible map
        message_send(game->spectatorAddress, displayMessage);
    }
}

/**
 * @brief deletes the playerSet containing player structs
 *
 * @param playerSet
 */
static void deletePlayerSet(set_t *playerSet)
{
    set_delete(playerSet, playerDelete);
}

/**
 * @brief helper function to delete players
 *
 * @param item
 */
static void playerDelete(void *item)
{
    player_t *player = item;
    if (player->isInitalized)
    {
        if (player->pointsSeen != NULL)
        {
            counters_delete(player->pointsSeen);
        }
        free(player->currentLocation);
    }
    free(player);
}
/**
 * @brief Carrys out processes needed when a player quits
 *
 * @param player
 */
static void playerQuit(player_t *player)
{
    player->inGame = false;
    setCharAtPoint(game->map, player->previousPoint, player->currentLocation);
}

void endGame(void *playerSet)
{
    Table(playerSet);
    deletegameStruct();
    deletePlayerSet(playerSet);
}

/**
* @brief
*
* @param playerSet
*
* QUIT GAME OVER:
A          4 Alice
B         16 Bob
C        230 Carol
*/
static void Table(void *playerSet)
{
    // size of the table based on the amount of characters in the rows
    // maximum 1 for the letter, 1 for a tab, 3 digits in the purse, 50 chars max in name
    // and a newline character, 26 max characetrs and one extra because of the first line

    char table[message_MaxBytes];
    strcpy(table, "QUIT GAME OVER:\n");
    char c[10];
    // allocate a buffer to hold the formatted string
    char buffer[100];
    // loop through set and add the information from each player into the
    // table string
    for (int keyCount = 1; keyCount <= MaxPlayers; keyCount++)
    {
        sprintf(c, "%d", keyCount);
        player_t *player = set_find(playerSet, c);
        if (player->isInitalized != false)
        {
            // get neccessary values to put in table
            char letter = player->letter;
            int purse = player->playerGold;
            char *name = player->name;

            // format the string and store it in the buffer
            sprintf(buffer, "%c\t%d %s\n", letter, purse, name);
            // add it to table
            strncat(table, buffer, 100);
        }
    }
    // loop through the players and send the table string to all
    for (int keyCount = 1; keyCount <= MaxPlayers; keyCount++)
    {
        sprintf(c, "%d", keyCount);
        player_t *player = set_find(playerSet, c);
        if (player->isInitalized != false && player->inGame)
        {
            message_send(player->address, table);
        }
        if (message_isAddr(game->spectatorAddress))
        {
            message_send(game->spectatorAddress, table);
        }
    }
}

// /**
//  * @brief ***EXTRA CREDIT*** if a player quits they drop the gold in their last location
//  * 
//  * @param player 
//  * @param playerSet 
//  */
// static void dropGold(player_t* player, void* playerSet){
//     int location = pointToLocation(player->currentLocation, calculateColumns(game->map));
//     counters_set(game->goldMap, location, player->playerGold);

//     setCharAtPoint(game->map, nugget, player->currentLocation);
//     updateDisplay();
// }

// /**
//  * @brief ***EXTRA CREDIT*** if a player quits they drop the gold in their last location
//  * 
//  * @param player 
//  * @param playerSet 
//  */
// static void stealGold(player_t* player, void* playerSet){
//     int location = pointToLocation(player->currentLocation, calculateColumns(game->map));
//     counters_set(game->goldMap, location, player->playerGold);

//     setCharAtPoint(game->map, nugget, player->currentLocation);
// }
