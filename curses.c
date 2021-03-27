#include <locale.h>
#define NCURSES_WIDECHAR 1
#include <ncursesw/ncurses.h>

int main(void) {
	setlocale(LC_CTYPE, "");
	WINDOW* w = initscr();
	WINDOW* p = newpad(100,100);
	mvwaddwstr(p, 0,0, L"test🖤");
	
   prefresh(p, 0,0,3,3,9+3,9+3);
	wgetch(p);
	
	endwin();
	return 0;
}

//	cchar_t wc;
//setcchar(&wc, L"🖤", 1, 0x0800, NULL); //instead of null we can pass an int ptr which is an extended color pair
//add_wch(&wc);
