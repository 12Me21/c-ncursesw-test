#include <locale.h>
#define NCURSES_WIDECHAR 1
#include <ncursesw/ncurses.h>

void Term_init(void) {
	setlocale(LC_CTYPE, "");
	initscr();
}

void Term_free(void) {
	endwin();
}

int main(void) {
	WINDOW* p = newpad(100,100);
	mvwaddwstr(p, 0,0, L"test🖤");
	
	prefresh(p, 0,0, 3,3, 9+3,9+3);
	wgetch(p);
}

//	cchar_t wc;
//setcchar(&wc, L"🖤", 1, 0x0800, NULL); //instead of null we can pass an int ptr which is an extended color pair
//add_wch(&wc);
