#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>

#include <ncurses.h>

#define WIN_HEIGHT 40
#define WIN_WIDTH 65
#define WIN1_START_X 10
#define WIN2_START_X (WIN1_START_X + WIN_WIDTH + 20)
#define WIN_START_Y 10

#include"dsh.h"
int main(int argc, char** argv) {
    //char *line=0;
    size_t size=0;
	dsh_init();
	initscr();

	

	

    
    printf("dsh> ");


	/* Make sure that Ctrl+C will exit the program */
	cbreak();

	/* Disable default key press echo */
	noecho();

	

	/* Create input window on the left with heading */
	mvwprintw(stdscr, WIN_START_Y - 2, WIN1_START_X, "%s", "Typing Window");
	WINDOW *input = newwin(WIN_HEIGHT, WIN_WIDTH, WIN_START_Y, WIN1_START_X);

	

	/* TODO 1a: Create output window on the right with heading. Use mvwprintw for heading, & newwin for new window */

	mvwprintw(stdscr, WIN_START_Y - 2, WIN2_START_X, "%s", "Status Window");
	WINDOW *output = newwin(WIN_HEIGHT, WIN_WIDTH, WIN_START_Y, WIN2_START_X);

	refresh();

	/* Create box around input window */
	box(input,0,0);
	wrefresh(input);

	box(output,0,0);
	wrefresh(output);

	/* TODO 1b: Create box around output window */

	int inputChar = 0;
	int prevChar = -1;
	int words = 0;

	/* Following co-ordinates are relative to window */
	int input_y  = 1;
	int input_x  = 1;

	char line[100];
	int lineCounter = 0;
	int cmdCounter = 0;
	//memset(line, 0, sizeof line);
	mvwprintw(input, input_y, input_x, "%s", "dsh> " );
	while(1) {
		/* Go to next line if width is exhausted */
		if (input_x == WIN_WIDTH - 1){
			input_x = 1;
			input_y++;
		}

		wmove(input, input_y, input_x+5);
		wrefresh(input);

		/* TODO 2a: Use wgetch to take single character input */
		inputChar = wgetch(input);


		if (inputChar == 127) {
			//inputChar = '\0';
			mvwprintw(input,input_y,input_x-1+5,"%c", ' ');
			
			input_x--;
			if (input_x == 0) {
				input_x++;
			}
			
			lineCounter--;
			if (lineCounter == -1) {
				lineCounter++;
			}
			line[lineCounter] = '\0';
			wmove(input, input_y, input_x+5);			
			refresh();

		} else if (inputChar == '\n') {
			//printf("the line is: %s", line);
			//lineCounter++;
			//char* temp = line;
			mvwprintw(input, input_y+1+cmdCounter, 2, "%d: %s",cmdCounter, line);
			line[lineCounter] = ' ';

			
			
			dsh_run(line);
			memset(line, 0, sizeof line);
			lineCounter = 0;

			// move cursor to 
			//input_y++;
			input_x = 1;
			
			//mvwprintw(input,input_y+1+cmdCounter, input_x, "%s", line);
			cmdCounter++;
		} else {
			prevChar = inputChar;
			line[lineCounter] = inputChar;
			lineCounter++;
			
			mvwprintw(input,input_y,input_x + 5,"%c",inputChar);
			input_x++;

		}

		
		

		




		
	}



	

//     while(getline(&line,&size,stdin) > 0) {

		

// 		refresh();
		
// 		if (input_x == WIN_WIDTH - 1){
// 			input_x = 1;
// 			input_y++;
// 		}

// 		wmove(input, input_y, input_x);
// 		wrefresh(input);

// 		/* TODO 2a: Use wgetch to take single character input */
// 		inputChar = wgetch(input);
		

// 		prevChar = inputChar;

// 		mvwprintw(input,input_y,input_x,"%c",inputChar);
// 		input_x++;
		
//         //dsh_run(line);

//         //printf("dsh> ");
//    }
   endwin();
}