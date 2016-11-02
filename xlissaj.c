/*
 * Copyright (c) 2001, 2016 Yohei Endo <yoheie@gmail.com>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must
 *    not claim that you wrote the original software. If you use this
 *    software in a product, an acknowledgment in the product
 *    documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must
 *    not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 *    distribution.
 */
#include<stdio.h>
#include<math.h>
#include<X11/Xlib.h>
#include<X11/Xutil.h>

#define PI 3.14159265358979323846

Display *XlsjGDsp;
Window XlsjGRoot, XlsjGWindow, XlsjGCanvas, XlsjGBtn[2],
       XlsjGX, XlsjGXplus, XlsjGXminus, XlsjGY, XlsjGYplus, XlsjGYminus,
       XlsjGDelay, XlsjGDelayplus, XlsjGDelayminus;
GC XlsjGGC;
XEvent XlsjGEvent;
Atom XlsjGAtom1, XlsjGAtom2;
Font XlsjGFont;
XSetWindowAttributes XlsjGAttr;
XSizeHints XlsjGSizeHints;
int nx, ny, delay;
unsigned long black, white;
char str[3];

void xlsj_init(void);
void xlsj_place(void);
void xlsj_draw_static(void);
void xlsj_draw(void);

int main(int argc, char *argv[])
{
	xlsj_init();
	nx = 1; ny = 1; delay = 0;
	xlsj_place();

	XMapWindow(XlsjGDsp, XlsjGWindow);
	XMapSubwindows(XlsjGDsp, XlsjGWindow);

	xlsj_draw_static();

	while (1) {
		if (XCheckTypedEvent(XlsjGDsp, ClientMessage, &XlsjGEvent)
				&& XlsjGEvent.xclient.message_type == XlsjGAtom1
				&& XlsjGEvent.xclient.data.l[0] == XlsjGAtom2) {
			break;
		}
		if (XCheckWindowEvent(XlsjGDsp, XlsjGBtn[1],
					ButtonPressMask, &XlsjGEvent)) {
			break;
		}
		if (XCheckWindowEvent(XlsjGDsp, XlsjGXplus,
					ButtonPressMask, &XlsjGEvent)) {
			if (nx != 10) {
				nx++;
				sprintf(str, "%2d", nx);
				XClearWindow(XlsjGDsp, XlsjGX);
				XDrawString(XlsjGDsp, XlsjGX,
						XlsjGGC, 8, 16, str, 2);
			}
		}
		if (XCheckWindowEvent(XlsjGDsp, XlsjGXminus,
					ButtonPressMask, &XlsjGEvent)) {
			if (nx != 1) {
				nx--;
				sprintf(str, "%2d", nx);
				XClearWindow(XlsjGDsp, XlsjGX);
				XDrawString(XlsjGDsp, XlsjGX,
						XlsjGGC, 8, 16, str, 2);
			}
		}
		if (XCheckWindowEvent(XlsjGDsp, XlsjGYplus,
					ButtonPressMask, &XlsjGEvent)) {
			if (ny != 10) {
				ny++;
				sprintf(str, "%2d", ny);
				XClearWindow(XlsjGDsp, XlsjGY);
				XDrawString(XlsjGDsp, XlsjGY,
						XlsjGGC, 8, 16, str, 2);
			}
		}
		if (XCheckWindowEvent(XlsjGDsp, XlsjGYminus,
					ButtonPressMask, &XlsjGEvent)) {
			if (ny != 1) {
				ny--;
				sprintf(str, "%2d", ny);
				XClearWindow(XlsjGDsp, XlsjGY);
				XDrawString(XlsjGDsp, XlsjGY,
						XlsjGGC, 8, 16, str, 2);
			}
		}
		if (XCheckWindowEvent(XlsjGDsp, XlsjGDelayplus,
					ButtonPressMask, &XlsjGEvent)) {
			if (delay != 7) {
				delay++;
				sprintf(str, "%d", delay);
				XClearWindow(XlsjGDsp, XlsjGDelay);
				XDrawString(XlsjGDsp, XlsjGDelay,
						XlsjGGC, 12, 16, str, 1);
			}
		}
		if (XCheckWindowEvent(XlsjGDsp, XlsjGDelayminus,
					ButtonPressMask, &XlsjGEvent)) {
			if (delay != 0) {
				delay--;
				sprintf(str, "%d", delay);
				XClearWindow(XlsjGDsp, XlsjGDelay);
				XDrawString(XlsjGDsp, XlsjGDelay,
						XlsjGGC, 12, 16, str, 1);
			}
		}
		if (XCheckWindowEvent(XlsjGDsp, XlsjGBtn[0],
					ButtonPressMask, &XlsjGEvent)) {
			XClearWindow(XlsjGDsp, XlsjGCanvas);
			xlsj_draw();
		}
	}

	return 0;
}

