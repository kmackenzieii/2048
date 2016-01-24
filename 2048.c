#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 *
 */

#define BOARD_SIZE 4

int state[BOARD_SIZE*BOARD_SIZE];
int score = 0;

/** @brief init_ncurses()
 *  @details 
 *  Runs ncurses-specific intializations to enable input and output
 */
void init_ncurses()
{
    initscr();
    raw();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    if (has_colors())
    {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);
        init_pair(2, COLOR_BLUE, COLOR_BLACK);
        init_pair(3, COLOR_GREEN, COLOR_BLACK);
        init_pair(4, COLOR_YELLOW, COLOR_BLACK);
        init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(6, COLOR_RED, COLOR_BLACK);
        init_pair(7, COLOR_WHITE, COLOR_BLACK);
    }
}


void board(WINDOW *win, int starty, int startx, int lines, int cols, 
	   int tile_width, int tile_height)
{	int endy, endx, i, j;
	
	endy = starty + lines * tile_height;
	endx = startx + cols  * tile_width;
	
	for(j = starty; j <= endy; j += tile_height)
		for(i = startx; i <= endx; ++i)
			mvwaddch(win, j, i, ACS_HLINE);
	for(i = startx; i <= endx; i += tile_width)
		for(j = starty; j <= endy; ++j)
			mvwaddch(win, j, i, ACS_VLINE);
	mvwaddch(win, starty, startx, ACS_ULCORNER);
	mvwaddch(win, endy, startx, ACS_LLCORNER);
	mvwaddch(win, starty, endx, ACS_URCORNER);
	mvwaddch(win, 	endy, endx, ACS_LRCORNER);
	for(j = starty + tile_height; j <= endy - tile_height; j += tile_height)
	{	mvwaddch(win, j, startx, ACS_LTEE);
		mvwaddch(win, j, endx, ACS_RTEE);	
		for(i = startx + tile_width; i <= endx - tile_width; i += tile_width)
			mvwaddch(win, j, i, ACS_PLUS);
	}
	for(i = startx + tile_width; i <= endx - tile_width; i += tile_width)
	{	mvwaddch(win, starty, i, ACS_TTEE);
		mvwaddch(win, endy, i, ACS_BTEE);
	}
	wrefresh(win);
}

int digits(int n)
{
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    if (n < 10000000) return 7;
    if (n < 100000000) return 8;
    if (n < 1000000000) return 9;
    return 10;
}

/** @brief update_screen()
 * @details
 * Runs anything necessary to update the screen
 */
void update_screen()
{
    int height          = BOARD_SIZE;
    int width           = BOARD_SIZE;
    int tile_height     = 4;
    int tile_width      = 8;
    int startx          = COLS/2 - (width*tile_width)/2;
    int starty          = LINES/2 - (height*tile_height/2);
    erase();
    board(stdscr, starty, startx, height, width, tile_width, tile_height);

    for (int j = 0; j<BOARD_SIZE; j++)
    {
        for (int i = 0; i<BOARD_SIZE; i++)
        {
            int tmp = state[i+(j*BOARD_SIZE)];
            if (tmp > 0)
            {
                if (has_colors())
                {
                    if (tmp <= 2) attron(COLOR_PAIR(1));
                    else if (tmp <= 4) attron(COLOR_PAIR(2));
                    else if (tmp <= 8) attron(COLOR_PAIR(3));
                    else if (tmp <= 16) attron(COLOR_PAIR(4));
                    else if (tmp <= 64) attron(COLOR_PAIR(5));
                    else if (tmp <= 1024) attron(COLOR_PAIR(6));
                    else if (tmp <= 2048) attron(COLOR_PAIR(7));
                }
                mvprintw(starty+j*tile_height+tile_height/2+tile_height%2,
                         startx+i*tile_width+tile_width/2+tile_width%2-digits(tmp)+1,
                         "%d",
                         tmp);
                if (has_colors())
                {
                    if (tmp <= 2) attroff(COLOR_PAIR(1));
                    else if (tmp <= 4) attroff(COLOR_PAIR(2));
                    else if (tmp <= 8) attroff(COLOR_PAIR(3));
                    else if (tmp <= 16) attroff(COLOR_PAIR(4));
                    else if (tmp <= 64) attroff(COLOR_PAIR(5));
                    else if (tmp <= 1024) attroff(COLOR_PAIR(6));
                    else if (tmp <= 2048) attroff(COLOR_PAIR(7));
                }
            }
        }
    }

    mvprintw(starty-1, startx, "Score: %d", score);
    refresh();
}

void new_num()
{
    int pos;
    int n = 0;
    do
    {
        pos = rand() % (BOARD_SIZE * BOARD_SIZE);
    }
    while(state[pos] != 0 && n++ < BOARD_SIZE * BOARD_SIZE);
    if (n < BOARD_SIZE * BOARD_SIZE)
    {
        state[pos] = ((rand() % 10) < 9) ? 2 : 4;
    }
}

