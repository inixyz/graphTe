#include "graphTe.h"

int maxI = 100, INF = 10;

int main()
{
	initHost();
	setWindowTitle("Mandelbrot's Set");
	setWindowSize(1000, 1000);
	update();

	double a1, a2, b1, b2, i, fx, fy;
	color colorVal = rgb(0, 0, 0);

	for(int x = 0; x < 1000; x++)
	{
		for(int y = 0; y < 1000; y++)
		{
			fx = map(x, 0, 1000, -2, 2);
			fy = map(y, 0, 1000, -2, 2);

			i = 0;

			a1 = fx;
			b1 = fy;

			while(i++ < maxI)
			{
				a2 = a1 * a1 - b1 * b1;
				b2 = 2 * a1 * b1;

				a1 = a2 + fx;
				b1 = b2 + fy;

				if(abs(a1 + a2) > INF)
					break;
			}

			colorVal.red = map(i, 0, maxI, 0, 255);
			colorVal.green = map(i, 0, maxI, 0, 255);
			colorVal.blue = map(i, 80, maxI, 80, 255);
			pixel(x, y, colorVal);
		}
	}

	display();

	forceInput();

	releaseHost();
}