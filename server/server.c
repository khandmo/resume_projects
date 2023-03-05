/**
 * @file server.c
 * @authors Matthew Givens; Nuggets; Charles Angles
 * @brief server
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
#include "random.h"
#include "../grid/grid.h"
#include "visibility.h"

/**************** global types ****************/
// Global Constants
static const int MaxNameLength = 50; // max number of chars in playerName
static const int MaxPlayers = 26;    // maximum number of players
// //static const int GoldTotal = 250;      // amount of gold in the game
// static const int GoldMinNumPiles= 10; // minimum number of gold piles
// static const int GoldMaxNumPiles = 30; // maximum number of gold piles

typedef struct game
{
    int GoldTotal;       // amount of gold in the game
    int GoldMinNumPiles; // minimum number of gold piles
    int GoldMaxNumPiles; // maximum number of gold piles
    char *map;           // current complete map
    int currPlayers;     // amount of current players in the game
    addr_t spectatorAddress;
    counters_t *goldMap; // a counters with gold locations and their gold

} game_t;

// global declaration
game_t *game;

typedef struct player
{
    bool inGame;              // if true, the player is in the game, once they quit this becomes false
    bool isInitalized;        // has the player been initialized yet
    char *name;               // name of player
    char letter;              // the letter representation on the map
    addr_t address;           // address specific to the client
    point_t *currentLocation; // point representing the current x and y location of the player
    char previousPoint;       // point that was replaced by the player letter
    int playerGold;           // total gold in the players purse
    int recentGold;           // most recent gold pickup
    counters_t *pointsSeen;   //
} player_t;

typedef struct spectator
{
    bool isSpectator;
    char *id;
} spectator_t;

typedef struct point
{
    int x;
    int y;
} point_t;

// Function decclarations
static bool handleMessage(void *arg, const addr_t from, const char *message);
static void deleteWordArray(char **array, int wordCount);
static void initializeGameData(char *filename, int seed);
static void deletegameStruct();
static void addPlayer(char *name, addr_t address, void *playerSet);
static player_t *getPlayer(addr_t address, void *playerSet);
static int processMove(player_t *player, void* playerSet, int x, int y);
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

/**
 * @brief
 *
 * @param argc
 * @param argv
 * @return int
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

/**************** handleMessage ****************/
/* Datagram received; print it.
 * We ignore 'arg' here.
 * Return true if any fatal error.
 */
