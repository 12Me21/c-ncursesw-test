#include <locale.h>
#define NCURSES_WIDECHAR 1
#include <ncursesw/ncurses.h>

int main(void) {
	setlocale(LC_CTYPE, "");
	WINDOW* w = initscr();
	addwstr(L"test🖤");
	refresh();
	
	getch();
	endwin();
	return 0;
}

//	cchar_t wc;
//setcchar(&wc, L"🖤", 1, 0x0800, NULL); //instead of null we can pass an int ptr which is an extended color pair
//add_wch(&wc);
