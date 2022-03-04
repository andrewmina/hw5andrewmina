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
	keypad(input, true);

	

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
	int arrowPosition = 0;
	int lineCounter = 0;
	int cmdCounter = 0;
	//memset(line, 0, sizeof line);
	char history[100][100]; // 2d array of strings char strs[NUMBER_OF_STRINGS][STRING_LENGTH+1];
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


		if (inputChar == KEY_BACKSPACE) {
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

			for (int i = 0; i < strlen(line); i++) {  // record the command in the history
				history[cmdCounter][i] = line[i];  // records current line
			}

			for(int i = 0; i < strlen(history[arrowPosition]); i++) {
				history[cmdCounter][i] = history[arrowPosition][i];
			}
			
			//history[cmdCounter][strlen(history[cmdCounter]+1)] = ' ';
			//memset(history[cmdCounter], 0, sizeof history[cmdCounter]);
			for (int i = 0; i < strlen(line)+1; i++) {  // record the command in the history
				if(arrowPosition == cmdCounter){
					history[cmdCounter][i+strlen(history[arrowPosition])] = line[i];
				} else {
					history[cmdCounter][i+strlen(history[arrowPosition])] = line[i];
				}
				
				
			}
			// if (strlen(history[]) == 0) {
			// 	history[cmdCounter][strlen(history[cmdCounter]+1)] =history[cmdCounter][strlen(history[cmdCounter])];
			// }
			if (arrowPosition == cmdCounter) {
				//history[cmdCounter][strlen(history[cmdCounter]+1)] =history[cmdCounter][strlen(history[cmdCounter])];
				history[cmdCounter][strlen(history[arrowPosition]+1)] = ' ';
			}
			

			
			
			
			mvwprintw(input, input_y+1+cmdCounter, 2, "%d: %s",cmdCounter, history[cmdCounter]);

			// if (arrowPosition == 0){
				// for (int i = 0; i < strlen(line); i++) {  // record the command in the history
				// 	history[cmdCounter][i] = line[i];
				// }
			// }else {

				// for(int i = 0; i < strlen(history[arrowPosition]); i++) {
				// 	history[cmdCounter][i] = history[arrowPosition][i];
				// }
				// for (int i = 0; i < strlen(line); i++) {  // record the command in the history
				// 	history[cmdCounter][i+strlen(history[arrowPosition])] = line[i];
					
				// }
				// memset(line, 0, sizeof line);
				// for (int i = 0; i < strlen(history[cmdCounter]); i++) {
				// 	line[i] = history[cmdCounter][i];
				// }
			// }

			//mvwgetnstr(input, 1, 6, history[cmdCounter], strlen(line));
			
			
			// for (int i = 6; i < 64; i++) {
			// 	mvwprintw(input, 1, i, "%c",' ');
			// }
			///
			wmove(input, 1, 6); //clearing
			wclrtoeol(input);
			box(input,0,0);
			refresh();
			for (int i = 0; i < cmdCounter; i++) { // debolding
				mvwprintw(input, input_y+1+i, 2, "%d: %s",i, history[i]);
			}



			///

			
			//line[lineCounter] = ' '; // add new line charecter
			dsh_run(history[cmdCounter]);
			memset(line, 0, sizeof line);
			lineCounter = 0;

			// move cursor to 
			//input_y++;
			input_x = 1;
			
			//mvwprintw(input,input_y+1+cmdCounter, input_x, "%s", line);
			cmdCounter++;
			arrowPosition = cmdCounter;
		} else if (inputChar == KEY_UP) {
			wmove(input, input_y, 6);
			
			wclrtoeol(input);//clearing
			box(input,0,0);
			refresh();
			for (int i = 0; i < cmdCounter; i++) { // debolding
				mvwprintw(input, input_y+1+i, 2, "%d: %s",i, history[i]);
			}
			
			
			// operation for bolding current command we are on
			if(arrowPosition > 0) {
				arrowPosition--;
			}
			if(arrowPosition != cmdCounter-1){
				mvwprintw(input, input_y+1+arrowPosition+1, 2, "%d: %s",arrowPosition+1, history[arrowPosition+1]);
			}
			wattron(input, A_BOLD); // bolding
			mvwprintw(input, input_y+1+arrowPosition, 2, "%d: %s",arrowPosition, history[arrowPosition]);
			wattroff(input, A_BOLD);
			
			
			// wrefresh(input);
			// //mvwprintw(input,input_y,input_x + 5,"%s",history[arrowPosition]);
			// for (int i = 0;i < strlen(history[arrowPosition]); i++){
			// 	mvwaddch(input,input_y,input_x + 5+i,history[arrowPosition][i]);
			// }
			// wmove(input, 1, input_x + 5 +strlen(history[arrowPosition]));

			

			wmove(input, input_y, 6);
			input_x=strlen(history[arrowPosition])+1;
			waddstr(input, history[arrowPosition]);
			memset(line, 0, sizeof line);
			for (int i = 0; i < strlen(history[arrowPosition]); i++){
				line[i] = history[arrowPosition][i];
			}
			wrefresh(input);
			
			
			

			


		} else if (inputChar == KEY_DOWN) {
			wmove(input, input_y, 6);
			
			wclrtoeol(input);
			box(input,0,0);
			refresh();

			for (int i = 0; i < cmdCounter; i++) {  // debolding
				mvwprintw(input, input_y+1+i, 2, "%d: %s",i, history[i]);
			}
			//wrefresh(input);
			
			if(arrowPosition < cmdCounter-1){
				arrowPosition++;
			}

			// Operation for bolding the correct command we are on
			if(arrowPosition != cmdCounter){
				mvwprintw(input, input_y+1+arrowPosition-1, 2, "%d: %s",arrowPosition-1, history[arrowPosition-1]);
			}
			if (arrowPosition != 0){
				wattron(input, A_BOLD);
				mvwprintw(input, input_y+1+arrowPosition, 2, "%d: %s",arrowPosition, history[arrowPosition]);
				wattroff(input, A_BOLD);
			}

			wmove(input, input_y, 6);
			waddstr(input, history[arrowPosition]);
			input_x=strlen(history[arrowPosition])+1;
			memset(line, 0, sizeof line);
			for (int i = 0; i < strlen(history[arrowPosition]); i++){
				line[i] = history[arrowPosition][i];
			}
			
			wrefresh(input);

			
			// wrefresh(input);
			// for (int i = 0;i < strlen(history[arrowPosition]); i++){
			// 	mvwaddch(input,input_y,input_x + 5 +i,history[arrowPosition][i]);
			// }

			// wmove(input, 1, input_x + 5 +strlen(history[arrowPosition]));
			
			
			
			
			

			
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