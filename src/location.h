/*------------------------------------------------------------------------------
| NuCTex		| location.h
| Author	| Benjamin E Allen
| Date		| 25 November, 2015
|-------------------------------------------------------------------------------
| Overview	| Define structs and functions used in the location code
\-----------------------------------------------------------------------------*/

/* this is the main location structure.
one location represents one map tile */
typedef struct Location {
	char* description;		// A string description of the location

	/* these are the location pointers. These will point to other locations that
	this location connects to. Set to a location that has been delclared, and
	make sure that it makes sense. To create a "wall," set the pointer to 0
	in the initialization (location.c) */
	struct Location* n;
	struct Location* s;
	struct Location* e;
	struct Location* w;
} Location;

// current game locations
Location nw;
Location ne;
Location sw;
Location se;


/* this pointer handles the player's current position. It changes often by a
number of different commands. This is first initialized in the location.c
file. Where you initialize it is where the player will start. */
Location *pos;