void init_state()
{
    for (int j = 0; j<BOARD_SIZE; j++)
    {
        for (int i = 0; i<BOARD_SIZE; i++)
        {
            state[i+(j*BOARD_SIZE)] = 0;
        }
    }

    //Make two new numbers
    new_num();
    new_num();
}

void init_rand()
{
    time_t t;
    srand((unsigned) time(&t));
}

void move_left()
{
    for (int j=0; j<BOARD_SIZE; j++)
    {
        for (int i=0; i<BOARD_SIZE; i++)
        {
            int n = i;
            while (n > 0 && n < BOARD_SIZE)
            {
                if (state[n+(j*BOARD_SIZE)] != 0 
                        && state[n-1+(j*BOARD_SIZE)] == 0)
                {
                    state[n-1+(j*BOARD_SIZE)] = state[n+(j*BOARD_SIZE)];
                    state[n+(j*BOARD_SIZE)] = 0;
                }
                n--;
            }
        }
    }
}

void move_right()
{
    for (int j=0; j<BOARD_SIZE; j++)
    {
        for (int i=BOARD_SIZE-1; i>=0; i--)
        {
            int n = i;
            while (n >= 0 && n < BOARD_SIZE-1)
            {
                if (state[n+(j*BOARD_SIZE)] != 0 
                        && state[n+1+(j*BOARD_SIZE)] == 0)
                {
                    state[n+1+(j*BOARD_SIZE)] = state[n+(j*BOARD_SIZE)];
                    state[n+(j*BOARD_SIZE)] = 0;
                }
                n++;
            }
        }
    }
}

void move_up()
{
    for (int i=0; i<BOARD_SIZE; i++)
    {
        for (int j=0; j<BOARD_SIZE; j++)
        {
            int n = j;
            while (n > 0 && n < BOARD_SIZE)
            {
                if (state[i+(n*BOARD_SIZE)] != 0 
                        && state[i+((n-1)*BOARD_SIZE)] == 0)
                {
                    state[i+((n-1)*BOARD_SIZE)] = state[i+(n*BOARD_SIZE)];
                    state[i+(n*BOARD_SIZE)] = 0;
                }
                n--;
            }
        }
    }
}

void move_down()
{
    for (int i=0; i<BOARD_SIZE; i++)
    {
        for (int j=BOARD_SIZE-1; j>=0; j--)
        {
            int n = j;
            while (n >= 0 && n < BOARD_SIZE-1)
            {
                if (state[i+(n*BOARD_SIZE)] != 0 
                        && state[i+((n+1)*BOARD_SIZE)] == 0)
                {
                    state[i+((n+1)*BOARD_SIZE)] = state[i+(n*BOARD_SIZE)];
                    state[i+(n*BOARD_SIZE)] = 0;
                }
                n++;
            }
        }
    }
}

void combine_horizontal()
{
    for (int j=0; j<BOARD_SIZE; j++)
    {
        for (int i=0; i<BOARD_SIZE-1; i++)
        {
            if (state[i+(j*BOARD_SIZE)] == state[(i+1)+((j)*BOARD_SIZE)])
            {
                state[i+(j*BOARD_SIZE)] = state[(i+1)+((j)*BOARD_SIZE)]<<1;
                state[(i+1)+((j)*BOARD_SIZE)] = 0;
                score += state[i+(j*BOARD_SIZE)];
                if (i == BOARD_SIZE-2)
                {
                    state[(i+1)+((j)*BOARD_SIZE)] = 0;
                }
            }
        }
    }
}

void combine_vertical()
{
    for (int i=0; i<BOARD_SIZE; i++)
    {
        for (int j=0; j<BOARD_SIZE-1; j++)
        {
            if (state[i+(j*BOARD_SIZE)] == state[i+((j+1)*BOARD_SIZE)])
            {
                state[i+(j*BOARD_SIZE)] = state[i+((j+1)*BOARD_SIZE)]<<1;
                state[i+((j+1)*BOARD_SIZE)] = 0;
                score += state[i+(j*BOARD_SIZE)];
                if (j == BOARD_SIZE-2)
                {
                    state[i+((j+1)*BOARD_SIZE)] = 0;
                }
            }
        }
    }
}

void play(int ch)
{
    switch (ch)
    {
        case KEY_LEFT:
            move_left();
            combine_horizontal();
            move_left();
            break;
        case KEY_RIGHT:
            move_right();
            combine_horizontal();
            move_right();
            break;
        case KEY_UP:
            move_up();
            combine_vertical();
            move_up();
            break;
        case KEY_DOWN:
            move_down();
            combine_vertical();
            move_down();
            break;
        default:
            break;
    }
}

int main(int arc, char* argv[])
{
    init_ncurses();
    init_rand();
    init_state();
    update_screen();

    int ch;
    while ((ch = getch()) != 'q')
    {
        play(ch);
        new_num();
        update_screen();
    }

    endwin();

    return 0;
}