void xlsj_init(void)
{
	XlsjGDsp = XOpenDisplay(NULL);
	XlsjGRoot = XRootWindow(XlsjGDsp, 0);
	XlsjGGC = XCreateGC(XlsjGDsp, XlsjGRoot, 0, 0);
	XlsjGFont = XLoadFont(XlsjGDsp,
			"-misc-fixed-*-r-*-*-17-*-*-*-*-*-*-*");
	XSetFont(XlsjGDsp, XlsjGGC, XlsjGFont);
	XlsjGAttr.backing_store = Always;
	XlsjGSizeHints.flags = (PMinSize | PMaxSize);
	XlsjGSizeHints.min_width = 340;
	XlsjGSizeHints.min_height = 435;
	XlsjGSizeHints.max_width = 340;
	XlsjGSizeHints.max_height = 435;

	black=BlackPixel(XlsjGDsp, 0);
	white=WhitePixel(XlsjGDsp, 0);
}

void xlsj_place(void)
{
	XlsjGWindow = XCreateSimpleWindow(XlsjGDsp, XlsjGRoot,
			0, 0, 340, 435, 0, black, white);
	XChangeWindowAttributes(XlsjGDsp, XlsjGWindow,
			CWBackingStore, &XlsjGAttr);
	XlsjGAtom1 = XInternAtom(XlsjGDsp, "WM_PROTOCOLS", False);
	XlsjGAtom2 = XInternAtom(XlsjGDsp, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(XlsjGDsp, XlsjGWindow, &XlsjGAtom2, 1);

	XSetWMNormalHints(XlsjGDsp, XlsjGWindow, &XlsjGSizeHints);

	XlsjGCanvas = XCreateSimpleWindow(XlsjGDsp, XlsjGWindow,
			19, 19, 300, 300, 1, black, white);
	XChangeWindowAttributes(XlsjGDsp, XlsjGCanvas,
			CWBackingStore, &XlsjGAttr);

	XlsjGX = XCreateSimpleWindow(XlsjGDsp, XlsjGWindow,
			90, 333, 40, 19, 1, black, white);
	XChangeWindowAttributes(XlsjGDsp, XlsjGX,
			CWBackingStore, &XlsjGAttr);

	XlsjGXplus = XCreateSimpleWindow(XlsjGDsp, XlsjGWindow,
			135, 333, 9, 7, 1, black, white);
	XChangeWindowAttributes(XlsjGDsp, XlsjGXplus,
			CWBackingStore, &XlsjGAttr);
	XSelectInput(XlsjGDsp, XlsjGXplus, ButtonPressMask);

	XlsjGXminus = XCreateSimpleWindow(XlsjGDsp, XlsjGWindow,
			135, 345, 9, 7, 1, black, white);
	XChangeWindowAttributes(XlsjGDsp, XlsjGXminus,
			CWBackingStore, &XlsjGAttr);
	XSelectInput(XlsjGDsp, XlsjGXminus, ButtonPressMask);

	XlsjGY = XCreateSimpleWindow(XlsjGDsp, XlsjGWindow,
			90, 358, 40, 19, 1, black, white);
	XChangeWindowAttributes(XlsjGDsp, XlsjGY,
			CWBackingStore, &XlsjGAttr);

	XlsjGYplus = XCreateSimpleWindow(XlsjGDsp, XlsjGWindow,
			135, 358, 9, 7, 1, black, white);
	XChangeWindowAttributes(XlsjGDsp, XlsjGYplus,
			CWBackingStore, &XlsjGAttr);
	XSelectInput(XlsjGDsp, XlsjGYplus, ButtonPressMask);

	XlsjGYminus = XCreateSimpleWindow(XlsjGDsp, XlsjGWindow,
			135, 370, 9, 7, 1, black, white);
	XChangeWindowAttributes(XlsjGDsp, XlsjGYminus,
			CWBackingStore, &XlsjGAttr);
	XSelectInput(XlsjGDsp, XlsjGYminus, ButtonPressMask);

	XlsjGDelay = XCreateSimpleWindow(XlsjGDsp, XlsjGWindow,
			210, 358, 40, 19, 1, black, white);
	XChangeWindowAttributes(XlsjGDsp, XlsjGDelay,
			CWBackingStore, &XlsjGAttr);

	XlsjGDelayplus = XCreateSimpleWindow(XlsjGDsp, XlsjGWindow,
			255, 358, 9, 7, 1, black, white);
	XChangeWindowAttributes(XlsjGDsp, XlsjGDelayplus,
			CWBackingStore, &XlsjGAttr);
	XSelectInput(XlsjGDsp, XlsjGDelayplus, ButtonPressMask);

	XlsjGDelayminus = XCreateSimpleWindow(XlsjGDsp, XlsjGWindow,
			255, 370, 9, 7, 1, black, white);
	XChangeWindowAttributes(XlsjGDsp, XlsjGDelayminus,
			CWBackingStore, &XlsjGAttr);
	XSelectInput(XlsjGDsp, XlsjGDelayminus, ButtonPressMask);

	XlsjGBtn[0] = XCreateSimpleWindow(XlsjGDsp, XlsjGWindow,
			19, 388, 90, 25, 1, black, white);
	XChangeWindowAttributes(XlsjGDsp, XlsjGBtn[0],
			CWBackingStore, &XlsjGAttr);
	XSelectInput(XlsjGDsp, XlsjGBtn[0], ButtonPressMask);

	XlsjGBtn[1] = XCreateSimpleWindow(XlsjGDsp, XlsjGWindow,
			123, 388, 90, 25, 1, black, white);
	XChangeWindowAttributes(XlsjGDsp, XlsjGBtn[1],
			CWBackingStore, &XlsjGAttr);
	XSelectInput(XlsjGDsp, XlsjGBtn[1], ButtonPressMask);

	XStoreName(XlsjGDsp, XlsjGWindow, "xlissaj");
	XSetIconName(XlsjGDsp, XlsjGWindow, "xlissaj");
}

void xlsj_draw_static(void)
{
	XPoint plusbtnpoints[] = { { 0, 7 }, { 4, 0 }, { 8, 7 } };
	XPoint minusbtnpoints[] = { { 0, 0 }, { 4, 6 }, { 8, 0 } };

	int i;
	char BtnLabel[2][5] = { "Draw", "Exit" };

	XFillPolygon(XlsjGDsp, XlsjGXplus,
			XlsjGGC, plusbtnpoints, 3, Convex, CoordModeOrigin);

	XFillPolygon(XlsjGDsp, XlsjGYplus,
			XlsjGGC, plusbtnpoints, 3, Convex, CoordModeOrigin);

	XFillPolygon(XlsjGDsp, XlsjGDelayplus,
			XlsjGGC, plusbtnpoints, 3, Convex, CoordModeOrigin);

	XFillPolygon(XlsjGDsp, XlsjGXminus,
			XlsjGGC, minusbtnpoints, 3, Convex, CoordModeOrigin);

	XFillPolygon(XlsjGDsp, XlsjGYminus,
			XlsjGGC, minusbtnpoints, 3, Convex, CoordModeOrigin);

	XFillPolygon(XlsjGDsp, XlsjGDelayminus,
			XlsjGGC, minusbtnpoints, 3, Convex, CoordModeOrigin);

	for (i = 0; i < 2; i++) {
		XDrawString(XlsjGDsp, XlsjGBtn[i],
				XlsjGGC, 20, 20, BtnLabel[i], 4);
	}

	XDrawString(XlsjGDsp, XlsjGWindow,
			XlsjGGC, 20, 350, "x=sin(      t)", 14);
	XDrawString(XlsjGDsp,XlsjGWindow,
			XlsjGGC, 20, 375, "y=sin(      t-PI*      /4)", 26);

	sprintf(str, "%2d", nx);
	XDrawString(XlsjGDsp, XlsjGX, XlsjGGC, 8, 16, str, 2);

	sprintf(str, "%2d", ny);
	XDrawString(XlsjGDsp, XlsjGY, XlsjGGC, 8, 16, str, 2);

	sprintf(str, "%d", delay);
	XDrawString(XlsjGDsp, XlsjGDelay, XlsjGGC, 12, 16, str, 1);
}

void xlsj_draw(void)
{
	int x, y;
	float t;

	for (t = 0; t < 63; t = t + 0.001) {
		x = sin(nx * t) * 140;
		y = sin(ny * t - PI * delay / 4) * 140;
		XDrawPoint(XlsjGDsp, XlsjGCanvas,
				XlsjGGC, x + 150, - y + 150);
	}
}
