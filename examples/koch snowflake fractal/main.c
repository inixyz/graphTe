#include "graphTe.h"
#include <math.h>

typedef struct
{
	long double x, y;
}
point;

#define MAX_I 4
#define lWidth 2
#define w 1000
#define h 1000

color lColor = {100, 255, 100, TRUE};
color bgColor = {10, 10, 10, TRUE};



void kochLine(point start, point end, unsigned int i)
{
	if(i >= MAX_I)
		line(start.x, start.y, end.x, end.y, lWidth, lColor); //graphTe function
	else
	{
		point a = start;
		point b = {start.x * 2/3 + end.x * 1/3, start.y * 2/3 + end.y * 1/3};
		point d = {start.x * 1/3 + end.x * 2/3, start.y * 1/3 + end.y * 2/3};
		point c = {(d.x - b.x) * cos(-M_PI/3) - (d.y - b.y) * sin(-M_PI/3) + b.x, (d.x - b.x) * sin(-M_PI/3) + (d.y - b.y) * cos(-M_PI/3) + b.y};
		point e = end;

		kochLine(a, b, i + 1);
		kochLine(b, c, i + 1);
		kochLine(c, d, i + 1);
		kochLine(d, e, i + 1);
	}
}

int main()
{
	initHost(); //graphTe function

	setWindowTitle("Triangle fractal"); //graphTe function
	setWindowSize(w, h); //graphTe function

	update(); //graphTe function

	point a = {500, 300}, b = {750, 700}, c = {250, 700};
	fill(bgColor); //graphTe function

	kochLine(a, b, 0);
	kochLine(b, c, 0);
	kochLine(c, a, 0);

	display(); //graphTe function

	forceInput(); //graphTe function

	releaseHost(); //graphTe function
}