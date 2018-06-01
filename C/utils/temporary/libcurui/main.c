#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ncursesw/ncurses.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <locale.h>
#include <semaphore.h>

#define MAX_BLOCK_LEN 8

#define DEF_TXT_COLOR 1
#define ATTACH_COLOR 2

WINDOW *pWin = NULL;

struct edv_block {
	WINDOW *pwin;
	int x, y;
	int w, h;
	char lable[MAX_BLOCK_LEN];
	char val[MAX_BLOCK_LEN];
};

static void draw_block(struct edv_block *pb, int color)
{
	int i = 0;
	WINDOW *pwin = pb->pwin;

	wattron(pwin, COLOR_PAIR(color));
	mvwaddch(pwin, pb->y, pb->x, '+');
	whline(pwin, '-', pb->w - 2);
	mvwaddch(pwin, pb->y, pb->x + pb->w - 1, '+');

	for (i = 1; i < (pb->h - 1); i++) {
		mvwaddch(pwin, pb->y + i, pb->x, '|');
		mvwaddch(pwin, pb->y + i, pb->x + pb->w - 1, '|');
	}

	mvwaddch(pwin, pb->y + i, pb->x, '+');
	whline(pwin, '-', pb->w - 2);
	mvwaddch(pwin, pb->y + i, pb->x + pb->w - 1, '+');
	wattroff(pwin, COLOR_PAIR(color));
	wrefresh(pwin);
}

static void draw_block_lable(struct edv_block *pb, int color)
{
	WINDOW *pwin = pb->pwin;

	wattron(pwin, COLOR_PAIR(color));
	mvwaddnstr(pwin, pb->y + 1, pb->x + 2, pb->lable, 8);
	wattroff(pwin, COLOR_PAIR(color));
	wrefresh(pwin);
}

static void draw_block_val(struct edv_block *pb, int color)
{
	WINDOW *pwin = pb->pwin;

	wattron(pwin, COLOR_PAIR(color));
	mvwaddnstr(pwin, pb->y + 2, pb->x + 2, pb->val, 8);
	wattroff(pwin, COLOR_PAIR(color));
	wrefresh(pwin);
}

static void clear_block_lable(struct edv_block *pb)
{
	WINDOW *pwin = pb->pwin;

	wattron(pwin, COLOR_PAIR(DEF_TXT_COLOR));
	mvwhline(pwin, pb->y + 1, pb->x + 2, ' ', pb->w - 4);
	wattroff(pwin, COLOR_PAIR(DEF_TXT_COLOR));
	wrefresh(pwin);
}

static void clear_block_val(struct edv_block *pb)
{
	WINDOW *pwin = pb->pwin;

	wattron(pwin, COLOR_PAIR(DEF_TXT_COLOR));
	mvwhline(pwin, pb->y + 2, pb->x + 2, ' ', pb->w - 4);
	wattroff(pwin, COLOR_PAIR(DEF_TXT_COLOR));
	wrefresh(pwin);
}

static void clear_block_content(struct edv_block *pb)
{
	WINDOW *pwin = pb->pwin;

	int i;

	for (i = 1; i < pb->h - 1; i++)
		mvwhline(pwin, pb->y + i, pb->x + 2, ' ', pb->w - 4);
	wrefresh(pwin);
}

static void init_term()
{
	initscr();
	noecho();
	curs_set(0);
	start_color();
	init_pair(DEF_TXT_COLOR, COLOR_WHITE, COLOR_BLACK);
	init_pair(ATTACH_COLOR, COLOR_GREEN, COLOR_BLACK);
}

int main(int argc, char **argv)
{
	char c = 0;
	int maxcol, maxrow;
	struct edv_block b;

	init_term();

	getmaxyx(stdscr, maxrow, maxcol);
	if( (pWin = newwin(maxrow, maxcol, 0, 0)) == NULL) {
		perror("newwin: ");
		return 1;
	}
	refresh();

	b.x = 0;
	b.y = 0;
	b.w = 12;
	b.h = 4;
	b.pwin = pWin;
	strcpy(b.lable, "vps");
	strcpy(b.val, "12345678");

	draw_block(&b, ATTACH_COLOR);
	draw_block_lable(&b, DEF_TXT_COLOR);
	draw_block_val(&b, ATTACH_COLOR);

	//clear_block_content(&b);
	clear_block_lable(&b);
	//clear_block_val(&b);

	while(1){
		c = getch();
	}

	return 0;
}
