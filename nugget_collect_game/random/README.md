# CS50 Final Project
## Winter 2023

### random

The `random` directory contains `random.c` which contains functions for creating random behavior throughout the game. The `random` function intitializes the gold piles in random amounts throughout the game map. And the `spawnlocation` function picks a random location on the map to spawn a player. The directory also contains a `testrandom.c` file which runs tests on the random functions to ensure they work. The functions can be seeded with an optional seed, if not the function will seed with the process id.

### Usage

`random.c` uses the following functions:

```c
void random(game_t *game, counters_t *goldMap, int seed);
static void updateMap(void *arg, const int key, const int count);
int spawnLocation(game_t *game);
```

### Implementation

`random` uses `rand()` and `srand` to create random behaviour throughout the game in terms of gold placement and gold distribution. We store gold location and amount in `counters_t` objects which link the location to the amount of gold. when gold is picked up we set the location to count 0.

### Testing


 `testrandom` runs the various functions of random and shows both their random behaviour and their seeded behvaiour. 
 To test enter into the random directory and run `make test`