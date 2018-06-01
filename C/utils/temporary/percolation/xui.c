#include "xui.h"
#include <stdio.h>
#include <stdlib.h>

#define MARIN 20

static Display *Dsp;
static Window Win;
static int Scr;
static GC Blockgc, Fullgc, Emptygc;
static GC Linegc;

static int Rectw;
static int Border;
static int Num;
static int X0, Y0;
static int IsAuto = 0;

static void fatal(char *msg)
{
	printf("xui: %s", msg);
	exit(1);
}

static void init_draw(int n)
{
	int i, j;

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			XuiDrawRect(i, j, TYPE_BLOCK);
}

int XuiInit(int n, int rectw, int border, int is_auto)
{
	XGCValues gcv;
	Colormap cm;
	XColor c, ce;
	int winw = n * rectw + 2 * MARIN;

	Rectw = rectw;
	Border = border;
	Num = n;
	X0 = MARIN;
	Y0 = MARIN;
	IsAuto = is_auto;

	Dsp = XOpenDisplay(NULL);
	if (Dsp == NULL)
		fatal("open display failed\n");

	Scr = DefaultScreen(Dsp);
	Win = XCreateSimpleWindow(Dsp, RootWindow(Dsp, Scr),
			10, 10, winw, winw, 1, 
			BlackPixel(Dsp, Scr), WhitePixel(Dsp, Scr));

	cm = DefaultColormap(Dsp, Scr);

	XAllocNamedColor(Dsp, cm, BLOCK_COLOR, &c, &ce);
	gcv.foreground = c.pixel;
	gcv.background = WhitePixel(Dsp, Scr);
	Blockgc = XCreateGC(Dsp, Win, GCForeground | GCBackground, &gcv);

	XAllocNamedColor(Dsp, cm, FULL_COLOR, &c, &ce);
	gcv.foreground = c.pixel;
	Fullgc = XCreateGC(Dsp, Win, GCForeground | GCBackground, &gcv);

	XAllocNamedColor(Dsp, cm, EMPTY_COLOR, &c, &ce);
	gcv.foreground = c.pixel;
	Emptygc = XCreateGC(Dsp, Win, GCForeground | GCBackground, &gcv);

	gcv.foreground = BlackPixel(Dsp, Scr);
	Linegc = XCreateGC(Dsp, Win, GCForeground | GCBackground, &gcv);

	XSelectInput(Dsp, Win, ExposureMask | KeyPressMask | ButtonPressMask);
	XMapWindow(Dsp, Win);

	return 0;
}

int XuiMainLoop(CALLBACK_FUNC *cb, void *data)
{
	XEvent e;
	int ok = 0;

	while (1) {
		XNextEvent(Dsp, &e);
		if (e.type == Expose) {
			init_draw(Num);
		} else if (e.type ==  KeyPress) {
			break;
		} else if (ok == 0 && e.type == ButtonPress) {
			if (IsAuto) {
				while (ok == 0)
					ok = cb(data);
			} else {
				ok = cb(data);
			}
		}
	}

	return 0;
}

void XuiClose()
{
	XCloseDisplay(Dsp);
}

void XuiDrawRect(int x, int y, enum rect_type_t type)
{
	GC gc;
	int x0, y0;

	switch (type) {
		case TYPE_BLOCK:
			gc = Blockgc;
			break;
		case TYPE_FULL:
			gc = Fullgc;
			break;
		case TYPE_EMPTY:
			gc = Emptygc;
			break;
		default:
			fatal("unknown rect type\n");
			break;
	}

	x0 = X0 + Rectw * x;
	y0 = Y0 + Rectw * y;
	XDrawRectangle(Dsp, Win, Linegc, x0, y0, Rectw, Rectw);
#ifdef VIEW3D
	XFillRectangle(Dsp, Win, gc, x0 + Border, y0 + Border,
			Rectw - 2 * Border, Rectw - 2 * Border);
#else
	XFillRectangle(Dsp, Win, gc, x0 + Border, y0 + Border,
			Rectw - Border, Rectw - Border);
#endif
	XFlush(Dsp);
}


