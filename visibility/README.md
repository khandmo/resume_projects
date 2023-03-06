# CS50 Final Project
## Winter 2023

### visibility

The `visibility` directory contains `visibility.c` which contains functions for determining
which points in a given map are visible given a player located in the map. The directory
also contains a `testvisibility.c` file which runs tests on the visiblity function 

### Usage

`visibility.c` uses the following functions:

```c
char* findVisibility(player_t* player, char* map);
bool isVisible1(point_t* start, point_t* end, char* map);
bool isVisible2(point_t* start, point_t* end, char* map);
float line_func(float slope, int x, int y);
point_t* point_new(int x, int y);
```

### Implementation

`visibility` supplies functions utilized by server to create a map unique to each player
representing what the map looks like based on the player's position in the map and what
the player has seen in the past.

### Testing

Use `testvisibility` to test visibility. In the visibility directory, run `make test`

