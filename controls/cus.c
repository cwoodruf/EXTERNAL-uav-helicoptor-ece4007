
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>                 
#include <curses.h>


void init()
	{
	init_pair(1,  COLOR_RED,     COLOR_BLACK);
	init_pair(2,  COLOR_GREEN,   COLOR_BLACK);
	init_pair(3,  COLOR_YELLOW,  COLOR_BLACK);
	init_pair(4,  COLOR_BLUE,    COLOR_BLACK);
	init_pair(5,  COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6,  COLOR_CYAN,    COLOR_BLACK);
	init_pair(7,  COLOR_BLUE,    COLOR_WHITE);
	init_pair(8,  COLOR_WHITE,   COLOR_RED);
	init_pair(9,  COLOR_BLACK,   COLOR_GREEN);
	init_pair(10, COLOR_BLUE,    COLOR_YELLOW);
	init_pair(11, COLOR_WHITE,   COLOR_BLUE);
	init_pair(12, COLOR_WHITE,   COLOR_MAGENTA);
	init_pair(13, COLOR_BLACK,   COLOR_CYAN);
	}

int main(void) {

    WINDOW * mainwin;
    if ( (mainwin = initscr()) == NULL ) {
	fprintf(stderr, "Error initialising ncurses.\n");
	exit(EXIT_FAILURE);
    }

    start_color();                    /*  Initialize colours  */


    /*  Print message  */

    mvaddstr(0, 0, " Hello, world! ");

    if ( has_colors() && COLOR_PAIRS >= 13 ) {

	int n = 1;

   init();

	while ( n <= 50 ) {
	    color_set(n%13, NULL);
	    refresh();
    	usleep(500000);
	    mvaddstr(n%18, 0, " Hello, world! ");
	    n++;
	}
    }


    /*  Refresh the screen and sleep for a
	while to get the full screen effect  */

    

    /*  Clean up after ourselves  */
    refresh();
    	sleep(1);
    delwin(mainwin);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}


