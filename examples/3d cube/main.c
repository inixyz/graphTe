#include "graphTe.h"
#include <time.h>
#include <math.h>

#define PI 3.14159265359

typedef struct
{
	double x, y, z;
}POINT3D;

typedef struct 
{
	int x, y;
}POINT2D;

int main()
{
	initHost();

	//constants:
	color COLOR_BG = (color){50, 50, 50, 1};
	color COLOR_CUBE = (color){0, 255, 255, 1};

	double SPEED_X = 0.05;
	double SPEED_Y = 0.15;
	double SPEED_Z = 0.10;

	double h = 1000;
	double w = 1000;

	double lineWidth = w / 100;

	double timeDelta, timeNow, timeLast = 0;

	double cx = w / 2;
	double cy = h / 2;
	double cz = 0;
	double size = h / 4;
	POINT3D vertices[8] = 
	{
		{cx - size, cy - size, cz - size},
		{cx + size, cy - size, cz - size},
		{cx + size, cy + size, cz - size},
		{cx - size, cy + size, cz - size},
		{cx - size, cy - size, cz + size},
		{cx + size, cy - size, cz + size},
		{cx + size, cy + size, cz + size},
		{cx - size, cy + size, cz + size}
	};

	POINT2D edges[12] = 
	{
		{0, 1}, {1, 2}, {2, 3}, {3, 0}, //back face
		{4, 5}, {5, 6}, {6, 7}, {7, 4}, //front face
		{0, 4}, {1, 5}, {2, 6}, {3, 7}  //connecting sides 
	};

	setWindowTitle("3d cube");
	setWindowSize(w, h);
	update();

	while(1)
	{
		timeNow = clock();
		timeDelta =  timeNow - timeLast;
		timeLast = timeNow;

		//background:
		fill(COLOR_BG);

		//rotate the cube along the z axis
		double angle = timeDelta * 0.001 * SPEED_Z * PI * 2;
		for(int i = 0; i < 8; i++)
		{
			double dx = vertices[i].x - cx;
			double dy = vertices[i].y - cy;
			double x = dx * cos(angle) - dy * sin(angle);
			double y = dx * sin(angle) + dy * cos(angle);

			vertices[i].x = x + cx;
			vertices[i].y = y + cy;
		}

		//rotate the cube along the x axis
		angle = timeDelta * 0.001 * SPEED_X * PI * 2;
		for(int i = 0; i < 8; i++)
		{
			double dy = vertices[i].y - cy;
			double dz = vertices[i].z - cz;
			double y = dy * cos(angle) - dz * sin(angle);
			double z = dy * sin(angle) + dz * cos(angle);

			vertices[i].y = y + cy;
			vertices[i].z = z + cz;
		}

		//rotate the cube along the y axis
		angle = timeDelta * 0.001 * SPEED_Y * PI * 2;
		for(int i = 0; i < 8; i++)
		{
			double dx = vertices[i].x - cx;
			double dz = vertices[i].z - cz;
			double x = dz * sin(angle) + dx * cos(angle);
			double z = dz * cos(angle) - dx * sin(angle);

			vertices[i].x = x + cx;
			vertices[i].z = z + cz;
		}


		//draw the cube:
		for(int i = 0; i < 12; i++)
		{
			line(vertices[edges[i].x].x, vertices[edges[i].x].y, vertices[edges[i].y].x, vertices[edges[i].y].y, lineWidth, COLOR_CUBE);
		}

		display();
	}
		
	releaseHost();
}
