# CS50
## Nuggets Game

This repository contains the code for the CS50 "Nuggets" game, in which players explore a set of rooms and passageways in search of gold nuggets.
The rooms and passages are defined by a **map** loaded by the server at the start of the game.
The gold nuggets are randomly distributed in **piles** within the rooms.
Up to 26 players, and one spectator, may play a given game.
Each player is randomly dropped into a room when joining the game.
Players move about, collecting nuggets when they move onto a pile.
When all gold nuggets are collected, the game ends and a summary is printed.

### ASSUMPTIONS

#### Assumes that a name with spaces is surrounded by quotation marks and that the name does not contain quotation marks
#### Assumes that you do not crtl+c out and instead you must press 'Q' not 'q'
#### Assumes that after you resize client screen once it stays at a valid size
