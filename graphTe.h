////////////////////////////////////////////////////////////
//
// graphTe - Graphical Terminal Wrapper
//
// Credits: Ene Alexandru-Florin @"inixyz" (alexandru.ene2702@gmail.com)
//
// Github: https://github.com/inixyz/graphTe
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely.
//
////////////////////////////////////////////////////////////

#ifndef _GRAPHTE_H_
#define _GRAPHTE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

//! Constant for the maximum char* size.
#define _CMAX 10000

//! Macro for the declaration of a short int.
#define int16 short int  

//! Macro for the declaration of a unsigned short int.
#define uint16 unsigned short int

////////////////////////////////////////////////////////////
/**
 * \brief   A function that re-maps a value to another range.
 * 
 * \details This functions re-maps a number from one range to another. That is, a value of inMin would get mapped
 *          to outMin, a value of inMax to outMax, values in-between to values in-between, etc.
 * 
 * \note    The "lower bounds" of either range may be smaller than the "upper bounds" so the map() function may be 
 *          used to reverse a range of numbers. The function auto-casts the given parameters to a long double data type.
 * 
 * \param[in]    x      The value that needs to be re-mapped to another range.
 * \param[in]    inMin  The lower bound of the value's current range.
 * \param[in]    inMax  The upper bound of the value's current range.
 * \param[in]    outMin The lower bound of the value's target range.
 * \param[in]    outMax The upper bound of the value's target range.
 * 
 * \return       Returns the mapped value in the target range.
 */