static bool
handleMessage(void *arg, addr_t from, const char *message)
{
    printf("address is %s\n", message_stringAddr(from));
    printf("Memory address is %p\n", (void *)&from);
    char *word = malloc(strlen(message) + 1);
    strcpy(word, message);
    char **wordArray = calloc(sizeof(char *), strlen(word) + 1);
    int size = strlen(word);
    int count = 0;
    int i = 0;
    char *termchar;

    for (i = 0; i < size; i++)
    {
        if (isalpha(message[i]))
        {
            // put the first letter in the word to the queryarray as a char*
            wordArray[count++] = &word[i];

            // accumulate a word while the following characters are alphaticals
            int v = word[i];
            while (isalpha(v))
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

    if (message[0] == 'P')
    {
        //*** if the message is PLAY then we
        char name[100] = "";
        for (int i = 1; i < count; i++)
        {
            strcat(name, wordArray[i]);
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
    else if (message[0] == 'S')
    {
        printf("Adding spectator\n");
        if (!message_isAddr(game->spectatorAddress))
        {
            printf("Spectator address is null\n");
            game->spectatorAddress = from;
            initializeSpectator();
            printf("After spectator initialized\n");
        }
        else
        {
            message_send(game->spectatorAddress, "QUIT You have been replaced by a new spectator.");
            printf("PAUSE\n");
            game->spectatorAddress = from;
            initializeSpectator();
        }
    }
    else if (message[0] == 'K')
    {
        if (count > 1)
        {
            if (handleKey(wordArray[1], arg, from))
            {
                return true;
            }
        }
        else
        {
            message_send(from, "ERROR unknown keystroke");
        }
    }
    else
    {
        message_send(from, "ERROR unknown keystroke");
    }
    free(word);
    fflush(stdout);
    return false;
}

/**
 * @brief
 *
 *
 *
 * @return true
 * @return false
 */
void initializeSpectator()
{
    // ***the messages need concatenation in the strings before being sent, only one char* can be sent***
    char gridMessage[100] = "";
    strcat(gridMessage, "GRID "); // add GRID to message
    char rowsNCols[10];
    sprintf(rowsNCols, "%d %d", calculateRows(game->map), calculateColumns(game->map)); // add rows and columns
    strcat(gridMessage, rowsNCols);                                                     // concatenate with GRID message
    char goldMessage[100] = "";
    strcat(goldMessage, "GOLD ");
    char gold[20];
    sprintf(gold, "0 0 %d", game->GoldTotal); // concatenate and add the most recent gold pickup with GOLD message
    strcat(goldMessage, gold);
    char displayMessage[10000] = "";
    strcat(displayMessage, "DISPLAY\n");
    strcat(displayMessage, game->map); // concatenate map text with DISPLAY message
    message_send((game->spectatorAddress), gridMessage);
    message_send((game->spectatorAddress), goldMessage);
    message_send((game->spectatorAddress), displayMessage);
}

/**************** deleteWordArray ****************/
/* Deleted the memory allocated into the wordArray
 *
 * Caller provides:
 *   valid char** array
 *   valid int wordCount
 * We do:
 *   Nothing if NULL array
 *   Free each slot in the array that was allocated previously
 */
static void
deleteWordArray(char **array, int wordCount)
{
    if (array != NULL)
    {
        int i = 0;
        while (i < wordCount)
        {
            free(array[i]); // free slot
            i++;
        }
    }
}
/**
 * @brief initializes game structs values
 *
 * @return game_t*
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
    random2(game, goldmap, seed);
    // assign it to the gamestruct
    game->goldMap = goldmap;
}
/**
 * @brief initializes set of player objects
 *
 * @param maxPlayers
 * @return set_t*
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
    free(game);
}

/**
 * @brief adds a player to the game, such that it can be used by the client
 *
 * @param name
 * @param address
 * @param playerSet
 */
static void addPlayer(char *name, addr_t address, void *playerSet)
{
    if ((game->currPlayers + 1) >= MaxPlayers)
    {
        message_send(address, "QUIT Sorry - you must provide player's name.");
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
    if (sizeOfName > MaxNameLength)
    {
        name[MaxNameLength - 1] = '\0';
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
    player->name = name;
    // create an empty counter for points seen
    player->pointsSeen = counters_new();
    // intitialize gold values to 0
    player->playerGold = 0;
    player->recentGold = 0;
    // picking a random spawn location for the player
    int spawn = spawnLocation(game);
    point_t *point = locationToPoint(spawn, game->map);
    printf("Server - point is (%d, %d)\n", point->x, point->y);
    player->currentLocation = point;
    // setting previous point to a '.' because the spawn can only be on a '.'
    player->previousPoint = '.';
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

    char gridMessage[100] = "";
    strcat(gridMessage, "GRID "); // add GRID to message
    char rowsNCols[10];
    sprintf(rowsNCols, "%d %d", calculateRows(game->map), calculateColumns(game->map)); // add rows and columns
    strcat(gridMessage, rowsNCols);                                                     // concatenate with GRID message
    message_send(player->address, gridMessage);
    updateDisplay(playerSet);
}
/**
 * @brief Get the Player object based on the address
 *
 * @param address
 * @param playerSet
 * @return player_t*
 */
static player_t *getPlayer(addr_t address, void *playerSet)
{
    if (message_eqAddr(address, game->spectatorAddress))
    {
        return NULL;
    }
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
    return NULL; // if for some reason we have not found the address
}

/* 0 - invalid move
 * 1 - valid open square
 * 2 - gold
 * 3 - hits a player
 * int x is -1,0,1 -1 left, 0 no move, 1 right
 *  int y is -1,o,1 -1 up, 0 no move, 1 down NOTE THIS IS REVERSED BECAUSE OF HOW THE MAP IS PRINTED
 */

static int
processMove(player_t *player, void* playerSet, int x, int y)
{
    int oldX = getX(player->currentLocation);
    int oldY = getY(player->currentLocation);
    x += getX(player->currentLocation);
    y += getY(player->currentLocation);
    if (getCharFromPair(x, y, game->map) == '-' || getCharFromPair(x, y, game->map) == '+' || getCharFromPair(x, y, game->map) == '|' || isspace(getCharFromPair(x, y, game->map)))
    {
        return 0;
    }
    else if (getCharFromPair(x, y, game->map) == '.' || getCharFromPair(x, y, game->map) == '#')
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
    else if (getCharFromPair(x, y, game->map) == '*')
    {
        setY(y, player->currentLocation);
        setX(x, player->currentLocation);
        point_t *oldPoint = malloc(sizeof(point_t));
        oldPoint->x = oldX;
        oldPoint->y = oldY;
        setCharAtPoint(game->map, player->letter, player->currentLocation);
        setCharAtPoint(game->map, player->previousPoint, oldPoint);
        free(oldPoint);
        player->previousPoint = '.';
        return 2;
    }
    else if (isalpha(getCharFromPair(x, y, game->map)))
    {
        setY(y, player->currentLocation);
        setX(x, player->currentLocation);
        return 3;
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
 * @param key
 * @param playerSet
 * @param address
 */
static bool handleKey(char *key, void *playerSet, addr_t address)
{
    // get the player at the address
    player_t *player = getPlayer(address, playerSet);
    int oldX;
    int oldY;
    if (player != NULL && player->isInitalized != false)
    {
        oldX = getX(player->currentLocation);
        oldY = getY(player->currentLocation);
    }
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
            while ((moveResult = processMove(player, playerSet, -1, 0)) != 0)
            {
                if (moveResult == 2)
                {
                    if (goldHelper(player, playerSet))
                    {
                        return true;
                    }
                }
                updateDisplay(playerSet);
            }
        }
        // call the function process move for a lowercase key once
        else if (strcmp(key, "h") == 0)
        {
            moveResult = processMove(player, playerSet, -1, 0);
        }
        else if (strcmp(key, "L") == 0)
        {
            while ((moveResult = processMove(player, playerSet, 1, 0)) != 0)
            {
                if (moveResult == 2)
                {
                    if (goldHelper(player, playerSet))
                    {
                        return true;
                    }
                }
                updateDisplay(playerSet);
            }
        }
        else if (strcmp(key, "l") == 0)
        {
            moveResult = processMove(player, 1, 0);
        }
        else if (strcmp(key, "J") == 0)
        {
            while ((moveResult = processMove(player, 0, 1)) != 0)
            {
                if (moveResult == 2)
                {
                    if (goldHelper(player, playerSet))
                    {
                        return true;
                    }
                }
                updateDisplay(playerSet);
            }
        }
        else if (strcmp(key, "j") == 0)
        {
            moveResult = processMove(player, 0, 1);
        }
        else if (strcmp(key, "K") == 0)
        {
            while ((moveResult = processMove(player, 0, -1)) != 0)
            {
                if (moveResult == 2)
                {
                    if (goldHelper(player, playerSet))
                    {
                        return true;
                    }
                }
                updateDisplay(playerSet);
            }
        }
        else if (strcmp(key, "k") == 0)
        {
            moveResult = processMove(player, 0, -1);
        }
        else if (strcmp(key, "Y") == 0)
        {
            while ((moveResult = processMove(player, -1, -1)) != 0)
            {
                moveResult = processMove(player, -1, -1);
                if (moveResult == 2)
                {
                    if (goldHelper(player, playerSet))
                    {
                        return true;
                    }
                }
                updateDisplay(playerSet);
            }
        }
        else if (strcmp(key, "y") == 0)
        {
            moveResult = processMove(player, -1, -1);
        }
        else if (strcmp(key, "U") == 0)
        {
            while ((moveResult = processMove(player, 1, -1)) != 0)
            {
                moveResult = processMove(player, 1, -1);
                if (moveResult == 2)
                {
                    if (goldHelper(player, playerSet))
                    {
                        return true;
                    }
                }
                updateDisplay(playerSet);
            }
        }
        else if (strcmp(key, "u") == 0)
        {
            moveResult = processMove(player, 1, -1);
        }
        else if (strcmp(key, "b") == 0)
        {
            moveResult = processMove(player, -1, 1);
        }
        else if (strcmp(key, "B") == 0)
        {
            while ((moveResult = processMove(player, -1, 1)) != 0)
            {
                moveResult = processMove(player, -1, 1);
                if (moveResult == 2)
                {
                    if (goldHelper(player, playerSet))
                    {
                        return true;
                    }
                }
                updateDisplay(playerSet);
            }
        }
        else if (strcmp(key, "n") == 0)
        {
            moveResult = processMove(player, 1, 1);
        }
        else if (strcmp(key, "N") == 0)
        {
            while ((moveResult = processMove(player, 1, 1)) != 0)
            {
                moveResult = processMove(player, 1, 1);
                if (moveResult == 2)
                {
                    if (goldHelper(player, playerSet))
                    {
                        return true;
                    }
                }
                updateDisplay(playerSet);
            }
        }
        // if there was an invalid keystroke
        else
        {
            message_send(address, "ERROR Invalid Keystroke");
        }
        // the player picked up gold
        if (moveResult == 2)
        {
            if (goldHelper(player, playerSet))
            {
                return true;
            }
        }
        else if (moveResult == 3)
        { // runs into other player
            playerSwap(oldX, oldY, player, playerSet);
        }
    }
    updateDisplay(playerSet);
    return false;
}

/**
 * @brief calls gold and checks if no gold leftover
 *
 * @param player
 * @param playerSet
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
 * @param player
 */
void gold(player_t *player)
{
    // get the amount of gold in the pile that was hit
    int ncols = calculateColumns(game->map);
    int location = pointToLocation(player->currentLocation, ncols);

    int g = counters_get(game->goldMap, location);
    counters_set(game->goldMap, location, 0);
    player->playerGold += g;
    player->recentGold = g;
    game->GoldTotal -= g;
    printf("Goldtotal: %d\n", game->GoldTotal);

    char goldMessage[100] = "";
    strcat(goldMessage, "GOLD ");
    char gold[30];
    sprintf(gold, "%d %d %d", player->recentGold, player->playerGold, game->GoldTotal); // concatenate and add the most recent gold pickup with GOLD message
    strcat(goldMessage, gold);
    message_send(player->address, goldMessage);
    player->recentGold = 0;
}
/**
 * @brief handles process if a player walks into a space where another player is
 *
 * @param oldX
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
                printf("%d\n", player->recentGold);
                char goldMessage[100] = "";
                strcat(goldMessage, "GOLD ");
                char gold[30];
                sprintf(gold, "%d %d %d", player->recentGold, player->playerGold, game->GoldTotal); // concatenate and add the most recent gold pickup with GOLD message
                strcat(goldMessage, gold);
                message_send(player->address, goldMessage);
                char displayMessage[10000] = "";
                strcat(displayMessage, "DISPLAY\n");
                strcat(displayMessage, findVisibility(player, game->map)); // concatenate map text with the visible map
                // printf("DISPLAY MESSAGE\n");
                printf("%s\n", displayMessage);
                message_send(player->address, displayMessage);
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
    counters_delete(player->pointsSeen);
    free(player->currentLocation);
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
            strcat(table, buffer);
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
