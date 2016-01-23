#include <ncurses.h>
#include <string.h>

/**
 *
 */


/** @brief init_ncurses()
 *  @details 
 *  Runs ncurses-specific intializations to enable input and output
 */
void init_ncurses()
{
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
}

/** @brief update_screen()
 * @details
 * Runs anything necessary to update the screen
 */
void update_screen()
{
    refresh();    
}

int main(int arc, char* argv[])
{
    init_ncurses();

    return 0;
}