////////////////////////////////////////////////////////////
long double map(long double x, long double inMin, long double inMax, long double outMin, long double outMax)
{
	return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

////////////////////////////////////////////////////////////
/**
 * \brief   Structure containing a color broken down into its RGB components.
 * 
 * \details The structure contains 3 members of type uint16(in the following order), which are: red, green, blue.
 * 
 * \note    All functions from the graphTe wrapper exchange and store color data through the use of this rgb color type.
 */
////////////////////////////////////////////////////////////
typedef struct
{
	uint16 red, green, blue;
}
color;

////////////////////////////////////////////////////////////
/**
 * \brief   A function creates a color variable from the given RGB parameters.
 * 
 * \details This function provides a red, green, blue (RGB) color based on the arguments supplied.
 * 
 * \note    The intensity for each argument is in the range 0 through 255. If all three intensities are zero, the result
 *          is black. If all three intensities are 255, the result if white.
 * 
 * \param[in]    red   The intensity of the red color.
 * \param[in]    green The intensity of the green color.
 * \param[in]    blue  The intensity of the blue color.
 * 
 * \return       Returns the color from its 3 RGB components as a graphTe color type.
 */
////////////////////////////////////////////////////////////
color rgb(uint16 red, uint16 green, uint16 blue)
{
	return (color){red, green, blue};
}

////////////////////////////////////////////////////////////
/**
 * \brief   Structure containing winAPI instances.
 * 
 * \details This structure contains different winAPI variables for ease of use. A host instance variable is provided, which is used by the graphTe wrapper to interact with the OS.
 *         
 * \note    The instance can be accessed through the host variable to retrieve the window handle information.
 *          The isntance dosen't store any data before or after the initialization of the host. 
 */
////////////////////////////////////////////////////////////
struct
{
	HWND hwnd; //! A handle to the console window.
	HDC hdc, bufferDC, imageDC; //! Device context for the memory canvas and the image buffer.
	HBITMAP bufferBitmap;//! A bitmap used for memory drawing (effectively enabling double-buffering frames).
	RECT rect; //! A rectangle containing the window position and size.
	uint16 width, height; //! The width and height of the host window.
	HANDLE outputHandle; //! A handle to the standard console output.
	HPEN linePen; //! The pen that is used to draw lines.
	char title[_CMAX]; //! The title of the window.
}
host; //! An instance variable

////////////////////////////////////////////////////////////
/**
 * \brief   Enumeration containing the pre-defined vga colors available in conHost.
 * 
 * \details The enumeration containing a set of 16 pre-defined colors available in most terminals such as the the default windows terminal (conHost).
 * 
 * \note    All colors use the default VGA COLOR PALET numbering, $0 is Black while $15 is White.
 *          Since the provided terminal will be set in graphical Mode, which allows trueColor, this enumeration is useful only for modifying the text color
 *          attribute for the standard input/output.
 */
////////////////////////////////////////////////////////////
typedef enum
{
	BLACK = 0,
	NAVY = 1,
	GREEN = 2,
	TEAL = 3,
	MAROON = 4,
	PURPLE = 5,
	OLIVE = 6,
	SILVER = 7,
	GRAY = 8,
	BLUE = 9,
	LIME = 10,
	AQUA = 11,
	RED = 12,
	FUCHSIA = 13,
	YELLOW = 14,
	WHITE = 15
}
vgaColor;

////////////////////////////////////////////////////////////
/**
 * \brief   Structure containing the coords of a point. 
 * 
 * \details The structure defines the x and y coordinates of a point inside the window area.
 * 
 * \note    The main use for this data type is representing the size and scale of window objects.
 */
////////////////////////////////////////////////////////////
typedef struct
{
	uint16 x, y;
}
vector2u;

////////////////////////////////////////////////////////////
/**
 * \brief   A function that updates the rectangle of the window host.
 * 
 * \details This function retrieves a new rectangle with the position and size of the host and refreshes the instance width and height. 
 * 
 * \param   This function does not have any parameters.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void updateWindowBounds()
{
	GetWindowRect(host.hwnd, &host.rect);
	host.width = host.rect.right - host.rect.left;
	host.height = host.rect.bottom - host.rect.top;
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function that disables the flashing console cursor.
 * 
 * \details This function resizes the console cursor and negates its visibility, which makes it stop flickering on frame rendering most of the time.
 * 
 * \param   This function does not have any parameters.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void disableConsoleCursor()
{
	CONSOLE_CURSOR_INFO cInfo;
	cInfo.dwSize = 100;
    cInfo.bVisible = FALSE;
	SetConsoleCursorInfo(host.outputHandle, &cInfo);
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function that updates the memory bitmap canvas to the new window size.
 * 
 * \details This function retrieves the new window bounds and creates new bitmap buffers with the new size and position.
 * 
 * \param   This function does not have any parameters.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void update()
{
	updateWindowBounds();

	DeleteObject(host.bufferBitmap);
	host.bufferBitmap = CreateCompatibleBitmap(host.hdc, host.width, host.height);
	SelectObject(host.bufferDC, host.bufferBitmap);	

	//! A console cursor disable is required after updating the window bounds.
	disableConsoleCursor();
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function that intializes the window instance handles.
 * 
 * \details This function retrieves the window instance handles and stores then in the host variable, enabling the use of graphTe functions in the target program.
 * 
 * \note   This function must be executed before the use of any library functions that involve graphics.
 * 
 * \param   This function does not have any parameters.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void initHost()
{
	//!Sync the following window handles.
	host.hwnd = GetConsoleWindow();
	host.hdc = GetDC(host.hwnd);
	host.bufferDC = CreateCompatibleDC(host.hdc);
	host.imageDC = CreateCompatibleDC(host.bufferDC);
	SelectObject(host.bufferDC, GetStockObject(DC_PEN));
	SelectObject(host.bufferDC, GetStockObject(DC_BRUSH));
	//! Standard output handle for the text attribute function
	host.outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	//! Enabling transparent background mode for the transparentImage function.
	SetBkMode(host.bufferDC, TRANSPARENT);

	//! A console cursor disable is required after synchronizing the window handles.
	disableConsoleCursor();

	update();

	//! A thread stop of at least 100 ms is required after updating the window position and size.
	Sleep(100);
}	

////////////////////////////////////////////////////////////
/**
 * \brief   A function that releases the window handles and frees up residual memory.
 * 
 * \details This function deletes the host device context and releases teh window handle to be used by other programs.
 * 
 * \note   Any use of releaseHost() must be either at the end of the program or followed by a respective initHost().
 * 
 * \param   This function does not have any parameters.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void releaseHost()
{
	//! Deletes the residual host buffer linked to the old handle.
	DeleteObject(host.bufferBitmap);
	DeleteDC(host.bufferDC);
	DeleteObject(host.imageDC);

	//! Realeases the main window handle and device context.
	ReleaseDC(host.hwnd, host.hdc);
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function that copies the buffer device context to the host, efectively drawing to screen.
 * 
 * \details This function uses a bitmap copy to display the buffer device context(memory canvas) to the screen. Displays on screen what has been rendered to the window so far.
 *          This function is typically called after all winAPI rendering has been done for the current frame, in order to show it on screen.
 * 
 * \note   display() only updates the buffer to the memory canvas size, therefore resizing the image to the initial window size or the last call of update().
 * 
 * \param   This function does not have any parameters.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void display()
{
	BitBlt(host.hdc, 0, 0, host.width, host.height, host.bufferDC, 0, 0, SRCCOPY);
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function that moves the console cursor to another position.
 * 
 * \details This function moves the cursor to the specified screen coordinates. If the new coordinates are not within the screen rectangle set by the most recent updateWindowBounds()
 *          function call, the system automatically adjusts the coordinates so that the console cursor stays within the rectangle.
 * 
 * \note   The console cursor's coordinates must be specified in the character grid format.(IE: x:4, y:6 is the 6th line and the 4th column).
 * 
 * \param[in]   x   The new x-coordinate of the cursor, in character grid coordinates format.
 * \param[in]   y   The new y-coordinate of the cursor, in character grid coordinates format.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void moveCursor(uint16 x, uint16 y)
{
	COORD position;
	position.X = x;
	position.Y = y;
	SetConsoleCursorPosition(host.outputHandle, position);
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function that changes the color of a pixel on screen.
 * 
 * \details This function sets the pixel at the specified coordinates to the specified color.
 * 
 * \note   Multiple calls of this function can be used to create custom vertex shape drawing.
 * 
 * \param[in]   x          The x-coordinate, in logical units, of the points to be set.
 * \param[in]   y          The y-coordinate, in logical units, of the points to be set.
 * \param[in]   fillColor  The color to be used to paint the pixel. To create a graphTe color value, use the rgb() function.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void pixel(int16 x, int16 y, color fillColor)
{
	SetPixelV(host.bufferDC, x, y, RGB(fillColor.red, fillColor.green, fillColor.blue));
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function draws a rectangle on the memory canvas.
 * 
 * \details This function draws a rectangle of the specified size to the specified coordinates. The rectangle has no outline and is filled with the given graphTe color. 
 * 
 * \note   This function is more efficient than multiple pixel() calls as it gives a single GPU job call.
 *          The use of this function along with individual FillRect() calls is not advised since the DC brush color is globaly set on the host device context.
 * 
 * \param[in]   x          The x-coordinate, in logical coordinates, of the upper-left corner of the rectangle.
 * \param[in]   y          The y-coordinate, in logical coordinates, of the upper-left corner of the rectangle.
 * \param[in]   width      The width, in logical units, of the rectangle.
 * \param[in]   height     The height, in logical units, of the rectangle.
 * \param[in]   fillColor  The color to be used to paint the rectangle. To create a graphTe color value, use the rgb() function.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void rect(int16 x, int16 y, uint16 width, uint16 height, color fillColor)
{
	RECT frame;

	frame.left = x;
	frame.top = y;
	frame.right = x + width;
	frame.bottom = y + height;

	SetDCBrushColor(host.bufferDC, RGB(fillColor.red, fillColor.green, fillColor.blue));
	FillRect(host.bufferDC, &frame, GetStockObject(DC_BRUSH));
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function paints the entire screen with one color.
 * 
 * \details This function paints the screen with a single color. This function is usually called once every frame, to clear the previous contents of the target. 
 * 
 * \note   Not updating the canvas size before calling this function will result in unexpected behaviour.
 *
 * \param[in]   fillColor  The color to be used to clear the render target. To create a graphTe color value, use the rgb() function.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void fill(color fillColor)
{
	rect(0, 0, host.width, host.height, fillColor);
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function paints a line between two given points.
 * 
 * \details This function draws a line from the first given position up to, but not including the specified second position.
 * 
 * \note   This function works with any line orientation, including negative points values.
 *
 * \param[in]   x1         Specifies the x-coordinate, in logical units, of the line's start point.
 * \param[in]   y1         Specifies the y-coordinate, in logical units, of the line's start point.
 * \param[in]   x2         Specifies the x-coordinate, in logical units, of the line's end point.
 * \param[in]   y2         Specifies the y-coordinate, in logical units, of the line's end point.
 * \param[in]   width      Specifies the width, in logical units, of the line's body.
 * \param[in]   fillColor  The color that the line will be painted in. To create a graphTe color value, use the rgb() function.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void line(int16 x1, int16 y1, int16 x2, int16 y2, uint16 width, color fillColor)
{
	host.linePen = CreatePen(PS_SOLID, width, RGB(fillColor.red, fillColor.green, fillColor.blue));
	SelectObject(host.bufferDC, host.linePen);
	MoveToEx(host.bufferDC, x1, y1, NULL);
	LineTo(host.bufferDC, x2, y2);
	DeleteObject(host.linePen);
	SelectObject(host.bufferDC, GetStockObject(DC_PEN));
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function paints an ellipse at the specified point.
 * 
 * \details This function draws an ellipse. The center of the ellipse is the center of the specified bounding rectangle.
 *          The ellipse is filled with the specified fillColor and has no outline.
 * 
 * \note   This function accepts inverse-scale bounding rectangles.
 *
 * \param[in]   x          Specifies the x-coordinate, in logical units, of the bounding rectangle's upper-left corner.
 * \param[in]   y          Specifies the y-coordinate, in logical units, of the bounding rectangle's upper-left corner.
 * \param[in]   width      The width, in logical units, of the bounding rectangle.
 * \param[in]   height     The height, in logical units, of the bounding rectangle.
 * \param[in]   fillColor  The color that the ellipse will be painted in. To create a graphTe color value, use the rgb() function.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void ellipse(int16 x, int16 y, uint16 width, uint16 height, color fillColor)
{
	SetDCPenColor(host.bufferDC, RGB(fillColor.red, fillColor.green, fillColor.blue));
	SetDCBrushColor(host.bufferDC, RGB(fillColor.red, fillColor.green, fillColor.blue));
	Ellipse(host.bufferDC, x, y, x + width, y + height);
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function paints a circle at the specified point.
 * 
 * \details This function draws a circle. The center of the ellipse is the center of the specified bounding rectangle.
 * 			The bounding rectangle's width and height are equal to the diameter of the circle.
 *          The ellipse is filled with the specified fillColor and has no outline.
 * 
 * \note   This function accepts inverse-scale bounding rectangles.
 *          This function serves as a convienience wrapper for the ellipse function.
 *
 * \param[in]   x          Specifies the x-coordinate, in logical units, of the bounding rectangle's upper-left corner.
 * \param[in]   y          Specifies the y-coordinate, in logical units, of the bounding rectangle's upper-left corner.
 * \param[in]   radius     The size, in logical units, of the circle's radius. (The width\height of the bounding rectangle will be twice the amount of this)
 * \param[in]   fillColor  The color that the circle will be painted in. To create a graphTe color value, use the rgb() function.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void circle(int16 x, int16 y, uint16 radius, color fillColor)
{
	ellipse(x, y, 2 * radius, 2 * radius, fillColor);
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function draws an image at the specified point.
 * 
 * \details This function renders a external bitmap buffer to the specified screen bounding rectangle. This function can load from a file a
 * 	        bitmap, icon, cursor or animated cursor. The primary use of this function is to render images on screen.
 * 
 * \note   This function accepts inverse-scale bounding rectangles.
 *
 * \param[in]   x            Specifies the x-coordinate, in logical units, of the bounding rectangle's upper-left corner.
 * \param[in]   y            Specifies the y-coordinate, in logical units, of the bounding rectangle's upper-left corner.
 * \param[in]   width        The width, in logical units, of the bounding rectangle.
 * \param[in]   height       The height, in logical units, of the bounding rectangle.
 * \param[in]   filenamePTR  A reference to a constant file path that will be used to retrieve the bitmap.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void image(int16 x, int16 y, uint16 width, uint16 height, char* filenamePTR)
{
	char filename[_CMAX];
	strcpy(filename, filenamePTR); 
	HBITMAP imageBitmap = LoadImageA(NULL, filename, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	SelectObject(host.imageDC, imageBitmap);

	BitBlt(host.bufferDC, x, y, width, height, host.imageDC, 0, 0, SRCCOPY);
	
	DeleteObject(imageBitmap);
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function draws an image with transparency at the specified point.
 * 
 * \details This function renders a external bitmap buffer to the specified screen bounding rectangle. This function can load from a file a
 * 	        bitmap, icon, cursor or animated cursor. The primary use of this function is to render images on screen. This function acts different from the 
 *          image() function by the use of an additional parameter transparentColor. The bitmap image will have every pixel that matches the transparentColor
 *          skipped, making a similar effect to transparent image formats.
 * 
 * \note   This function accepts inverse-scale bounding rectangles.
 *
 * \param[in]   x                Specifies the x-coordinate, in logical units, of the bounding rectangle's upper-left corner.
 * \param[in]   y                Specifies the y-coordinate, in logical units, of the bounding rectangle's upper-left corner.
 * \param[in]   width            The width, in logical units, of the bounding rectangle.
 * \param[in]   height           The height, in logical units, of the bounding rectangle.
 * \param[in]   filenamePTR      A reference to a constant file path that will be used to retrieve the bitmap.
 * \param[in]   transparentColor The color that will be replaced with transparency at the render step. To create a graphTe color value, use the rgb() function.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void transparentImage(int16 x, int16 y, uint16 width, uint16 height, char* filenamePTR, color transparentColor)
{
	char filename[_CMAX];
	strcpy(filename, filenamePTR); 
	HBITMAP imageBitmap = LoadImageA(NULL, filename, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	SelectObject(host.imageDC, imageBitmap);

	TransparentBlt(host.bufferDC, x, y, width, height, host.imageDC, 0, 0, width, height, RGB(transparentColor.red, transparentColor.green, transparentColor.blue));

	DeleteObject(imageBitmap);
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function draws text in specified bounding rectangle.
 * 
 * \details This function draws formatted text in the specified bounding rectangle. It formats the text according to the specified
 *          method (expanding tabs, justifying charecters, breaking lines, and so forth).
 * 
 * \note    This function accepts inverse-scale bounding rectangles.
 *
 * \param[in]   x          Specifies the x-coordinate, in logical units, of the bounding rectangle's upper-left corner.
 * \param[in]   y          Specifies the y-coordinate, in logical units, of the bounding rectangle's upper-left corner.
 * \param[in]   width      The width, in logical units, of the bounding rectangle.
 * \param[in]   height     The height, in logical units, of the bounding rectangle.
 * \param[in]   textPTR    A reference to a constant char* containing the desired text to draw.
 * \param[in]   fillColor  The color that will be used to paint the text. To create a graphTe color value, use the rgb() function.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void textRect(int16 x, int16 y, uint16 width, uint16 height, char* textPTR, color fillColor)
{
	char text[_CMAX];
	strcpy(text, textPTR);

	RECT textBox;
	textBox.left = x;
	textBox.top = y;
	textBox.right = x + width;
	textBox.bottom = y + height;

	SetTextColor(host.bufferDC, RGB(fillColor.red, fillColor.green, fillColor.blue));
	DrawText(host.bufferDC, text, strlen(text), &textBox, DT_LEFT);
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function draws text at a specified point on the screen.
 * 
 * \details This function draws formatted text at the specified coordinates of the screen. It formats the text according to the specified
 *          method (expanding tabs, justifying charecters, breaking lines, and so forth).
 * 
 * \note   This function accepts coordinates outside of the window area.
 *
 * \param[in]   x          Specifies the x-coordinate, in logical units, of the point at which the first character will appear.
 * \param[in]   y          Specifies the y-coordinate, in logical units, of the point at which the first character will appear.
 * \param[in]   textPTR    A reference to a constant char* containing the desired text to draw.
 * \param[in]   fillColor  The color that will be used to paint the text. To create a graphTe color value, use the rgb() function.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void text(int16 x, int16 y, char* textPTR, color fillColor)
{
	char text[_CMAX];
	strcpy(text, textPTR);

	RECT textBox;
	textBox.left = x;
	textBox.top = y;
	textBox.right = x + 10;
	textBox.bottom = y + 10;

	SetTextColor(host.bufferDC, RGB(fillColor.red, fillColor.green, fillColor.blue));
	DrawText(host.bufferDC, text, strlen(text), &textBox, DT_NOCLIP);
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function that retrieves a VGA color code from the specified colors.
 * 
 * \details This function creates the VGA color code from the given text fill color and text background color.
 * 
 * \param[in]   fillColor  The color in graphTe VGA color format of the text fill.
 * \param[in]   bgColor    The color in graphTe VGA color format of the text background.
 * 
 * \return  This function returns a VGA color code of the coresponding color.
 */
////////////////////////////////////////////////////////////
uint16 getVGAColor(vgaColor fillColor, vgaColor bgColor)
{
	return bgColor * 16 + fillColor;
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function that changes the color of the standard input/output text.
 * 
 * \details This function changes the color that the following standard input/output subrutines to the specified
 *          foreground and background colors.
 * 
 * \note    The color of the text will remain changed until the next function call.
 * 
 * \param[in]   fillColor  The color in graphTe VGA color format of the text fill.
 * \param[in]   bgColor    The color in graphTe VGA color format of the text background.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void setPrintColor(vgaColor fillColor, vgaColor bgColor)
{
	SetConsoleTextAttribute(host.outputHandle, getVGAColor(fillColor, bgColor));
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function that changes the tile of the window.
 * 
 * \details This function modifies the title of the conHost window.
 * 
 * \param[in]  title  A reference to the new window title.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void setWindowTitle(char* title)
{
	SetConsoleTitle(title);
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function that retrieves the current window title.
 * 
 * \details This function retrieves the current title of the conHost window.
 * 
 * \param   This function does not have any parameters.
 * 
 * \return  This function return a reference to the current window title.
 */
////////////////////////////////////////////////////////////
char* getWindowTitle()
{	
	GetWindowTextA(host.hwnd, host.title, _CMAX);
	return host.title;
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function that modifes the size of the window.
 * 
 * \details This function resizes the conHost window to the specified widht/height.
 * 
 * \note    The provided window size must be bigger than 120/120 pixels and smaller than 4000/4000 pixels.
 * 
 * \param[in]  width   The width, in logical units, that the new window will have.
 * \param[in]  height  The height, in logical units, that the new window will have.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void setWindowSize(uint16 width, uint16 height)
{
	updateWindowBounds();
	MoveWindow(host.hwnd, host.rect.left, host.rect.top, width, height, TRUE);

	Sleep(100);
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function that retrieves the size of the current window.
 * 
 * \details This function retrieves the size of the current conHost window, while also updating the memory window bounds.
 * 
 * \param   This function does not have any parameters.
 * 
 * \return  This function returns a structure containing the width and height of the current window.
 */
////////////////////////////////////////////////////////////
vector2u getWindowSize()
{
	updateWindowBounds();
	return (vector2u){host.width, host.height};
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function that modifes the position of the window.
 * 
 * \details This function moves the conHost window to the specified coordinates.
 * 
 * \param[in]  x   The x-coordinate, in logical units, of the upper-left cornet of the new window position.
 * \param[in]  y   The y-coordinate, in logical units, of the upper-left cornet of the new window position.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void setWindowPosition(uint16 x, uint16 y)
{
	updateWindowBounds();
	MoveWindow(host.hwnd, x, y, host.width, host.height, TRUE);
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function that retrives the current position of the window.
 * 
 * \details This function retrieves the position, in logical coordiantes of the current conHost window 
 * 
 * \param   This function does not have any parameters.
 * 
 * \return  This function returns a structure containing the x and y coordinate of the upper-left corner of the window.
 */
////////////////////////////////////////////////////////////
vector2u getWindowPosition()
{
	updateWindowBounds();
	return (vector2u){host.rect.left, host.rect.top};
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function that retrieves the Window Event type input.
 * 
 * \details This function retrieves the pressed key at the function call frame.
 * 
 * \note    This function should not be used without a certain reason as event type input can be unexpected when using a frame based system.
 * 
 * \param   This function does not have any parameters.
 * 
 * \return  This function returns a character coresponding to the pressed key or 0 if no key is pressed.
 */
////////////////////////////////////////////////////////////
char input()
{
	if(kbhit())
		return getch();
	else 
		return 0;
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function that retrieves the Window Event type input while blocking program execution.
 * 
 * \details This function blocks program execution until a key is presssed. The key gets retrived using the standard window Event input type.
 * 
 * \note    This function should not be used without a certain reason as event type input can be unexpected when using a frame based system.
 *          In some circumstances this function will not block the program execution.
 * 
 * \param   This function does not have any parameters.
 * 
 * \return  This function returns a character coresponding to the pressed key.
 */
////////////////////////////////////////////////////////////
char forceInput()
{
	return getch();
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function that retrieves the live state of any given key.
 * 
 * \details This function retrieves the live input state of any specified key or button of the OS handler.
 * 
 * \note    This function takes a WORD paremater for the key structure making it compatible with winAPI virtual key codes.
 *          These codes can be found here:  https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes .
 * 			A hex value can be passed as a paremter instead in case of unspecified keys.
 *          If this function is used for mouse input , the quick-edit mode of conHost must be disabled.
 * 
 * \param[in]  keyCode  The WORD representing the code for the virtual key which state is referenced.
 * 
 * \return  This function returns TRUE if the key is pressed and FALSE if the key if not pressed. 
 */
////////////////////////////////////////////////////////////
BOOL checkKeyLiveInput(WORD keyCode)
{
	return GetAsyncKeyState(keyCode);
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function that retrieves the coordinates of the mouse position referenced to the window.
 * 
 * \details This function retrieves the position of teh mouse cursorm, in screen coordinates. The coordinates are re-maped in reference
 *          to the window size and position.
 * 
 * \param   This function does not havy any parameters.
 * 
 * \return  This function returns a structure containing the coordinates of the mouse position.
 */
////////////////////////////////////////////////////////////
POINT getMousePosition()
{
	POINT mPos;
	GetCursorPos(&mPos);
	ScreenToClient(GetForegroundWindow(), &mPos);

	return mPos;
}

////////////////////////////////////////////////////////////
/**
 * \brief   A function that plays a sound from an external file.
 * 
 * \details This function plays a sound specified by the given file name. This function call does not stop
 *          program execution because the sound is played in an asynchronous matter.
 * 
 * \note    The specified path must be smaller than 256 characters.
 *          The recomended format for the sound file is ".wav".
 * 
 * \param[in]  filenamePTR  A reference to a constant char* with the path to the file to be played.
 * 
 * \return  This function does not return anything.
 */
////////////////////////////////////////////////////////////
void playSound(char* filenamePTR)
{
	char filename[_CMAX];
	strcpy(filename, filenamePTR);

	PlaySoundA(filename, NULL, SND_FILENAME | SND_ASYNC);
}

#endif //end graphTe.h

/*! \mainpage
 * 
 * \section intro_sec GraphTe is multi-media
 * 
 *     GraphTe provides a simple interface to the various components of the ambiguous
 *     WINAPI module of the MinGW and Visual C++ compilers. It is composed of three modules:
 *     graphics, audio and input/output that are bundled up in a single header for ease of use.
 *     
 * \section second_sec GraphTe is compatible with C & C++
 * 
 *     GraphTe has been created with the use of the C programming language in mind. The wrapper does
 *     not use any superset incompatiblities, which makes it compatible with C++.
 * 
 * \section third_sec Compiling your project with GraphTe
 * 
 *     Compiling your project with GraphTe is as simple as placing the single header file in your
 *     source directory and linking the required modules with your compiler: gdi32, msimg32, winmm
 * 
 *     Example of a compile command: gcc *.c -lgdi32 -lmsimg32 -lwinmm
 */