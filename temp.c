/* Basic heursitic function. */

/* There are several states that lead up to a win. */
/* Let's only care about combinations greater than 3 across a 5 tile window */

/* XXX-- */ /* -XXX- */ /* --XXX */ /* XX-X- */ /* X-X-X */ /* -X-XX */ /* XX--X */ /* X--XX */
/* Straight 3, capped 3, even 3, double capped 3 */

/*XXXX-*/ /*-XXXX*/ /*XX-XX*/ /*XXX-X*/ /* X-XXX */ 
/* Straight 4, split 4, and capped 4 */

/* XXXXX */ 
/* Either win or lose */

/* We should have a list of strings for each of these. */
char* straight3[3] = {"TTTEE", "ETTTE", "EETTT"};
char* cap3[4] = {"TEETT", "TTEET", "TTETE", "ETETT"};
char* even3 = {"TETET"}; 
char* straight4[2] = {"TTTTE", "TTTTE"};
char* split4 = "TTETT";
char* capped4[2] = {"TTTET", "TETTT"};
char* win = "TTTTT";

/* Now we've got to look for these patters in the board. */

/* First for our opponent */
/* So based on the move file, zoom in on their last move. */
/* Search an area of 4 around that move to find any of the patterns listed above */

/* So parse a 9x9 w/ the last move as the center */

/* When a tile of the opponent's color is found start the heuristic value function */
/* Look in a 5 length window in each direction of the tile, (can limit this to E, SE, S, SW) */
/* Can add another limit to not bother searching directions that would go out of the 9x9 */

/* As you parse and discover patterns, assign (+=) values to the empty tiles within them */
/* corresponding to their values in the heuristic. High = better for the player. */

/* Once the whole region has been parsed through, look through the map and grab the highest value */
/* move for the opponent. */

/* Repeat for ourselves */

/* Compare the two options, Move to the most valuable tile for either player. If there's a tie always choose our own. */